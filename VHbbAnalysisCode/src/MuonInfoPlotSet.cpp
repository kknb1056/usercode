#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::MuonInfoPlotSet::MuonInfoPlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::MuonInfoPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MuonInfoPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//


	pGlobalChi2_=new TH1F( "globalChi2","Global chi2", 60, 0, 20 );
	pGlobalChi2_->SetDirectory(pDirectory);

	pNumberOfPixelHits_=new TH1F( "numberOfPixelHits", "Number of pixel hits", 11, -0.5, 10.5 );
	pNumberOfPixelHits_->SetDirectory(pDirectory);

	pNumberOfGlobalHits_=new TH1F( "numberOfGlobalHits", "Number of global hits", 31, -0.5, 30.5 );
	pNumberOfGlobalHits_->SetDirectory(pDirectory);

	pNumberOfHits_=new TH1F( "numberOfHits", "Number of hits", 31, -0.5, 30.5 );
	pNumberOfHits_->SetDirectory(pDirectory);

	pCategory_=new TH1F( "category", "Category", 60, 0, 20 );
	pCategory_->SetDirectory(pDirectory);

	pNumberOfMatches_=new TH1F( "numberOfMatches", "Number of matches", 16, -0.5, 15.5 );
	pNumberOfMatches_->SetDirectory(pDirectory);

	pIPDB_=new TH1F( "ipDb", "ipDb, whatever that means", 60, 0, 2 );
	pIPDB_->SetDirectory(pDirectory);

	pEta_=new TH1F( "eta", "Eta", 60, -3, 3 );
	pEta_->SetDirectory(pDirectory);

	pPt_=new TH1F( "pT", "pT", 60, 0, 250 );
	pPt_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::MuonInfoPlotSet::fill( const VHbbEvent::MuonInfo& muon )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MuonInfoPlotSet::book() - histograms have not been booked" );

	pGlobalChi2_->Fill( muon.globChi2 );
	pNumberOfPixelHits_->Fill( muon.nPixelHits );
	pNumberOfGlobalHits_->Fill( muon.globNHits );
	pNumberOfHits_->Fill( muon.nHits );
	pCategory_->Fill( muon.cat );
	pNumberOfMatches_->Fill( muon.nMatches );
	pIPDB_->Fill( muon.ipDb );
	pEta_->Fill( muon.p4.Eta() );
	pPt_->Fill( muon.p4.Pt() );
}

