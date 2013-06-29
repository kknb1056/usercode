#include "l1menu/ReducedMenuSample.h"

#include <vector>
#include <stdexcept>
#include <fcntl.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "l1menu/ReducedEvent.h"
#include "l1menu/MenuSample.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/ITrigger.h"
#include "l1menu/ICachedTrigger.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include "l1menu/IEvent.h"
#include "l1menu/IMenuRate.h"
#include "l1menu/ITriggerRate.h"
#include "l1menu/tools/tools.h"
#include "protobuf/l1menu.pb.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>

namespace // unnamed namespace
{
	/** @brief Sentry that closes a Unix file descriptor when it goes out of scope.
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 07/Jun/2013
	 */
	class UnixFileSentry
	{
	public:
		UnixFileSentry( int fileDescriptor ) : fileDescriptor_(fileDescriptor) {}
		~UnixFileSentry() { close(fileDescriptor_); }
	private:
		int fileDescriptor_;
	};

	class MenuRateImplementation;

	class TriggerRateImplementation : public l1menu::ITriggerRate
	{
	public:
		TriggerRateImplementation( const l1menu::ITrigger& trigger, float fraction, const MenuRateImplementation& menuRate );
		TriggerRateImplementation& operator=( TriggerRateImplementation&& otherTriggerRate ); // Move assignment
		virtual ~TriggerRateImplementation();

		// Methods required by the l1menu::ITriggerRate interface
		virtual const l1menu::ITrigger& trigger() const;
		virtual float fraction() const;
		virtual float rate() const;
	protected:
		std::unique_ptr<l1menu::ITrigger> pTrigger_;
		float fraction_;
		const MenuRateImplementation& menuRate_;
	};

	class MenuRateImplementation : public l1menu::IMenuRate
	{
	public:
		void setTotalFraction( float fraction ) { totalFraction_=fraction; }
		void setScaling( float scaling ) { scaling_=scaling; }
		float scaling() const { return scaling_; }
		void addTriggerRate( const l1menu::ITrigger& trigger, float fraction ) { triggerRates_.push_back( std::move(TriggerRateImplementation(trigger,fraction,*this)) ); }
		// Methods required by the l1menu::IMenuRate interface
		virtual float totalFraction() const { return totalFraction_; }
		virtual float totalRate() const { return totalFraction_*scaling_; }
		virtual const std::vector<const l1menu::ITriggerRate*>& triggerRates() const
		{
			// If the sizes are the same I'll assume nothing has changed and the references
			// are still valid. I don't expect this method to be called until the triggerRates_
			// vector is complete anyway.
			if( triggerRates_.size()!=baseClassReferences_.size() )
			{
				baseClassReferences_.clear();
				for( const auto& triggerRate : triggerRates_ )
				{
					baseClassReferences_.push_back( &triggerRate );
				}
			}

			return baseClassReferences_;
		}

	protected:
		float totalFraction_;
		float scaling_;
		std::vector<TriggerRateImplementation> triggerRates_;
		mutable std::vector<const l1menu::ITriggerRate*> baseClassReferences_;
	};

	/** @brief An object that stores pointers to trigger parameters to avoid costly string comparisons.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 26/Jun/2013
	 */
	class CachedTriggerImplementation : public l1menu::ICachedTrigger
	{
	public:
		CachedTriggerImplementation( const l1menu::ReducedMenuSample& sample, const l1menu::ITrigger& trigger )
		{
			const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers(trigger);

			for( const auto& identifier : parameterIdentifiers )
			{
				identifiers_.push_back( std::make_pair( identifier.second, &trigger.parameter(identifier.first) ) );
			}
		}
		virtual bool apply( const l1menu::IEvent& event )
		{
			// Not happy using a static_cast, but this method is called in many, many loops.
			// I should probably find out how much faster this is than a dynamic_cast, maybe
			// it's not even worth it. Anyway, I'm banking that no one will ever pass an
			// event that wasn't created with the same sample that this proxy was created
			// with.
			const l1menu::ReducedEvent* pEvent=static_cast<const l1menu::ReducedEvent*>(&event);
			for( const auto& identifier : identifiers_ )
			{
				if( pEvent->parameterValue(identifier.first) < *identifier.second ) return false;
			}

			// If control got this far then all of the thresholds passed, and
			// I can pass the event.
			return true;
		}
	protected:
		std::vector< std::pair<l1menu::ReducedEvent::ParameterID,const float*> > identifiers_;
	}; // end of class ReducedSampleCachedTrigger

