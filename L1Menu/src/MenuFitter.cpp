#include "l1menu/MenuFitter.h"

#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cmath>
#include "l1menu/ICachedTrigger.h"
#include "l1menu/ISample.h"
#include "l1menu/IEvent.h"
#include "l1menu/IMenuRate.h"
#include "l1menu/ITriggerRate.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/ITrigger.h"
#include "l1menu/TriggerRatePlot.h"
#include "l1menu/tools/tools.h"
#include "l1menu/tools/stringManipulation.h"

namespace // Use the unnamed namespace for things only used here
{
	/** @brief Structure to collate a few things about triggers that can be scaled
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/Jul/2013
	 */
	struct TriggerScalingDetails
	{
		size_t triggerNumber; ///< The number of the trigger in the trigger table
		float bandwidthFraction; ///< The fraction of the total bandwidth requested for this trigger
		l1menu::TriggerRatePlot ratePlot; ///< The rate plot for this trigger
		std::string mainThreshold;
		std::vector< std::pair<std::string,float> > thresholdScalings; ///< The constant to scale each threshold compared to the main threshold
	};
} // end of the unnamed namespace


//
// Need to declare the pimple. So far it's just been forward declared.
//
namespace l1menu
{
	/** @brief Private members for the MenuFitter wrapped up in a compiler firewall.
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/Jul/2013
	 */
	class MenuFitterPrivateMembers
	{
	public:
		MenuFitterPrivateMembers( const l1menu::ISample& newSample, const l1menu::MenuRatePlots* pRatePlots ) : sample(newSample), pMenuRatePlots(pRatePlots) {}
		const l1menu::ISample& sample;
		const l1menu::MenuRatePlots* pMenuRatePlots;
		l1menu::TriggerMenu menu;
		std::vector< ::TriggerScalingDetails > scalableTriggers;
		std::vector<std::pair<size_t,float> > bandwidthFractions;
		std::vector<l1menu::TriggerRatePlot> ratePlots;
		void initiateOtherTriggerInfo( size_t triggerNumber, bool lockThresholds, float fractionOfTotalBandwidth );
	};

}

l1menu::MenuFitter::MenuFitter( const l1menu::ISample& sample )
	: pImple_( new MenuFitterPrivateMembers(sample,NULL) )
{

}

l1menu::MenuFitter::MenuFitter( const l1menu::ISample& sample, const l1menu::MenuRatePlots& menuRatePlots )
	: pImple_( new MenuFitterPrivateMembers(sample,&menuRatePlots) )
{

}

l1menu::MenuFitter::~MenuFitter()
{
	// No operation
}

const l1menu::TriggerMenu& l1menu::MenuFitter::menu() const
{
	return pImple_->menu;
}

