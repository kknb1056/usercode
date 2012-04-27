#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoCollectionPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::MuonInfoCollectionPlotSet::MuonInfoCollectionPlotSet( bool createIsolationTree )
	: histogramHaveBeenBooked_(false), muonInfoPlotSet_(createIsolationTree)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::MuonInfoCollectionPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MuonInfoCollectionPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	pNumberOfMuons_=new TH1F( "numberOfMuons", "Number of muons in each collection", 21, -0.5, 20.5 );
	pNumberOfMuons_->SetDirectory( pDirectory );

	muonInfoPlotSet_.book( pDirectory );

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::MuonInfoCollectionPlotSet::fill( const std::vector<VHbbEvent::MuonInfo>& muonCollection, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MuonInfoCollectionPlotSet::book() - histograms have not been booked" );

	pNumberOfMuons_->Fill( muonCollection.size() );

	for( std::vector<VHbbEvent::MuonInfo>::const_iterator iMuon=muonCollection.begin(); iMuon!=muonCollection.end(); ++iMuon )
	{
		muonInfoPlotSet_.fill( *iMuon, pAuxInfo );
	}

}