	TriggerRateImplementation::TriggerRateImplementation( const l1menu::ITrigger& trigger, float fraction, const MenuRateImplementation& menuRate )
		: fraction_(fraction), menuRate_(menuRate)
	{
		pTrigger_=std::move( l1menu::TriggerTable::instance().copyTrigger(trigger) );
	}
	TriggerRateImplementation& TriggerRateImplementation::operator=( TriggerRateImplementation&& otherTriggerRate )
	{
		pTrigger_=std::move( otherTriggerRate.pTrigger_ );
		fraction_=otherTriggerRate.fraction_;
		// I can't change the menuRate_ reference, but that should already be set to the right one anyway.
		return *this;
	}
	TriggerRateImplementation::~TriggerRateImplementation() {}
	const l1menu::ITrigger& TriggerRateImplementation::trigger() const { return *pTrigger_; }
	float TriggerRateImplementation::fraction() const { return fraction_; }
	float TriggerRateImplementation::rate() const { return fraction_*menuRate_.scaling(); }

	float sumWeights( const l1menuprotobuf::Run& run )
	{
		float returnValue=0;
		for( const auto& event : run.event() )
		{
			if( event.has_weight() ) returnValue+=event.weight();
			else returnValue+=1;
		}
		return returnValue;
	}
}

namespace l1menu
{
	/** @brief Private members for the ReducedMenuSample class
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/May/2013
	 */
	class ReducedMenuSamplePrivateMembers
	{
	private:
		l1menu::TriggerMenu mutableTriggerMenu_;
	public:
		ReducedMenuSamplePrivateMembers( const l1menu::ReducedMenuSample& thisObject, const l1menu::TriggerMenu& newTriggerMenu );
		ReducedMenuSamplePrivateMembers( const l1menu::ReducedMenuSample& thisObject, const std::string& filename );
		//void copyMenuToProtobufSample();
		l1menu::ReducedEvent event;
		const l1menu::TriggerMenu& triggerMenu; // External const access to mutableTriggerMenu_
		float eventRate;
		float sumOfWeights;
		l1menuprotobuf::SampleHeader protobufSampleHeader;
		// Protobuf doesn't implement move semantics so I'll use pointers
		std::vector<std::unique_ptr<l1menuprotobuf::Run> > protobufRuns;
		const static int EVENTS_PER_RUN;
		const static char PROTOBUF_MESSAGE_DELIMETER;
		const static std::string FILE_FORMAT_MAGIC_NUMBER;
	};

	const int ReducedMenuSamplePrivateMembers::EVENTS_PER_RUN=20000;
	const char ReducedMenuSamplePrivateMembers::PROTOBUF_MESSAGE_DELIMETER='\n';
	const std::string ReducedMenuSamplePrivateMembers::FILE_FORMAT_MAGIC_NUMBER="l1menuReducedMenuSample";
}