std::unique_ptr<const l1menu::IMenuRate> l1menu::MenuFitter::fit( float totalRate, float tolerance )
{
	//
	// First set all the thresholds to be the ratio of the total bandwidth that
	// the user specified. With correlations this will probably mean the total
	// will come out less than the requested rate, but it's a good first step.
	// Loop over all of the triggers whose thresholds are not fixed.
	//
	for( const auto& triggerScalingDetails : pImple_->scalableTriggers )
	{
		l1menu::ITrigger& trigger=pImple_->menu.getTrigger( triggerScalingDetails.triggerNumber );

		std::cout << "Trying to find a threshold for " << trigger.name() << " to try and get a rate of " << totalRate*triggerScalingDetails.bandwidthFraction << std::endl;

		float& mainThreshold=trigger.parameter( triggerScalingDetails.mainThreshold );
		// Figure out what threshold should give the target rate for this particular trigger.
		// Note this is a reference so this command changes the trigger.
		mainThreshold=triggerScalingDetails.ratePlot.findThreshold( totalRate*triggerScalingDetails.bandwidthFraction );
		// Then scale all of the others off this
		for( const auto& nameScalePair : triggerScalingDetails.thresholdScalings )
		{
			trigger.parameter( nameScalePair.first )=mainThreshold*nameScalePair.second;
		}

		std::cout << "Initially setting threshold for " << trigger.name() << " to " << mainThreshold << " to try and get a rate of " << totalRate*triggerScalingDetails.bandwidthFraction << std::endl;
	}

	// Then work out what the total rate is
	std::unique_ptr<const l1menu::IMenuRate> pMenuRate=pImple_->sample.rate( pImple_->menu );

	size_t iterationNumber=0;
	while ( std::fabs(pMenuRate->totalRate()-totalRate)>tolerance )
	{
		if( iterationNumber>10 ) throw std::runtime_error( "Too many iterations" );
		++iterationNumber;

		float scaleAllRatesBy=totalRate/pMenuRate->totalRate();
		std::cout << "\n" << "New loop. Last iteration had a rate of " << pMenuRate->totalRate() << ". Scaling all rates by " << scaleAllRatesBy << " to try and get " << totalRate << std::endl;

		for( const auto& triggerScalingDetails : pImple_->scalableTriggers )
		{
			const size_t& triggerNumber=triggerScalingDetails.triggerNumber;
			l1menu::ITrigger& trigger=pImple_->menu.getTrigger( triggerNumber );
			const l1menu::ITriggerRate* pTriggerRate=pMenuRate->triggerRates()[triggerNumber];

			float& mainThreshold=trigger.parameter( triggerScalingDetails.mainThreshold );
			// Figure out what threshold should give the target rate for this particular trigger.
			// Note this is a reference so this command changes the trigger.
			mainThreshold=triggerScalingDetails.ratePlot.findThreshold( pTriggerRate->rate()*scaleAllRatesBy );
			// Then scale all of the others off this
			for( const auto& nameScalePair : triggerScalingDetails.thresholdScalings )
			{
				trigger.parameter( nameScalePair.first )=mainThreshold*nameScalePair.second;
			}
			std::cout << "Changing threshold for " << trigger.name() << " to " << mainThreshold << " to try and change the rate from " << pTriggerRate->rate() << " to " << pTriggerRate->rate()*scaleAllRatesBy << std::endl;

		} // end of loop over triggers I'm allowed to change thresholds for

		pMenuRate=pImple_->sample.rate( pImple_->menu );
	}

	return pMenuRate;
}

void l1menu::MenuFitter::addTrigger( const l1menu::ITrigger& trigger, float fractionOfTotalBandwidth, bool lockThresholds )
{
	if( !lockThresholds && (fractionOfTotalBandwidth<0 || fractionOfTotalBandwidth>1) ) throw std::runtime_error("Invalid fraction of the total bandwidth requested");

	size_t triggerNumber=pImple_->menu.numberOfTriggers(); // This will be the number of the next trigger added
	pImple_->menu.addTrigger( trigger );
	pImple_->initiateOtherTriggerInfo( triggerNumber, lockThresholds, fractionOfTotalBandwidth );
}

