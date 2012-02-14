#include "TrkUpgradeAnalysis/VHbb/interface/ElectronInfoCollectionPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::ElectronInfoCollectionPlotSet::ElectronInfoCollectionPlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::ElectronInfoCollectionPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::ElectronInfoCollectionPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	pNumberOfElectrons_=new TH1F( "numberOfElectrons", "Number of electrons in each collection", 21, -0.5, 20.5 );
	pNumberOfElectrons_->SetDirectory( pDirectory );

	electronInfoPlotSet_.book( pDirectory );

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::ElectronInfoCollectionPlotSet::fill( const std::vector<VHbbEvent::ElectronInfo>& electronCollection )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::ElectronInfoCollectionPlotSet::book() - histograms have not been booked" );

	pNumberOfElectrons_->Fill( electronCollection.size() );

	for( std::vector<VHbbEvent::ElectronInfo>::const_iterator iElectron=electronCollection.begin(); iElectron!=electronCollection.end(); ++iElectron )
	{
		electronInfoPlotSet_.fill( *iElectron );
	}

}