l1menu::ReducedMenuSamplePrivateMembers::ReducedMenuSamplePrivateMembers( const l1menu::ReducedMenuSample& thisObject, const l1menu::TriggerMenu& newTriggerMenu )
	: mutableTriggerMenu_( newTriggerMenu ), event(thisObject), triggerMenu( mutableTriggerMenu_ ), eventRate(1), sumOfWeights(0)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// I need to copy the details of the trigger menu into the protobuf storage.
	// This means I'm holding a duplicate, but I need it to write the sample to a
	// protobuf file, so I might as well do it now.
	for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& trigger=triggerMenu.getTrigger(triggerNumber);

		l1menuprotobuf::Trigger* pProtobufTrigger=protobufSampleHeader.add_trigger();
		pProtobufTrigger->set_name( trigger.name() );
		pProtobufTrigger->set_version( trigger.version() );

		// Record all of the parameters. It's not strictly necessary to record the values
		// of the parameters that are recorded for each event, but I might as well so that
		// the trigger menu is loaded exactly as it was saved.
		const auto parameterNames=trigger.parameterNames();
		for( const auto& parameterName : parameterNames )
		{
			l1menuprotobuf::Trigger_TriggerParameter* pProtobufParameter=pProtobufTrigger->add_parameter();
			pProtobufParameter->set_name(parameterName);
			pProtobufParameter->set_value( trigger.parameter(parameterName) );
		}

		// Make a note of the names of the parameters that are recorded for each event. For this
		// I'm just recording the parameters that refer to the thresholds.
		const auto thresholdNames=l1menu::tools::getThresholdNames(trigger);
		for( const auto& thresholdName : thresholdNames ) pProtobufTrigger->add_varying_parameter(thresholdName);

	} // end of loop over triggers

	// Always make sure there is at least one Run ready to be added to
	std::unique_ptr<l1menuprotobuf::Run> pNewRun( new l1menuprotobuf::Run );
	protobufRuns.push_back( std::move( pNewRun ) );

}

