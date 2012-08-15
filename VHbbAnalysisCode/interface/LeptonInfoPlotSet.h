#ifndef trkupgradeanalysis_LeptonInfoPlotSet_h
#define trkupgradeanalysis_LeptonInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

// Forward declarations
class TH1F;
class TDirectory;
class TTree;
class VHbbEventAuxInfo;

namespace trkupgradeanalysis
{
	/** @brief An abstract class that incorporates some of the code used for both electron and muon plotsets.
	 *
	 * Note that this isn't intended to be used on its own, rather as a member for ElectronInfoPlotSet and
	 * MuonInfoPlotSet classes to encapsulate common code between them.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 02/Jul/2012
	 */
	template<class T_Lepton>
	class LeptonInfoPlotSet
	{
	public:
		/** @brief Only constructor
		 *
		 * createIsolationTree - set whether to record the isolation values in a TTree or not. Default
		 * is off to save space.
		 */
		LeptonInfoPlotSet( bool createIsolationTree=false );
		void book( TDirectory* pDirectory );
		void fill( const T_Lepton& lepton, const VHbbEventAuxInfo* pAuxInfo=NULL );

		static float combinedRelativeIsolation( const T_Lepton& lepton );
		static float deltaBetaCorrectedIsolation( const T_Lepton& lepton, float deltaBetaFactor=-0.5 );
		static float rho25CorrectedIsolation( const T_Lepton& lepton, float rho25 );
		static float thirdRho25CorrectedIsolation( const T_Lepton& lepton, float rho25 );
		static float deltaBetaCorrectedIsolationNoZeroing( const T_Lepton& lepton, float deltaBetaFactor=-0.5 );
		static float rho25CorrectedIsolationNoZeroing( const T_Lepton& lepton, float rho25 );
		static float thirdRho25CorrectedIsolationNoZeroing( const T_Lepton& lepton, float rho25 );
	private:
		bool histogramHaveBeenBooked_;

		bool createIsolationTree_; ///< Whether or not to record isolation values in a TTree.
		TTree* pIsolationTree_; ///< This TTree is only created if createIsolationTree is set to true in the constructor.
		int numberOfPrimaryVertices_branch_; ///< All these are variables to hold the branch data
		float chargedIsolation_branch_;
		float photonIsolation_branch_;
		float neutralIsolation_branch_;
		float pileupIsolation_branch_;
		float rho25_branch_;
		float pT_branch_;

		TH1F* pEta_;
		TH1F* pPhi_;
		TH1F* pPt_;
		TH1F* pChargedIsolation_;
		TH1F* pPhotonIsolation_;
		TH1F* pNeutralIsolation_;
		TH1F* pPileupIsolation_;
		TH1F* pRelativeChargedIsolation_;
		TH1F* pRelativePhotonIsolation_;
		TH1F* pRelativeNeutralIsolation_;
		TH1F* pRelativePileupIsolation_;
		TH1F* pRelativeIsolation_;
		TH1F* pDeltaBetaCorrectedIsolation_;
		TH1F* pRho25CorrectedIsolation_;
		TH1F* pThirdRho25CorrectedIsolation_;
		TH1F* pDeltaBetaCorrectedIsolationNoZeroing_;
		TH1F* pRho25CorrectedIsolationNoZeroing_;
		TH1F* pThirdRho25CorrectedIsolationNoZeroing_;
	};

} // end of namespace trkupgradeanalysis



//----------------------------------------------------------
//
// Need the implementation here since it's templated
//
//----------------------------------------------------------
#include <stdexcept>
#include <TH1F.h>
#include <TTree.h>
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"


template<class T_Lepton>
trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::LeptonInfoPlotSet( bool createIsolationTree )
	: histogramHaveBeenBooked_(false), createIsolationTree_(createIsolationTree), pIsolationTree_(NULL)
{
	// No operation besides the initialiser list.
}

