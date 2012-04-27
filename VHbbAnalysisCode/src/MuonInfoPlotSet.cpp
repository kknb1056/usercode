#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>
#include <TTree.h>

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"

trkupgradeanalysis::MuonInfoPlotSet::MuonInfoPlotSet( bool createIsolationTree )
	: histogramHaveBeenBooked_(false), createIsolationTree_(createIsolationTree), pIsolationTree_(NULL)
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

	pChargedIsolation_=new TH1F( "chargedIsolation", "chargedIsolation", 60, 0, 30 );
	pChargedIsolation_->SetDirectory(pDirectory);

	pPhotonIsolation_=new TH1F( "photonIsolation", "photonIsolation", 60, 0, 30 );
	pPhotonIsolation_->SetDirectory(pDirectory);

	pNeutralIsolation_=new TH1F( "neutralIsolation", "neutralIsolation", 60, 0, 30 );
	pNeutralIsolation_->SetDirectory(pDirectory);

	pPileupIsolation_=new TH1F( "pileupIsolation", "pileupIsolation", 60, 0, 30 );
	pPileupIsolation_->SetDirectory(pDirectory);

	pRelativeChargedIsolation_=new TH1F( "relativeChargedIsolation", "relativeChargedIsolation", 60, 0, 1 );
	pRelativeChargedIsolation_->SetDirectory(pDirectory);

	pRelativePhotonIsolation_=new TH1F( "relativePhotonIsolation", "relativePhotonIsolation", 60, 0, 1 );
	pRelativePhotonIsolation_->SetDirectory(pDirectory);

	pRelativeNeutralIsolation_=new TH1F( "relativeNeutralIsolation", "relativeNeutralIsolation", 60, 0, 1 );
	pRelativeNeutralIsolation_->SetDirectory(pDirectory);

	pRelativePileupIsolation_=new TH1F( "relativePileupIsolation", "relativePileupIsolation", 60, 0, 1 );
	pRelativePileupIsolation_->SetDirectory(pDirectory);

	pRelativeIsolation_=new TH1F( "relativeIsolation", "relativeIsolation", 120, 0, 2 );
	pRelativeIsolation_->SetDirectory(pDirectory);

	pDeltaBetaCorrectedIsolation_=new TH1F( "deltaBetaCorrectedIsolation", "deltaBetaCorrectedIsolation", 120, 0, 2 );
	pDeltaBetaCorrectedIsolation_->SetDirectory(pDirectory);

	pRho25CorrectedIsolation_=new TH1F( "rho25CorrectedIsolation", "rho25CorrectedIsolation", 120, 0, 2 );
	pRho25CorrectedIsolation_->SetDirectory(pDirectory);

	if( createIsolationTree_ )
	{
		pIsolationTree_=new TTree("isolationTree","Data about the muon isolation");
		pIsolationTree_->Branch("numberOfPrimaryVertices",&numberOfPrimaryVertices_branch_,"numberOfPrimaryVertices/I");
		pIsolationTree_->Branch("chargedIsolation",&chargedIsolation_branch_,"chargedIsolation/F");
		pIsolationTree_->Branch("photonIsolation",&photonIsolation_branch_,"photonIsolation/F");
		pIsolationTree_->Branch("neutralIsolation",&neutralIsolation_branch_,"neutralIsolation/F");
		pIsolationTree_->Branch("pileupIsolation",&pileupIsolation_branch_,"pileupIsolation/F");
		pIsolationTree_->Branch("rho25",&rho25_branch_,"rho25/F");
		pIsolationTree_->Branch("pT",&pT_branch_,"pT/F");
		pIsolationTree_->SetDirectory( pDirectory );
	}
	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::MuonInfoPlotSet::fill( const VHbbEvent::MuonInfo& muon, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MuonInfoPlotSet::book() - histograms have not been booked" );

	// Get the additional info about the event if it's available
	int numberOfPrimaryVertices=-1;
	float rho25=-1;
	if( pAuxInfo )
	{
		numberOfPrimaryVertices=pAuxInfo->pvInfo.nVertices;
		rho25=pAuxInfo->puInfo.rho25;
	}

	pGlobalChi2_->Fill( muon.globChi2 );
	pNumberOfPixelHits_->Fill( muon.nPixelHits );
	pNumberOfGlobalHits_->Fill( muon.globNHits );
	pNumberOfHits_->Fill( muon.nHits );
	pCategory_->Fill( muon.cat );
	pNumberOfMatches_->Fill( muon.nMatches );
	pIPDB_->Fill( muon.ipDb );
	pEta_->Fill( muon.p4.Eta() );
	pPt_->Fill( muon.p4.Pt() );
	pChargedIsolation_->Fill( muon.pfChaIso );
	pPhotonIsolation_->Fill( muon.pfPhoIso );
	pNeutralIsolation_->Fill( muon.pfNeuIso );
	pPileupIsolation_->Fill( muon.pfChaPUIso );
	pRelativeChargedIsolation_->Fill( muon.pfChaIso/muon.p4.Pt() );
	pRelativePhotonIsolation_->Fill( muon.pfPhoIso/muon.p4.Pt() );
	pRelativeNeutralIsolation_->Fill( muon.pfNeuIso/muon.p4.Pt() );
	pRelativePileupIsolation_->Fill( muon.pfChaPUIso/muon.p4.Pt() );
	pRelativeIsolation_->Fill( combinedRelativeIsolation(muon) );
	pDeltaBetaCorrectedIsolation_->Fill( deltaBetaCorrectedIsolation(muon) );
	if( pAuxInfo ) pRho25CorrectedIsolation_->Fill( rho25CorrectedIsolation(muon,rho25) ); // Don't bother filling if I couldn't get the rho25 info

	// If the TTree is not null then isolation data has been requested
	if( pIsolationTree_ )
	{
		numberOfPrimaryVertices_branch_=numberOfPrimaryVertices;
		chargedIsolation_branch_=muon.pfChaIso;
		photonIsolation_branch_=muon.pfPhoIso;
		neutralIsolation_branch_=muon.pfNeuIso;
		pileupIsolation_branch_=muon.pfChaPUIso;
		rho25_branch_=rho25;
		pT_branch_=muon.p4.Pt();

		pIsolationTree_->Fill();
	}
}


float trkupgradeanalysis::MuonInfoPlotSet::combinedRelativeIsolation( const VHbbEvent::MuonInfo& muon )
{
	return (muon.pfChaIso+muon.pfPhoIso+muon.pfNeuIso)/muon.p4.Pt();
}

float trkupgradeanalysis::MuonInfoPlotSet::deltaBetaCorrectedIsolation( const VHbbEvent::MuonInfo& muon, float deltaBetaFactor )
{
	// deltaBetaFactor defaults to -0.5
	float correctedNeutralIsolation=muon.pfPhoIso+muon.pfNeuIso+deltaBetaFactor*muon.pfChaPUIso;
	if( correctedNeutralIsolation<0 ) correctedNeutralIsolation=muon.pfPhoIso+muon.pfNeuIso;
	return (muon.pfChaIso+correctedNeutralIsolation)/muon.p4.Pt();
}

float trkupgradeanalysis::MuonInfoPlotSet::rho25CorrectedIsolation( const VHbbEvent::MuonInfo& muon, float rho25 )
{
	float uncorrectedIsolation=combinedRelativeIsolation(muon);
	float correctedIsolation=uncorrectedIsolation-(M_PI*0.4*0.4*rho25)/muon.p4.Pt();
	if( correctedIsolation<0 ) correctedIsolation=0;

	return correctedIsolation;
}