l1menu::ReducedMenuSamplePrivateMembers::ReducedMenuSamplePrivateMembers( const l1menu::ReducedMenuSample& thisObject, const std::string& filename )
	: event(thisObject), triggerMenu(mutableTriggerMenu_), eventRate(1), sumOfWeights(0)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// Open the file with read ability
	int fileDescriptor = open( filename.c_str(), O_RDONLY );
	if( fileDescriptor==0 ) throw std::runtime_error( "ReducedMenuSample initialise from file - couldn't open file" );
	::UnixFileSentry fileSentry( fileDescriptor ); // Use this as an exception safe way of closing the input file
	google::protobuf::io::FileInputStream fileInput( fileDescriptor );

	// First read the magic number at the start of the file and make sure it
	// matches what I expect. This is uncompressed so I'll wrap it in a block
	// to make sure the CodedInputStream is destructed before creating a new
	// one with gzip input.
	{
		google::protobuf::io::CodedInputStream codedInput( &fileInput );

		// As a read buffer, I'll create a string the correct size (filled with an arbitrary
		// character) and read straight into that.
		std::string readMagicNumber;
		if( !codedInput.ReadString( &readMagicNumber, FILE_FORMAT_MAGIC_NUMBER.size() ) ) throw std::runtime_error( "ReducedMenuSample initialise from file - error reading magic number" );
		if( readMagicNumber!=FILE_FORMAT_MAGIC_NUMBER ) throw std::runtime_error( "ReducedMenuSample - tried to initialise with a file that is not the correct format" );

		google::protobuf::uint32 fileformatVersion;
		if( !codedInput.ReadVarint32( &fileformatVersion ) ) throw std::runtime_error( "ReducedMenuSample initialise from file - error reading file format version" );
		// So far I only have (and ever expect to have) one version of the file
		// format, imaginatively versioned "1". You never know though...
		if( fileformatVersion>1 ) std::cerr << "Warning: Attempting to read a ReducedMenuSample with version " << fileformatVersion << " with code that only knows up to version 1." << std::endl;
	}

	google::protobuf::io::GzipInputStream gzipInput( &fileInput );
	google::protobuf::io::CodedInputStream codedInput( &gzipInput );

	// Disable warnings on this input stream (second parameter, -1). The
	// first parameter is the default. I'll change this if necessary in
	// the loop later.
	size_t totalBytesLimit=67108864;
	codedInput.SetTotalBytesLimit( totalBytesLimit, -1 );

	google::protobuf::uint64 messageSize;

	// Read the size of the header message
	if( !codedInput.ReadVarint64( &messageSize ) ) throw std::runtime_error( "ReducedMenuSample initialise from file - error reading message size for header" );
	google::protobuf::io::CodedInputStream::Limit readLimit=codedInput.PushLimit(messageSize);
	if( !protobufSampleHeader.ParseFromCodedStream( &codedInput ) ) throw std::runtime_error( "ReducedMenuSample initialise from file - some unknown error while reading header" );
	codedInput.PopLimit(readLimit);

	// Keep looping until there is nothing more to be read from the file.
	while( codedInput.ReadVarint64( &messageSize ) )
	{
		readLimit=codedInput.PushLimit(messageSize);

		// Make sure the CodedInputStream doesn't refuse to read the message because it's
		// read too much already. I'll also add an arbitrary 50 on to always make sure
		// I can read the next messageSize if there is one.
		if( gzipInput.ByteCount()+messageSize+50 > totalBytesLimit )
		{
			totalBytesLimit+=messageSize*5; // Might as well set it a little higher than necessary while I'm at it.
			codedInput.SetTotalBytesLimit( totalBytesLimit, -1 );
		}
		std::unique_ptr<l1menuprotobuf::Run> pNewRun( new l1menuprotobuf::Run );
		if( !pNewRun->ParseFromCodedStream( &codedInput ) ) throw std::runtime_error( "ReducedMenuSample initialise from file - some unknown error while reading run" );
		protobufRuns.push_back( std::move( pNewRun ) );

		codedInput.PopLimit(readLimit);
	}


	// Always make sure there is at least one Run ready to be added to. Later
	// code assumes there is already a run there.
	if( protobufRuns.empty() )
	{
		std::unique_ptr<l1menuprotobuf::Run> pNewRun( new l1menuprotobuf::Run );
		protobufRuns.push_back( std::move( pNewRun ) );
	}

	// Count up the sum of the weights of all events
	for( const auto& pRun : protobufRuns )
	{
		sumOfWeights+=sumWeights( *pRun );
	}

	// I have all of the information in the protobuf members, but I also need the trigger information
	// in the form of l1menu::TriggerMenu. Copy out the required information.
	for( int triggerNumber=0; triggerNumber<protobufSampleHeader.trigger_size(); ++triggerNumber )
	{
		const l1menuprotobuf::Trigger& inputTrigger=protobufSampleHeader.trigger(triggerNumber);

		mutableTriggerMenu_.addTrigger( inputTrigger.name(), inputTrigger.version() );
		// Get a reference to the trigger I just created
		l1menu::ITrigger& trigger=mutableTriggerMenu_.getTrigger(triggerNumber);

		// Run through all of the parameters and set them to what they were
		// when the sample was made.
		for( int parameterNumber=0; parameterNumber<inputTrigger.parameter_size(); ++parameterNumber )
		{
			const auto& inputParameter=inputTrigger.parameter(parameterNumber);
			trigger.parameter(inputParameter.name())=inputParameter.value();
		}

		// I should probably check the threshold names exist. I'll do it another time.
	}

}

l1menu::ReducedMenuSample::ReducedMenuSample( const l1menu::MenuSample& originalSample, const l1menu::TriggerMenu& triggerMenu )
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( *this, triggerMenu ) )
{
	addSample( originalSample );
}

l1menu::ReducedMenuSample::ReducedMenuSample( const l1menu::TriggerMenu& triggerMenu )
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( *this, triggerMenu ) )
{
	// No operation besides the initialiser list
}

l1menu::ReducedMenuSample::ReducedMenuSample( const std::string& filename )
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( *this, filename ) )
{
	// No operation except the initialiser list
}

l1menu::ReducedMenuSample::~ReducedMenuSample()
{
	// No operation. Just need one defined otherwise the default one messes up
	// the unique_ptr deletion because ReducedMenuSamplePrivateMembers isn't
	// defined elsewhere.
}

