#include "TrkUpgradeAnalysis/VHbb/interface/NTupleCandidateProxy.h"

#include <stdexcept>
#include <cmath>
#include <iostream>

#include <TTree.h>

// Use the unnamed namespace for things I only want in this file
namespace
{
	// These are copied from VHbbAnalysis/VHbbDataFormats/bin/Ntupler.cc,
	// I tried including the file to get them but it causes linker errors.
#define MAXJ 30
#define MAXL 10
#define MAXB 10
	struct HiggsInfo
	{
	  float mass;
	  float pt;
	  float eta;
	  float phi;
	  float dR;
	  float dPhi;
	  float dEta;
	  ~HiggsInfo() {}
	};

	struct TrackInfo
	{
	  float mass;  //MT in case of W
	  float pt;
	  float eta;
	  float phi;
	  ~TrackInfo() {}
	};

	/** @brief My wrapper around the ntuple jet info
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 05/Mar/12
	 */
	struct JetInfo
	{
		bool id[MAXJ];
		float eta[MAXJ];
		float pt[MAXJ];
		float csv[MAXJ];

		void setBranchAddress( TTree* pTree, const std::string& namePrefix )
		{
			pTree->SetBranchAddress( (namePrefix+"id").c_str(), &id );
			pTree->SetBranchAddress( (namePrefix+"eta").c_str(), &eta );
			pTree->SetBranchAddress( (namePrefix+"pt").c_str(), &pt );
			pTree->SetBranchAddress( (namePrefix+"csv").c_str(), &csv );
		}
		~JetInfo() {}
	};
}

namespace trkupgradeanalysis
{

	/** @brief The pimple definition for the NTupleCandidateProxy class
	 *
	 * Google "pimple idiom" if you don't know what this is about.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 05/Mar/12
	 */
	struct NTupleCandidateProxy_pImple
	{
		~NTupleCandidateProxy_pImple() {}

		int Vtype; ///< @brief The CandidateType.

		HiggsInfo H;
		TrackInfo V;

		::JetInfo hJet;
		::JetInfo aJet;
		int nhJets; ///< @brief The number of additional jets before any cleaning.
//		bool hJet_id[MAXJ]; // MAXJ is defined in Ntupler.cc
//		float hJet_eta[MAXJ];
//		float hJet_pt[MAXJ];

		int naJets; ///< @brief The number of additional jets before any cleaning.
//		bool aJet_id[MAXJ]; // MAXJ is defined in Ntupler.cc
//		float aJet_eta[MAXJ];
//		float aJet_pt[MAXJ];

	};
}

trkupgradeanalysis::NTupleCandidateProxy::NTupleCandidateProxy( TTree* pTree )
	: pTree_(pTree), pImple_( new NTupleCandidateProxy_pImple )
{
	pTree_->SetBranchAddress("Vtype",&pImple_->Vtype);

	pTree_->SetBranchAddress("H",&pImple_->H);
	pTree_->SetBranchAddress("V",&pImple_->V);

	pTree_->SetBranchAddress("nhJets",&pImple_->nhJets);
	pImple_->hJet.setBranchAddress( pTree_, "hJet_" );

	pTree_->SetBranchAddress("naJets",&pImple_->naJets);
	pImple_->aJet.setBranchAddress( pTree_, "aJet_" );

	maxCandidateNumber_=pTree_->GetEntries();
	candidateNumber_=-1; // Set to minus one, so that the first call to nextCandidate() sets it to the first (0) entry
	pTree_->GetEntry(0); // Load in the first candidate so that the memory contents isn't random. Note the first call to nextCandidate() will keep it on the 0th entry.
}

trkupgradeanalysis::NTupleCandidateProxy::~NTupleCandidateProxy()
{
	// No operation
}

bool trkupgradeanalysis::NTupleCandidateProxy::nextCandidate()
{
	++candidateNumber_;
	if( candidateNumber_>=maxCandidateNumber_ ) return false;

	pTree_->GetEntry(candidateNumber_);
	return true;
}

int trkupgradeanalysis::NTupleCandidateProxy::numberOfRawAdditionalJets()
{
	return pImple_->naJets;
}

int trkupgradeanalysis::NTupleCandidateProxy::numberOfAdditionalJets()
{
	int numberOfAdditionalJets=0;
	for( int b=0; b<pImple_->naJets; ++b )
	{
//		if( b==MAXJ ) throw std::runtime_error("VHbbEvent::numberOfAdditionalJets() - Too many additional jets.");
		if( pImple_->aJet.id[b] == 1 && std::fabs(pImple_->aJet.eta[b]) < 2.5 && pImple_->aJet.pt[b] > 20) ++numberOfAdditionalJets;
//		std::cout << "pImple_->aJet.id[b]=" << pImple_->aJet.id[b] << " std::fabs(pImple_->aJet.eta[b])=" << std::fabs(pImple_->aJet.eta[b]) << " pImple_->aJet.pt[b]=" << pImple_->aJet.pt[b] << std::endl;
	}
	return numberOfAdditionalJets;
}


int trkupgradeanalysis::NTupleCandidateProxy::candidateType()
{
	return pImple_->Vtype;
}

int trkupgradeanalysis::NTupleCandidateProxy::numberOfHiggsCandidateJets()
{
	return pImple_->nhJets;
}

float trkupgradeanalysis::NTupleCandidateProxy::csvOfHiggsCandidateJet( int jetNumber )
{
	if( jetNumber>=pImple_->nhJets || jetNumber>=MAXJ || jetNumber<0 ) throw std::runtime_error("NTupleCandidateProxy::csvOfHiggsCandidateJet() - Invalid jet number.");

	return pImple_->hJet.csv[jetNumber];
}

float trkupgradeanalysis::NTupleCandidateProxy::zMass()
{
	return pImple_->V.mass;
}

