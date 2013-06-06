#include "l1menu/ReducedMenuSample.h"

#include <vector>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include "l1menu/IReducedEvent.h"
#include "l1menu/MenuSample.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/ITrigger.h"
#include "l1menu/tools.h"
#include "protobuf/l1menu.pb.h"

namespace // unnamed namespace
{
	class ReducedEventImplementation : public l1menu::IReducedEvent
	{
	public:
		virtual ~ReducedEventImplementation() {}
		virtual float parameterValue( size_t parameterNumber ) const {  return pProtobufEvent->threshold(parameterNumber); }
		virtual float weight() const { if( pProtobufEvent->has_weight() ) return pProtobufEvent->weight(); else return 1; }
		void setWeight( float newWeight ) { pProtobufEvent->set_weight(newWeight); }
		l1menuprotobuf::Event* pProtobufEvent;
	};
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
		ReducedMenuSamplePrivateMembers( const l1menu::TriggerMenu& newTriggerMenu );
		ReducedMenuSamplePrivateMembers( const std::string& filename );
		void copyMenuToProtobufSample();
		::ReducedEventImplementation event;
		const l1menu::TriggerMenu& triggerMenu;
		l1menuprotobuf::Sample protobufSample;
	};
}

l1menu::ReducedMenuSamplePrivateMembers::ReducedMenuSamplePrivateMembers( const l1menu::TriggerMenu& newTriggerMenu )
	: mutableTriggerMenu_( newTriggerMenu ), triggerMenu( mutableTriggerMenu_ )
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// I need to copy the details of the trigger menu into the protobuf storage.
	// This means I'm holding a duplicate, but I need it to write the Sample message
	// so I might as well do it now.
	for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& trigger=triggerMenu.getTrigger(triggerNumber);

		l1menuprotobuf::Trigger* pProtobufTrigger=protobufSample.add_trigger();
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
		const auto thresholdNames=l1menu::getThresholdNames(trigger);
		for( const auto& thresholdName : thresholdNames ) pProtobufTrigger->add_threshold(thresholdName);

	} // end of loop over triggers

}

l1menu::ReducedMenuSamplePrivateMembers::ReducedMenuSamplePrivateMembers( const std::string& filename )
	: triggerMenu(mutableTriggerMenu_)
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	{ // new block to limit scope of variables, so that the file is closed immediately after reading
		std::ifstream inputFile( filename );
		protobufSample.ParseFromIstream( &inputFile );
	}


	// I have all of the information in the protobuf Sample, but I also need the trigger information
	// in the form of l1menu::TriggerMenu. Copy out the required information.
	for( int triggerNumber=0; triggerNumber<protobufSample.trigger_size(); ++triggerNumber )
	{
		const l1menuprotobuf::Trigger& inputTrigger=protobufSample.trigger(triggerNumber);

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
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( triggerMenu ) )
{
	addSample( originalSample );
}

l1menu::ReducedMenuSample::ReducedMenuSample( const l1menu::TriggerMenu& triggerMenu )
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( triggerMenu ) )
{
	// No operation besides the initialiser list
}

l1menu::ReducedMenuSample::~ReducedMenuSample()
{
	// No operation. Just need one defined otherwise the default one messes up
	// the unique_ptr deletion because ReducedMenuSamplePrivateMembers isn't
	// defined elsewhere.
}

void l1menu::ReducedMenuSample::addSample( const l1menu::MenuSample& originalSample )
{
	for( size_t eventNumber=0; eventNumber<originalSample.numberOfEvents(); ++eventNumber )
	{
		const l1menu::IEvent& event=originalSample.getEvent( eventNumber );
		l1menuprotobuf::Event* pProtobufEvent=pImple_->protobufSample.add_event();

		// Loop over all of the triggers
		for( size_t triggerNumber=0; triggerNumber<pImple_->triggerMenu.numberOfTriggers(); ++triggerNumber )
		{
			std::unique_ptr<l1menu::ITrigger> pTrigger=pImple_->triggerMenu.getTriggerCopy(triggerNumber);
			std::vector<std::string> thresholdNames=getThresholdNames(*pTrigger);

			try
			{
				setTriggerThresholdsAsTightAsPossible( event, *pTrigger, 0.001 );
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

	} // end of loop over events
}

l1menu::ReducedMenuSample::ReducedMenuSample( const std::string& filename )
	: pImple_( new l1menu::ReducedMenuSamplePrivateMembers( filename ) )
{
	// No operation except the initialiser list
}

void l1menu::ReducedMenuSample::saveToFile( const std::string& filename ) const
{
	std::ofstream outputFile( filename );
	pImple_->protobufSample.SerializeToOstream( &outputFile );
}

size_t l1menu::ReducedMenuSample::numberOfEvents() const
{
	return pImple_->protobufSample.event_size();
}

const l1menu::IReducedEvent& l1menu::ReducedMenuSample::getEvent( size_t eventNumber ) const
{
	pImple_->event.pProtobufEvent=pImple_->protobufSample.mutable_event(eventNumber);
	return pImple_->event;
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
		std::vector<std::string> parameterNames=getNonThresholdParameterNames( trigger );
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
			std::vector<std::string> parameterNames=getNonThresholdParameterNames( trigger );
			for( const auto& parameterName : parameterNames )
			{
				if( trigger.parameter(parameterName)!=triggerInMenu.parameter(parameterName) ) triggerWasFound=false;
			}
		}

		std::vector<std::string> thresholdNames=l1menu::getThresholdNames(triggerInMenu);
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
	if( !triggerWasFound ) throw std::runtime_error( "l1menu::ReducedMenuSample::getTriggerParameterIdentifiers() called for a trigger that was not used to create the sample" );

	return returnValue;
}