void l1menu::ReducedMenuSample::addSample( const l1menu::MenuSample& originalSample )
{
	l1menuprotobuf::Run* pCurrentRun=pImple_->protobufRuns.back().get();

	for( size_t eventNumber=0; eventNumber<originalSample.numberOfEvents(); ++eventNumber )
	{
		// Split the events up into groups in arbitrary numbers. This is to get around
		// a protobuf aversion to long messages.
		if( pCurrentRun->event_size() >= pImple_->EVENTS_PER_RUN )
		{
			// Gone over the arbitrary limit, so create a new protobuf Run and start
			// using that instead.
			std::unique_ptr<l1menuprotobuf::Run> pNewRun( new l1menuprotobuf::Run );
			pImple_->protobufRuns.push_back( std::move( pNewRun ) );
			pCurrentRun=pImple_->protobufRuns.back().get();
		}

		const l1menu::L1TriggerDPGEvent& event=originalSample.getEvent( eventNumber );
		l1menuprotobuf::Event* pProtobufEvent=pCurrentRun->add_event();

		// Loop over all of the triggers
		for( size_t triggerNumber=0; triggerNumber<pImple_->triggerMenu.numberOfTriggers(); ++triggerNumber )
		{
			std::unique_ptr<l1menu::ITrigger> pTrigger=pImple_->triggerMenu.getTriggerCopy(triggerNumber);
			std::vector<std::string> thresholdNames=l1menu::tools::getThresholdNames(*pTrigger);

			try
			{
				l1menu::tools::setTriggerThresholdsAsTightAsPossible( event, *pTrigger, 0.001 );
				// Set all of the parameters to match the thresholds in the trigger
				for( const auto& thresholdName : thresholdNames )
				{
					pProtobufEvent->add_threshold( pTrigger->parameter(thresholdName) );
				}
			}
			catch( std::exception& error )
			{
				// setTriggerThresholdsAsTightAsPossible() couldn't find thresholds so record
				// -1 for everything.
				// Range based for loop gives me a warning because I don't use the thresholdName.
				for( size_t index=0; index<thresholdNames.size(); ++index ) pProtobufEvent->add_threshold(-1);
			} // end of try block that sets the trigger thresholds

		} // end of loop over triggers

		pImple_->sumOfWeights+=event.weight();
	} // end of loop over events
}

void l1menu::ReducedMenuSample::saveToFile( const std::string& filename ) const
{
	// Open the file. Parameters are filename, write ability and create, rw-r--r-- permissions.
	int fileDescriptor = open( filename.c_str(), O_WRONLY | O_CREAT, 0644 );
	if( fileDescriptor==0 ) throw std::runtime_error( "ReducedMenuSample save to file - couldn't open file" );
	::UnixFileSentry fileSentry( fileDescriptor ); // Use this as an exception safe way of closing the output file

	// Setup the protobuf file handlers
	google::protobuf::io::FileOutputStream fileOutput( fileDescriptor );

	// I want the magic number and file format identifier uncompressed, so
	// I'll write those before switching to using gzipped output.
	{ // Block to make sure codedOutput is destructed before the gzip version is created
		google::protobuf::io::CodedOutputStream codedOutput( &fileOutput );

		// Write a magic number at the start of all files
		codedOutput.WriteString( pImple_->FILE_FORMAT_MAGIC_NUMBER );
		// Write an integer that specifies what version of the file format I'm using. I
		// have no intention of changing the format but I might as well keep the option
		// open.
		codedOutput.WriteVarint32( 1 );
	}

	google::protobuf::io::GzipOutputStream gzipOutput( &fileOutput );
	google::protobuf::io::CodedOutputStream codedOutput( &gzipOutput );

	// Write the size of the header message into the file...
	codedOutput.WriteVarint64( pImple_->protobufSampleHeader.ByteSize() );
	// ...and then write the header
	pImple_->protobufSampleHeader.SerializeToCodedStream( &codedOutput );

	// Now go through each of the runs and do the same for those
	for( const auto& pRun : pImple_->protobufRuns )
	{
		codedOutput.WriteVarint64( pRun->ByteSize() );
		pRun->SerializeToCodedStream( &codedOutput );
	}

}

