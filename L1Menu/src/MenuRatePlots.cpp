#include "l1menu/MenuRatePlots.h"

#include <sstream>
#include <iostream>
#include "l1menu/ITrigger.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/TriggerRatePlot.h"
#include "l1menu/tools.h"
#include <TH1F.h>

l1menu::MenuRatePlots::MenuRatePlots( const l1menu::TriggerMenu& triggerMenu, TDirectory* pDirectory )
{

	// Loop over each of the triggers in the menu, book a histogram for it and then create
	// a l1menu::TriggerRate plot for it.
	for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		std::unique_ptr<l1menu::ITrigger> pTrigger=triggerMenu.getTriggerCopy(triggerNumber);
		// Figure out the parameter names of all the possible thresholds.
		const std::vector<std::string> thresholdNames= l1menu::getThresholdNames(*pTrigger);

		// When a threshold is tested, I want all the other thresholds to be zero. I'll run through
		// and zero all of them now.
		for( std::vector<std::string>::const_iterator iName=thresholdNames.begin(); iName!=thresholdNames.end(); ++iName )
		{
			pTrigger->parameter(*iName)=0;
		}


		// I want a plot for each of the thresholds, so I'll loop over the threshold names
		for( std::vector<std::string>::const_iterator iThresholdName=thresholdNames.begin(); iThresholdName!=thresholdNames.end(); ++iThresholdName )
		{
			std::cout << "** Adding plot for " << *iThresholdName << " in trigger " << pTrigger->name() << std::endl;

			unsigned int numberOfBins=100;
			float lowerEdge=0;
			float upperEdge=100;
			try
			{
				const l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				numberOfBins=triggerTable.getSuggestedNumberOfBins( pTrigger->name(), *iThresholdName );
				lowerEdge=triggerTable.getSuggestedLowerEdge( pTrigger->name(), *iThresholdName );
				upperEdge=triggerTable.getSuggestedUpperEdge( pTrigger->name(), *iThresholdName );
			}
			catch( std::exception& error) { /* Do nothing. If no binning suggestions have been set for this trigger use the defaults I set above. */ }

			std::unique_ptr<TH1> pHistogram( new TH1F( (pTrigger->name()+"_v_"+(*iThresholdName)).c_str(), "This title gets changed by TriggerRatePlot anyway", numberOfBins, lowerEdge, upperEdge ) );
			pHistogram->SetDirectory( pDirectory );
			triggerPlots_.push_back( std::move(l1menu::TriggerRatePlot(*pTrigger,std::move(pHistogram),*iThresholdName)) );
		}

	} // end of loop over the triggers in the menu
}

void l1menu::MenuRatePlots::addEvent( const l1menu::IEvent& event )
{
	// Loop over each of the TriggerRatePlots and add the event to each of them.
	for( std::vector<l1menu::TriggerRatePlot>::iterator iRatePlot=triggerPlots_.begin(); iRatePlot!=triggerPlots_.end(); ++iRatePlot )
	{
		iRatePlot->addEvent( event );
	}
}

void l1menu::MenuRatePlots::setDirectory( TDirectory* pDirectory )
{
	// Loop over each of the TriggerRatePlots and individually set the directory.
	for( std::vector<l1menu::TriggerRatePlot>::iterator iRatePlot=triggerPlots_.begin(); iRatePlot!=triggerPlots_.end(); ++iRatePlot )
	{
		iRatePlot->getPlot()->SetDirectory( pDirectory );
	}
}

void l1menu::MenuRatePlots::relinquishOwnershipOfPlots()
{
	// Loop over each of the TriggerRatePlots and individually release them.
	for( std::vector<l1menu::TriggerRatePlot>::iterator iRatePlot=triggerPlots_.begin(); iRatePlot!=triggerPlots_.end(); ++iRatePlot )
	{
		iRatePlot->relinquishOwnershipOfPlot();
	}
}
