#include "TrkUpgradeAnalysis/VHbb/interface/SimpleJetCollectionPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::SimpleJetCollectionPlotSet::SimpleJetCollectionPlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::SimpleJetCollectionPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::SimpleJetCollectionPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	pNumberOfJets_=new TH1F( "numberOfJets", "Number of jets in each collection", 101, -0.5, 100.5 );
	pNumberOfJets_->SetDirectory( pDirectory );

	simpleJetPlotSet_.book( pDirectory );

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::SimpleJetCollectionPlotSet::fill( const std::vector<VHbbEvent::SimpleJet>& jetCollection, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::SimpleJetCollectionPlotSet::book() - histograms have not been booked" );

	pNumberOfJets_->Fill( jetCollection.size() );

	for( std::vector<VHbbEvent::SimpleJet>::const_iterator iJet=jetCollection.begin(); iJet!=jetCollection.end(); ++iJet )
	{
		simpleJetPlotSet_.fill( *iJet, pAuxInfo );
	}

}