size_t l1menu::ReducedMenuSample::numberOfEvents() const
{
	size_t numberOfEvents=0;
	for( const auto& pRun : pImple_->protobufRuns ) numberOfEvents+=pRun->event_size();
	return numberOfEvents;
}

const l1menu::TriggerMenu& l1menu::ReducedMenuSample::getTriggerMenu() const
{
	return pImple_->triggerMenu;
}

bool l1menu::ReducedMenuSample::containsTrigger( const l1menu::ITrigger& trigger, bool allowOlderVersion ) const
{
	// Loop over all of the triggers in the menu, and see if there is one
	// where the name and version match.
	for( size_t triggerNumber=0; triggerNumber<pImple_->triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& triggerInMenu=pImple_->triggerMenu.getTrigger(triggerNumber);
		if( triggerInMenu.name()!=trigger.name() ) continue;
		if( allowOlderVersion )
		{
			if( triggerInMenu.version()>trigger.version() ) continue;
		}
		else
		{
			if( triggerInMenu.version()!=trigger.version() ) continue;
		}

		// If control got this far then there is a trigger with the required name
		// and sufficient version. I now need to check all of the non threshold parameters
		// to make sure they match, i.e. make sure the ReducedSample was made with the same
		// eta cuts or whatever.
		// I don't care if the thresholds don't match because that's what's stored in the
		// ReducedMenuSample.
		std::vector<std::string> parameterNames=l1menu::tools::getNonThresholdParameterNames( trigger );
		bool allParametersMatch=true;
		for( const auto& parameterName : parameterNames )
		{
			if( trigger.parameter(parameterName)!=triggerInMenu.parameter(parameterName) ) allParametersMatch=false;
		}

		if( allParametersMatch ) return true;
	} // end of loop over triggers

	// If control got this far then no trigger was found that matched
	return false;
}

const std::map<std::string,size_t> l1menu::ReducedMenuSample::getTriggerParameterIdentifiers( const l1menu::ITrigger& trigger, bool allowOlderVersion ) const
{
	std::map<std::string,size_t> returnValue;

	// Need to find out how many parameters there are for each event. Basically the sum
	// of the number of thresholds for all triggers.
	size_t parameterNumber=0;
	bool triggerWasFound=true;
	for( size_t triggerNumber=0; triggerNumber<pImple_->triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& triggerInMenu=pImple_->triggerMenu.getTrigger(triggerNumber);

		triggerWasFound=true; // Set to true, then back to false if any of the tests fail
		// See if this trigger in the menu is the same as the one passed as a parameter
		if( triggerInMenu.name()!=trigger.name() ) triggerWasFound=false;
		if( allowOlderVersion )
		{
			if( triggerInMenu.version()>trigger.version() ) triggerWasFound=false;
		}
		else
		{
			if( triggerInMenu.version()!=trigger.version() ) triggerWasFound=false;
		}

		// If control got this far then there is a trigger with the required name
		// and sufficient version. I now need to check all of the non threshold parameters
		// to make sure they match, i.e. make sure the ReducedSample was made with the same
		// eta cuts or whatever.
		// I don't care if the thresholds don't match because that's what's stored in the
		// ReducedMenuSample.
		if( triggerWasFound ) // Trigger can still fail, but no point doing this check if it already has
		{
			std::vector<std::string> parameterNames=l1menu::tools::getNonThresholdParameterNames( trigger );
			for( const auto& parameterName : parameterNames )
			{
				if( trigger.parameter(parameterName)!=triggerInMenu.parameter(parameterName) ) triggerWasFound=false;
			}
		}

		std::vector<std::string> thresholdNames=l1menu::tools::getThresholdNames(triggerInMenu);
		if( triggerWasFound )
		{
			for( const auto& thresholdName : thresholdNames )
			{
				returnValue[thresholdName]=parameterNumber;
				++parameterNumber;
			}
			break;
		}
		else parameterNumber+=thresholdNames.size();
	}

	// There could conceivably be a trigger that was found but has no thresholds
	// (I guess - it would be a pretty pointless trigger though). To indicate the
	// difference between that and a trigger that wasn't found I'll respectively
	// return the empty vector or throw an exception.
	if( !triggerWasFound ) throw std::runtime_error( "l1menu::ReducedMenuSample::getTriggerParameterIdentifiers() called for a trigger that was not used to create the sample - "+trigger.name() );

	return returnValue;
}

