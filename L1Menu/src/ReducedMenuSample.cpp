#include "l1menu/ReducedMenuSample.h"

#include <vector>
#include <stdexcept>
#include <fstream>
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
		virtual float parameterValue( size_t parameterNumber ) const {  return pThresholdValues->at(parameterNumber); }
		virtual float weight() const { return *pWeight; }
		void setWeight( float newWeight ) { *pWeight=newWeight; }
		std::vector<float>* pThresholdValues;
		float* pWeight;
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
	public:
		ReducedMenuSamplePrivateMembers( size_t newNumberOfEvents, size_t numberOfParameters, const l1menu::TriggerMenu newTriggerMenu )
			: thresholdsForAllEvents( newNumberOfEvents, std::vector<float>(numberOfParameters) ),
			  weights( newNumberOfEvents ), numberOfEvents( newNumberOfEvents ), triggerMenu( newTriggerMenu )
		{
			// Run through every event and default every weight to 1.
			for( auto& eventWeight : weights )
			{
				eventWeight=1;
			}
		}
		std::vector< std::vector<float> > thresholdsForAllEvents;
		std::vector<float> weights;
		::ReducedEventImplementation event;
		size_t numberOfEvents;
		const l1menu::TriggerMenu triggerMenu;
	};
}

#include <iostream>
l1menu::ReducedMenuSample::ReducedMenuSample( const l1menu::MenuSample& originalSample, const l1menu::TriggerMenu& triggerMenu )
{
	size_t numberOfEvents=originalSample.numberOfEvents();
	// Need to find out how many parameters there are for each event. Basically the sum
	// of the number of thresholds for all triggers.
	size_t numberOfParameters=0;
	for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& trigger=triggerMenu.getTrigger(triggerNumber);
		numberOfParameters+=l1menu::getThresholdNames(trigger).size();
	}

	// Now I know how many events there are and how many parameters, I can create the pimple
	// with the correct parameters.
	pImple_.reset( new l1menu::ReducedMenuSamplePrivateMembers( numberOfEvents, numberOfParameters, triggerMenu ) );

	//
	// Now I've set the storage to the correct size, run through each event
	// and fill with the correct values.
	//
	for( size_t eventNumber=0; eventNumber<numberOfEvents; ++eventNumber )
	{
		const l1menu::IEvent& event=originalSample.getEvent( eventNumber );
		std::vector<float>& parameters=pImple_->thresholdsForAllEvents[eventNumber];

		size_t parameterNumber=0;
		// Loop over all of the triggers
		for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
		{
			std::unique_ptr<l1menu::ITrigger> pTrigger=triggerMenu.getTriggerCopy(triggerNumber);
			std::vector<std::string> thresholdNames=getThresholdNames(*pTrigger);

			try
			{
				setTriggerThresholdsAsTightAsPossible( event, *pTrigger, 0.001 );
				// Set all of the parameters to match the thresholds in the trigger
				for( const auto& thresholdName : thresholdNames )
				{
					parameters[parameterNumber]=pTrigger->parameter(thresholdName);
					++parameterNumber;
				}
			}
			catch( std::exception& error )
			{
				// setTriggerThresholdsAsTightAsPossible() couldn't find thresholds so record
				// -1 for everything.
				for( size_t index=0; index<thresholdNames.size(); ++index )
				{
					parameters[parameterNumber]=-1;
					++parameterNumber;
				}
			} // end of try block that sets the trigger thresholds

		} // end of loop over triggers
	} // end of loop over events
}

l1menu::ReducedMenuSample::~ReducedMenuSample()
{
	// No operation. Just need one defined otherwise the default one messes up
	// the unique_ptr deletion because ReducedMenuSamplePrivateMembers isn't
	// defined elsewhere.
}

