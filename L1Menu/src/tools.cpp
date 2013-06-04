#include "l1menu/tools.h"

#include <sstream>
#include <exception>
#include <map>
#include <stdexcept>
#include <algorithm>
#include "l1menu/ITrigger.h"
#include "l1menu/IEvent.h"
#include "l1menu/TriggerTable.h"

std::vector<std::string> l1menu::getThresholdNames( const l1menu::ITrigger& trigger )
{
	std::vector<std::string> returnValue;

	//
	// I don't know how many thresholds there are, so I'll try and get every possible one and catch
	// the exception when I eventually hit a threshold that doesn't exist.
	//

	std::stringstream stringConverter;
	// I plan in the future to implement cross triggers with "leg1threshold1", "leg2threshold1" etcetera,
	// so I'll loop over those possible prefixes.
	for( size_t legNumber=0; true; ++legNumber ) // Loop continuously until the exception handler calls "break"
	{
		size_t thresholdNumber;
		try
		{
			// Loop over all possible numbers of thresholds
			for( thresholdNumber=1; true; ++thresholdNumber ) // Loop continuously until I hit an exception
			{
				stringConverter.str("");
				if( legNumber!=0 ) stringConverter << "leg" << legNumber; // For triggers with only one leg I don't want to prefix anything.

				stringConverter << "threshold" << thresholdNumber;

				trigger.parameter(stringConverter.str());
				// If the threshold doesn't exist the statement above will throw an exception, so
				// I've reached this far then the threshold name must exist.
				returnValue.push_back( stringConverter.str() );
			}

		}
		catch( std::exception& error )
		{
			// If this exception is from the first threshold tried then the prefix (e.g. "leg1") does not
			// exist, so I know I've finished. If it isn't from the first threshold then there could be
			// other prefixes (e.g. "leg2") that have thresholds that can be modified, in which case I
			// need to continue.
			if( thresholdNumber==1 && legNumber!=0 ) break;
		}
	}

	return returnValue;
}

std::vector<std::string> l1menu::getNonThresholdParameterNames( const l1menu::ITrigger& trigger )
{
	std::vector<std::string> returnValue;

	// It'll be easier to get the threshold names and then copy
	// everything that's not in there to the return value.
	std::vector<std::string> allParameterNames=trigger.parameterNames();
	std::vector<std::string> thresholdNames=getThresholdNames(trigger);

	for( const auto& parameterName : allParameterNames )
	{
		const auto& iFindResult=std::find( thresholdNames.begin(), thresholdNames.end(), parameterName );
		// If the current parameter name isn't one of the thresholds add
		// it the vector which I'm going to return.
		if( iFindResult==thresholdNames.end() ) returnValue.push_back(parameterName);
	}

	return returnValue;
}

void l1menu::setTriggerThresholdsAsTightAsPossible( const l1menu::IEvent& event, l1menu::ITrigger& trigger, float tolerance )
{
	std::vector<std::string> thresholdNames=l1menu::getThresholdNames( trigger );
	std::map<std::string,float> tightestPossibleThresholds;

	// First set all of the thresholds to zero
	for( const auto& thresholdName : thresholdNames ) trigger.parameter(thresholdName)=0;

	// Now run through each threshold at a time and figure out how low it can be and still
	// pass the event.
	for( const auto& thresholdName : thresholdNames )
	{
		// Note that this is a reference, so when this is changed the trigger is modified
		float& threshold=trigger.parameter(thresholdName);

		float lowThreshold=0;
		float highThreshold=500;
		// See if an indication of the range of the trigger has been set
		try // These calls will throw an exception if no suggestion has been set
		{
			lowThreshold=l1menu::TriggerTable::instance().getSuggestedLowerEdge( trigger.name(), thresholdName );
			highThreshold=l1menu::TriggerTable::instance().getSuggestedUpperEdge( trigger.name(), thresholdName );
		}
		catch( std::exception& error ) { /* No indication set. Do nothing and just use the defaults I set previously. */ }

		threshold=lowThreshold;
		bool lowTest=trigger.apply( event );

		threshold=highThreshold;
		bool highTest=trigger.apply( event );

		if( lowTest==highTest ) std::runtime_error( "l1menu::setTriggerThresholdsAsTightAsPossible() - couldn't find a set of thresholds to pass the given event.");

		// Try and find the turn on point by bisection
		while( highThreshold-lowThreshold > tolerance )
		{
			threshold=(highThreshold+lowThreshold)/2;
			bool midTest=trigger.apply(event);

			if( lowTest==midTest && highTest!=midTest ) lowThreshold=threshold;
			else if( highTest==midTest ) highThreshold=threshold;
			else throw std::runtime_error( std::string("Something fucked up while testing ")+trigger.name() );
		}

		// Record what this value was for the parameter named
		tightestPossibleThresholds[thresholdName]=highThreshold;
		// Then set back to zero ready to test the other thresholds
		threshold=0;
	}

	//
	// Now that I've figured out what all of the thresholds need to be, run
	// through and set the trigger up with these thresholds.
	//
	for( const auto& parameterValuePair : tightestPossibleThresholds )
	{
		trigger.parameter(parameterValuePair.first)=parameterValuePair.second;
	}
}