const l1menu::IEvent& l1menu::ReducedMenuSample::getEvent( size_t eventNumber ) const
{
	for( const auto& pRun : pImple_->protobufRuns )
	{
		if( eventNumber<static_cast<size_t>(pRun->event_size()) )
		{
			pImple_->event.pProtobufEvent_=pRun->mutable_event(eventNumber);
			return pImple_->event;
		}
		// Event must be in a later run, so reduce the number by how many events
		// were in this run and look again.
		eventNumber-=pRun->event_size();
	}

	// Should always find the event before getting to this point, so throw an
	// exception if this happens.
	throw std::runtime_error( "ReducedMenuSample::getEvent(eventNumber) was asked for an invalid eventNumber" );

}

std::unique_ptr<l1menu::ICachedTrigger> l1menu::ReducedMenuSample::createCachedTrigger( const l1menu::ITrigger& trigger ) const
{
	return std::unique_ptr<l1menu::ICachedTrigger>( new CachedTriggerImplementation(*this,trigger) );
}

float l1menu::ReducedMenuSample::eventRate() const
{
	return pImple_->eventRate;
}

void l1menu::ReducedMenuSample::setEventRate( float rate )
{
	pImple_->eventRate=rate;
}

float l1menu::ReducedMenuSample::sumOfWeights() const
{
	return pImple_->sumOfWeights;
}

std::unique_ptr<const l1menu::IMenuRate> l1menu::ReducedMenuSample::rate( const l1menu::TriggerMenu& menu ) const
{
	// TODO make sure the TriggerMenu is valid for this sample

	// I need a non-const menu, so make a copy
	l1menu::TriggerMenu mutableMenu( menu );

	// The number of events that pass each trigger
	std::vector<size_t> numberOfEventsPassed( menu.numberOfTriggers() );
	float numberOfEventsPassingAnyTrigger;

	for( size_t triggerNumber=0; triggerNumber<menu.numberOfTriggers(); ++triggerNumber )
	{
		mutableMenu.getTrigger( triggerNumber ).initiateForReducedSample( *this );
	}

	for( size_t eventNumber=0; eventNumber<numberOfEvents(); ++eventNumber )
	{
		const l1menu::IEvent& event=getEvent(eventNumber);
		bool anyTriggerPassed=false;

		for( size_t triggerNumber=0; triggerNumber<menu.numberOfTriggers(); ++triggerNumber )
		{
			if( event.passesTrigger( mutableMenu.getTrigger(triggerNumber) ) )
			{
				// If the event passes the trigger, increment the counter
				++numberOfEventsPassed[triggerNumber];
				anyTriggerPassed=true;
			}
		}

		if( anyTriggerPassed ) ++numberOfEventsPassingAnyTrigger;
	}

	::MenuRateImplementation* pRates=new ::MenuRateImplementation;
	// This is the value I want to return, but I still need access to the extended attributes of the subclass
	std::unique_ptr<const l1menu::IMenuRate> pReturnValue( pRates );

	pRates->setScaling( pImple_->eventRate );

	pRates->setTotalFraction( static_cast<float>(numberOfEventsPassingAnyTrigger)/static_cast<float>(numberOfEvents()) );

	for( size_t triggerNumber=0; triggerNumber<numberOfEventsPassed.size(); ++triggerNumber )
	{
		float fraction=static_cast<float>(numberOfEventsPassed[triggerNumber])/static_cast<float>(numberOfEvents());
		pRates->addTriggerRate( mutableMenu.getTrigger(triggerNumber), fraction );
	}

	return pReturnValue;
}