template<class T_Lepton>
void trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::LeptonInfoPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//


	pEta_=new TH1F( "eta", "Eta", 60, -3, 3 );
	pEta_->SetDirectory(pDirectory);

	pPhi_=new TH1F( "phi", "phi", 60, -3.15, 3.15 );
	pPhi_->SetDirectory(pDirectory);

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

	pThirdRho25CorrectedIsolation_=new TH1F( "thirdRho25CorrectedIsolation", "thirdRho25CorrectedIsolation", 120, 0, 2 );
	pThirdRho25CorrectedIsolation_->SetDirectory(pDirectory);

	pDeltaBetaCorrectedIsolationNoZeroing_=new TH1F( "deltaBetaCorrectedIsolationNoZeroing", "deltaBetaCorrectedIsolationNoZeroing", 120, -1, 1 );
	pDeltaBetaCorrectedIsolationNoZeroing_->SetDirectory(pDirectory);

	pRho25CorrectedIsolationNoZeroing_=new TH1F( "rho25CorrectedIsolationNoZeroing", "rho25CorrectedIsolationNoZeroing", 120, -1, 1 );
	pRho25CorrectedIsolationNoZeroing_->SetDirectory(pDirectory);

	pThirdRho25CorrectedIsolationNoZeroing_=new TH1F( "thirdRho25CorrectedIsolationNoZeroing", "thirdRho25CorrectedIsolationNoZeroing", 120, -1, 1 );
	pThirdRho25CorrectedIsolationNoZeroing_->SetDirectory(pDirectory);

	if( createIsolationTree_ )
	{
		pIsolationTree_=new TTree("isolationTree","Data about the lepton isolation");
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

template<class T_Lepton>
void trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::fill( const T_Lepton& lepton, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::LeptonInfoPlotSet::book() - histograms have not been booked" );

	// Get the additional info about the event if it's available
	int numberOfPrimaryVertices=-1;
	float rho25=-1;
	if( pAuxInfo )
	{
		numberOfPrimaryVertices=pAuxInfo->pvInfo.nVertices;
		rho25=pAuxInfo->puInfo.rho25;
	}

	pEta_->Fill( lepton.p4.Eta() );
	pPhi_->Fill( lepton.p4.Phi() );
	pPt_->Fill( lepton.p4.Pt() );
	pChargedIsolation_->Fill( lepton.pfChaIso );
	pPhotonIsolation_->Fill( lepton.pfPhoIso );
	pNeutralIsolation_->Fill( lepton.pfNeuIso );
	pPileupIsolation_->Fill( lepton.pfChaPUIso );
	pRelativeChargedIsolation_->Fill( lepton.pfChaIso/lepton.p4.Pt() );
	pRelativePhotonIsolation_->Fill( lepton.pfPhoIso/lepton.p4.Pt() );
	pRelativeNeutralIsolation_->Fill( lepton.pfNeuIso/lepton.p4.Pt() );
	pRelativePileupIsolation_->Fill( lepton.pfChaPUIso/lepton.p4.Pt() );
	pRelativeIsolation_->Fill( combinedRelativeIsolation(lepton) );
	pDeltaBetaCorrectedIsolation_->Fill( deltaBetaCorrectedIsolation(lepton) );
	pDeltaBetaCorrectedIsolationNoZeroing_->Fill( deltaBetaCorrectedIsolationNoZeroing(lepton) );
	if( pAuxInfo )
	{
		// Don't bother filling if I couldn't get the rho25 info
		pRho25CorrectedIsolation_->Fill( rho25CorrectedIsolation(lepton,rho25) );
		pRho25CorrectedIsolationNoZeroing_->Fill( rho25CorrectedIsolationNoZeroing(lepton,rho25) );
		pThirdRho25CorrectedIsolation_->Fill( thirdRho25CorrectedIsolation(lepton,rho25) );
		pThirdRho25CorrectedIsolationNoZeroing_->Fill( thirdRho25CorrectedIsolationNoZeroing(lepton,rho25) );
	}

	// If the TTree is not null then isolation data has been requested
	if( pIsolationTree_ )
	{
		numberOfPrimaryVertices_branch_=numberOfPrimaryVertices;
		chargedIsolation_branch_=lepton.pfChaIso;
		photonIsolation_branch_=lepton.pfPhoIso;
		neutralIsolation_branch_=lepton.pfNeuIso;
		pileupIsolation_branch_=lepton.pfChaPUIso;
		rho25_branch_=rho25;
		pT_branch_=lepton.p4.Pt();

		pIsolationTree_->Fill();
	}
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::combinedRelativeIsolation( const T_Lepton& lepton )
{
	return (lepton.pfChaIso+lepton.pfPhoIso+lepton.pfNeuIso)/lepton.p4.Pt();
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::deltaBetaCorrectedIsolation( const T_Lepton& lepton, float deltaBetaFactor )
{
	// deltaBetaFactor defaults to -0.5
	float correctedNeutralIsolation=lepton.pfPhoIso+lepton.pfNeuIso+deltaBetaFactor*lepton.pfChaPUIso;
	if( correctedNeutralIsolation<0 ) correctedNeutralIsolation=lepton.pfPhoIso+lepton.pfNeuIso;
	return (lepton.pfChaIso+correctedNeutralIsolation)/lepton.p4.Pt();
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::rho25CorrectedIsolation( const T_Lepton& lepton, float rho25 )
{
	float uncorrectedIsolation=combinedRelativeIsolation(lepton);
	float correctedIsolation=uncorrectedIsolation-(0.4*0.4*M_PI*rho25)/lepton.p4.Pt();
	if( correctedIsolation<0 ) correctedIsolation=0;

	return correctedIsolation;
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::thirdRho25CorrectedIsolation( const T_Lepton& lepton, float rho25 )
{
	float uncorrectedIsolation=combinedRelativeIsolation(lepton);
	float correctedIsolation=uncorrectedIsolation-(0.4*0.4*M_PI*rho25)/3.0/lepton.p4.Pt();
	if( correctedIsolation<0 ) correctedIsolation=0;

	return correctedIsolation;
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::deltaBetaCorrectedIsolationNoZeroing( const T_Lepton& lepton, float deltaBetaFactor )
{
	// deltaBetaFactor defaults to -0.5
	float correctedNeutralIsolation=lepton.pfPhoIso+lepton.pfNeuIso+deltaBetaFactor*lepton.pfChaPUIso;

	return (lepton.pfChaIso+correctedNeutralIsolation)/lepton.p4.Pt();
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::rho25CorrectedIsolationNoZeroing( const T_Lepton& lepton, float rho25 )
{
	float uncorrectedIsolation=combinedRelativeIsolation(lepton);
	float correctedIsolation=uncorrectedIsolation-(0.4*0.4*M_PI*rho25)/lepton.p4.Pt();


	return correctedIsolation;
}

template<class T_Lepton>
float trkupgradeanalysis::LeptonInfoPlotSet<T_Lepton>::thirdRho25CorrectedIsolationNoZeroing( const T_Lepton& lepton, float rho25 )
{
	float uncorrectedIsolation=combinedRelativeIsolation(lepton);
	float correctedIsolation=uncorrectedIsolation-(0.4*0.4*M_PI*rho25)/3.0/lepton.p4.Pt();


	return correctedIsolation;
}

#endif // end of "#ifndef trkupgradeanalysis_LeptonInfoPlotSet_h"