l1menu::ReducedMenuSample::ReducedMenuSample( const std::string& filename )
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	l1menuprotobuf::Sample inputSample;
	{ // new block to limit scope of variables
		std::ifstream inputFile( filename );
		inputSample.ParseFromIstream( &inputFile );
	}

	l1menu::TriggerMenu triggerMenu;

	size_t totalNumberOfThresholds=0;

	for( int triggerNumber=0; triggerNumber<inputSample.trigger_size(); ++triggerNumber )
	{
		const l1menuprotobuf::Trigger& inputTrigger=inputSample.trigger(triggerNumber);

		triggerMenu.addTrigger( inputTrigger.name(), inputTrigger.version() );
		// Get a reference to the trigger I just created
		l1menu::ITrigger& trigger=triggerMenu.getTrigger(triggerNumber);

		// Run through all of the parameters and set them to what they were
		// when the sample was made.
		for( int parameterNumber=0; parameterNumber<inputTrigger.parameter_size(); ++parameterNumber )
		{
			const auto& inputParameter=inputTrigger.parameter(parameterNumber);
			trigger.parameter(inputParameter.name())=inputParameter.value();
		}

		// I should probably check the threshold names exist. At the moment I just see how
		// many there are so that I can initialise the buffer that holds the event data.
		totalNumberOfThresholds+=inputTrigger.threshold_size();
	}

	// Now I have the menu set up as much as I need it (thresholds aren't set
	// but I don't care about those). I can initialise the ReducedMenuSamplePrivateMembers
	// pImple_ with the menu and the correct sizes for the data buffer.
	pImple_.reset( new l1menu::ReducedMenuSamplePrivateMembers( inputSample.event_size(), totalNumberOfThresholds, triggerMenu ) );

	// Now run over each event from the input file
	for( int eventNumber=0; eventNumber<inputSample.event_size(); ++eventNumber )
	{
		auto& thresholdsForEvent=pImple_->thresholdsForAllEvents[eventNumber];
		const auto& inputEvent=inputSample.event(eventNumber);

		for( int thresholdNumber=0; thresholdNumber<inputEvent.threshold_size(); ++thresholdNumber )
		{
			thresholdsForEvent[thresholdNumber]=inputEvent.threshold(thresholdNumber);
		}

		// See if the weight was stored and set it if it has
		if( inputEvent.has_weight() ) pImple_->weights[eventNumber]=inputEvent.weight();
	}
}

void l1menu::ReducedMenuSample::saveToFile( const std::string& filename ) const
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	l1menuprotobuf::Sample outputSample;

	for( size_t triggerNumber=0; triggerNumber<pImple_->triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& trigger=pImple_->triggerMenu.getTrigger(triggerNumber);

		l1menuprotobuf::Trigger* pOutputTrigger=outputSample.add_trigger();
		pOutputTrigger->set_name( trigger.name() );
		pOutputTrigger->set_version( trigger.version() );

		// Record all of the parameters. It's not strictly necessary to record the values
		// of the parameters that are recorded for each event, but I might as well so that
		// the trigger menu is loaded exactly as it was saved.
		const auto parameterNames=trigger.parameterNames();
		for( const auto& parameterName : parameterNames )
		{
			l1menuprotobuf::Trigger_TriggerParameter* pOutputParameter=pOutputTrigger->add_parameter();
			pOutputParameter->set_name(parameterName);
			pOutputParameter->set_value( trigger.parameter(parameterName) );
		}

		// Make a note of the names of the parameters that are recorded for each event.
		const auto thresholdNames=l1menu::getThresholdNames(trigger);
		for( const auto& thresholdName : thresholdNames ) pOutputTrigger->add_threshold(thresholdName);

	} // end of loop over triggers

	//
	// I've done everything for what is effectively the header. I now need
	// to run through each event and record the thresholds for each one.
	//
	for( const auto& thresholdsForEvent : pImple_->thresholdsForAllEvents )
	{
		l1menuprotobuf::Event* pOutputEvent=outputSample.add_event();
		for( const auto& threshold : thresholdsForEvent )
		{
			pOutputEvent->add_threshold( threshold );
		}
	}

	// I should have the correct number of events in the protobuf version of
	// the sample now. I'll run through and set the event weights as well, but
	// I'll only add it if it's different to 1. I think this saves space in
	// the file.
	for( int eventNumber=0; eventNumber<outputSample.event_size(); ++eventNumber )
	{
		if( pImple_->weights[eventNumber]!=1 )
		{
			l1menuprotobuf::Event* pOutputEvent=outputSample.mutable_event(eventNumber);
			pOutputEvent->set_weight( pImple_->weights[eventNumber] );
		}
	}


	//
	// Everything should be set up in the class now, so I can open the
	// output file and write to it.
	//
	std::ofstream outputFile( filename );
	outputSample.SerializeToOstream( &outputFile );
}

size_t l1menu::ReducedMenuSample::numberOfEvents() const
{
	return pImple_->numberOfEvents;
}

const l1menu::IReducedEvent& l1menu::ReducedMenuSample::getEvent( size_t eventNumber ) const
{
	pImple_->event.pThresholdValues=&pImple_->thresholdsForAllEvents[eventNumber];
	pImple_->event.pWeight=&pImple_->weights[eventNumber];
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
