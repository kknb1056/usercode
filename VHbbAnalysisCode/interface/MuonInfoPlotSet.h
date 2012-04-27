#ifndef trkupgradeanalysis_MuonInfoPlotSet_h
#define trkupgradeanalysis_MuonInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

// Forward declarations
class TH1F;
class TDirectory;
class TTree;
class VHbbEventAuxInfo;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about MuonInfo objects.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 27/Nov/2011
	 */
	class MuonInfoPlotSet
	{
	public:
		/** @brief Only constructor
		 *
		 * createIsolationTree - set whether to record the isolation values in a TTree or not. Default
		 * is off to save space.
		 */
		MuonInfoPlotSet( bool createIsolationTree=false );
		void book( TDirectory* pDirectory );
		void fill( const VHbbEvent::MuonInfo& muon, const VHbbEventAuxInfo* pAuxInfo=NULL );

		static float combinedRelativeIsolation( const VHbbEvent::MuonInfo& muon );
		static float deltaBetaCorrectedIsolation( const VHbbEvent::MuonInfo& muon, float deltaBetaFactor=-0.5 );
		static float rho25CorrectedIsolation( const VHbbEvent::MuonInfo& muon, float rho25 );
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

		TH1F* pGlobalChi2_;
		TH1F* pNumberOfPixelHits_;
		TH1F* pNumberOfGlobalHits_;
		TH1F* pNumberOfHits_;
		TH1F* pCategory_;
		TH1F* pNumberOfMatches_;
		TH1F* pIPDB_;
		TH1F* pEta_;
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
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_MuonInfoPlotSet_h"
