#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidatePlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

trkupgradeanalysis::VHbbCandidatePlotSet::VHbbCandidatePlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::VHbbCandidatePlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::VHbbCandidatePlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	pCandidateType_=new TH1F( "candidateType","Candidate type",6,-0.5,5.5);
	pCandidateType_->SetDirectory(pDirectory);
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::Zmumu+1, "Zmumu");
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::Zee+1, "Zee");
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::Wmun+1, "Wmun");
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::Wen+1, "Wen");
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::Znn+1, "Znn");
	pCandidateType_->GetXaxis()->SetBinLabel( VHbbCandidate::UNKNOWN+1, "UNKNOWN");

	pHiggsMass_=new TH1F( "higgsMass","Higgs candidate mass",75,0,250);
	pHiggsMass_->SetDirectory(pDirectory);

	pZMass_=new TH1F( "ZMass","Z candidate mass",65,0,200);
	pZMass_->SetDirectory(pDirectory);

	pNumberOfElectrons_=new TH1F( "numberOfElectrons","Number of electrons",6,-0.5,5.5);
	pNumberOfElectrons_->SetDirectory(pDirectory);

	pNumberOfMuons_=new TH1F( "numberOfMuons","Number of muons",6,-0.5,5.5);
	pNumberOfMuons_->SetDirectory(pDirectory);

	pNumberOfTaus_=new TH1F( "numberOfTaus","Number of taus",6,-0.5,5.5);
	pNumberOfTaus_->SetDirectory(pDirectory);

	pNumberOfMETs_=new TH1F( "numberOfMETs","Number of MET info objects",6,-0.5,5.5);
	pNumberOfMETs_->SetDirectory(pDirectory);

	pNumberOfAdditionalJets_=new TH1F( "numberAdditionalJets", "Number of additional jets", 101, -0.5, 100.5 );
	pNumberOfAdditionalJets_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::VHbbCandidatePlotSet::fill( const VHbbCandidate& vhbbCandidate )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::VHbbCandidatePlotSet::book() - histograms have not been booked" );

	pCandidateType_->Fill( vhbbCandidate.candidateType );
	pHiggsMass_->Fill( vhbbCandidate.H.p4.M() );
	pZMass_->Fill( vhbbCandidate.V.p4.M() );
	pNumberOfElectrons_->Fill( vhbbCandidate.V.electrons.size() );
	pNumberOfMuons_->Fill( vhbbCandidate.V.muons.size() );
	pNumberOfTaus_->Fill( vhbbCandidate.V.taus.size() );
	pNumberOfMETs_->Fill( vhbbCandidate.V.mets.size() );
	pNumberOfAdditionalJets_->Fill( vhbbCandidate.additionalJets.size() );
}