void l1menu::MenuFitter::loadMenuFromFile( const std::string& filename )
{
	std::ifstream file( filename.c_str() );
	if( !file.is_open() ) throw std::runtime_error( "Unable to open file "+filename );

	const size_t bufferSize=200;
	char buffer[bufferSize];

	// The old file format provides the rates it wants for each trigger
	// as an absolute value, whereas I want it as a fraction of the total.
	// So I need to run through the whole file and add up each of these
	// to get a total, then divide each value by the total to get the fraction.
	// Hence I can't do that until I've loaded every trigger. I'll keep track
	// of whether each threshold is locked and the absolute rate requested in
	// this vector.
	std::vector< std::pair<bool,float> > triggerAuxiliaryInfo;
	float totalRequestedRate=0; // This is the sum of all the rates requested for each trigger

	while( file.good() )
	{
		try
		{
			// Get one line at a time
			file.getline( buffer, bufferSize );

			// split the line by whitespace into columns
			std::vector<std::string> tableColumns=l1menu::tools::splitByWhitespace( buffer );

			if( tableColumns.size()==1 && tableColumns[0].empty() ) continue; // Allow blank lines without giving a warning
			if( tableColumns.size()!=12 ) throw std::runtime_error( "The line does not have the correct number of columns" );

			if( pImple_->menu.addTriggerFromOldFormat( tableColumns ) )
			{
				// If the trigger was created successfully, keep a note of these
				// details so that I can later initiate the auxiliary information.
				// I can't do that until I know the total requestedRate.
				bool lockThresholds=l1menu::tools::convertStringToFloat( tableColumns[11] );
				float requestedRate=l1menu::tools::convertStringToFloat( tableColumns[9] );
				triggerAuxiliaryInfo.push_back( std::make_pair(lockThresholds,requestedRate) );
				totalRequestedRate+=requestedRate;
			}

		} // end of try block
		catch( std::runtime_error& exception )
		{
			std::cout << "Some error occured while processing the line \"" << buffer << "\":" << exception.what() << std::endl;
		}
	}

	// Now I've processed all the lines I should know the total requested rate and I can
	// initiate the auxiliary information.
	size_t triggerNumber=pImple_->menu.numberOfTriggers()-triggerAuxiliaryInfo.size(); // There may have been triggers in there previously
	for( const auto auxiliaryInfo : triggerAuxiliaryInfo )
	{
		pImple_->initiateOtherTriggerInfo( triggerNumber, auxiliaryInfo.first, auxiliaryInfo.second/totalRequestedRate );
		++triggerNumber;
	}
}

void l1menu::MenuFitterPrivateMembers::initiateOtherTriggerInfo( size_t triggerNumber, bool lockThresholds, float fractionOfTotalBandwidth )
{
	const l1menu::ITrigger& newTrigger=menu.getTrigger( triggerNumber );

	if( !lockThresholds )
	{

		//
		// Create a rate plot for this trigger
		//
		const std::vector<std::string> thresholdNames=l1menu::tools::getThresholdNames(newTrigger);
		const std::string& mainThreshold=thresholdNames.front();
		unsigned int numberOfBins=100;
		float lowerEdge=0;
		float upperEdge=100;
		try
		{
			l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
			numberOfBins=triggerTable.getSuggestedNumberOfBins( newTrigger.name(), mainThreshold );
			lowerEdge=triggerTable.getSuggestedLowerEdge( newTrigger.name(), mainThreshold );
			upperEdge=triggerTable.getSuggestedUpperEdge( newTrigger.name(), mainThreshold );
		}
		catch( std::exception& error) { /* Do nothing. If no binning suggestions have been set for this trigger use the defaults I set above. */ }

		l1menu::TriggerRatePlot ratePlot(newTrigger,newTrigger.name()+"_v_allThresholdsScaled",numberOfBins,lowerEdge,upperEdge,mainThreshold,thresholdNames);
		ratePlot.addSample( sample );

		// Record the scaling between the main threshold and all of the others, so that
		// when they get increased/decreased it's all done proportionally.
		std::vector< std::pair<std::string,float> > thresholdScalings;
		const float mainThresholdValue=newTrigger.parameter(mainThreshold);
		for( const auto& thresholdName : thresholdNames )
		{
			if( thresholdName==mainThreshold ) continue;
			thresholdScalings.push_back( std::make_pair( thresholdName, newTrigger.parameter(thresholdName)/mainThresholdValue ) );
		}

		//
		// Bundle all of this information in the helper structure I wrote in
		// the unnamed namespace.
		//
		scalableTriggers.push_back( ::TriggerScalingDetails{triggerNumber,fractionOfTotalBandwidth,std::move(ratePlot),mainThreshold,std::move(thresholdScalings)} );
	}

}
