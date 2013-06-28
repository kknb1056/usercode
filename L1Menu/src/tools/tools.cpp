#include "l1menu/tools/tools.h"

#include <sstream>
#include <exception>
#include <map>
#include <stdexcept>
#include <algorithm>
#include "l1menu/ITrigger.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include "l1menu/TriggerTable.h"

std::vector<std::string> l1menu::tools::getThresholdNames( const l1menu::ITrigger& trigger )
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

std::vector<std::string> l1menu::tools::getNonThresholdParameterNames( const l1menu::ITrigger& trigger )
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

void l1menu::tools::setTriggerThresholdsAsTightAsPossible( const l1menu::L1TriggerDPGEvent& event, l1menu::ITrigger& trigger, float tolerance )
{
	std::vector<std::string> thresholdNames=l1menu::tools::getThresholdNames( trigger );
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
			highThreshold*=5; // Make sure the high threshold is very high, to catch all tails
		}
		catch( std::exception& error ) { /* No indication set. Do nothing and just use the defaults I set previously. */ }

		threshold=lowThreshold;
		bool lowTest=trigger.apply( event );

		threshold=highThreshold;
		bool highTest=trigger.apply( event );

		if( lowTest==highTest ) std::runtime_error( "l1menu::tools::setTriggerThresholdsAsTightAsPossible() - couldn't find a set of thresholds to pass the given event.");

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

std::pair<float,float> l1menu::tools::calorimeterRegionEtaBounds( size_t calorimeterRegion )
{
	if( calorimeterRegion==0 ) return std::make_pair( -5.0, -4.5 );
	else if( calorimeterRegion==1 ) return std::make_pair( -4.5, -4.0 );
	else if( calorimeterRegion==2 ) return std::make_pair( -4.0, -3.5 );
	else if( calorimeterRegion==3 ) return std::make_pair( -3.5, -3.0 );
	else if( calorimeterRegion==4 ) return std::make_pair( -3.0, -2.172 );
	else if( calorimeterRegion==5 ) return std::make_pair( -2.172, -1.74 );
	else if( calorimeterRegion==6 ) return std::make_pair( -1.74, -1.392 );
	else if( calorimeterRegion==7 ) return std::make_pair( -1.392, -1.044 );
	else if( calorimeterRegion==8 ) return std::make_pair( -1.044, -0.696 );
	else if( calorimeterRegion==9 ) return std::make_pair( -0.696, -0.348 );
	else if( calorimeterRegion==10 ) return std::make_pair( -0.348, 0 );
	else if( calorimeterRegion==11 ) return std::make_pair( 0, 0.348 );
	else if( calorimeterRegion==12 ) return std::make_pair( 0.348, 0.696 );
	else if( calorimeterRegion==13 ) return std::make_pair( 0.696, 1.044 );
	else if( calorimeterRegion==14 ) return std::make_pair( 1.044, 1.392 );
	else if( calorimeterRegion==15 ) return std::make_pair( 1.392, 1.74 );
	else if( calorimeterRegion==16 ) return std::make_pair( 1.74, 2.172 );
	else if( calorimeterRegion==17 ) return std::make_pair( 2.172, 3.0 );
	else if( calorimeterRegion==18 ) return std::make_pair( 3.0, 3.5 );
	else if( calorimeterRegion==19 ) return std::make_pair( 3.5, 4.0 );
	else if( calorimeterRegion==20 ) return std::make_pair( 4.0, 4.5 );
	else if( calorimeterRegion==21 ) return std::make_pair( 4.5, 5.0 );
	else throw std::runtime_error( "l1menu::tools::calorimeterRegionEtaBounds was given an invalid calorimeter region" );
}

float l1menu::tools::convertEtaCutToRegionCut( float etaCut )
{
	if( etaCut<calorimeterRegionEtaBounds(0).first || etaCut>calorimeterRegionEtaBounds(21).second )
			throw std::runtime_error( "l1menu::tools::convertEtaCutToRegionCut was given an eta value outside the allowed region" );

	size_t caloRegion;
	for( caloRegion=0; caloRegion<=21; ++caloRegion )
	{
		std::pair<float,float> regionBounds=calorimeterRegionEtaBounds( caloRegion );
		if( regionBounds.first<=etaCut && etaCut<regionBounds.second ) break;
	}

	// I should have an answer for which calorimeter region the etaCut corresponds to.
	// I want to format the result to make sure it's in one half of the symmetric detector.
	if( caloRegion>10 ) caloRegion=21-caloRegion;
	return caloRegion;
}

float l1menu::tools::convertRegionCutToEtaCut( float regionCut )
{
	if( regionCut<0 || regionCut>21 ) throw std::runtime_error( "l1menu::tools::convertRegionCutToEtaCut was given an invalid calorimeter region" );

	// Result will be absolute eta. Use the symmetry to make the comparison easier.
	if( regionCut>10 ) regionCut=21-regionCut;

	// The float will be truncated to an integer.
	std::pair<float,float> regionBounds=calorimeterRegionEtaBounds( regionCut );

	return std::fabs(regionBounds.second);
}

