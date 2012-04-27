#ifndef trkupgradeanalysis_IsolationStudyPlotSet_h
#define trkupgradeanalysis_IsolationStudyPlotSet_h

#include <vector>
#include <utility> // for std::pair

// Can't forward declare this because I need VHbbEvent::MuonInfo
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoCollectionPlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoPlotSet.h"

// Forward declarations
class TH1F;
class TDirectory;
class VHbbEventAuxInfo;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms that study lepton isolation.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 11/Apr/2012
	 */
	class IsolationStudyPlotSet
	{
	public:
		IsolationStudyPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbEvent& event, const VHbbEventAuxInfo* pAuxInfo );
	private:
		bool histogramHaveBeenBooked_;

		/** @brief Applies the same cleaning as VHbb package, apart from isolation.
		 *
		 * Code copied almost verbatim from HbbCandidateFinderAlgo::findMuons(...) in
		 * VHbbAnalysis/VHbbDataFormats/src/HbbCandidateFinderAlgo.cc, but with the line about isolation
		 * dropped. Copied from the VHbb package as it was on 12/Mar/2012.
		 */
		std::vector<VHbbEvent::MuonInfo> cleanMuons( const std::vector<VHbbEvent::MuonInfo>& muons );

		/** @brief Returns the first muon in the collection and the next one with an opposite charge.
		 *
		 * The intention is that the collection should be sorted high to low by pT, so that this method
		 * gives the highest pT muon and the next highest with opposite charge.
		 *
		 * @throw std::runtime_error If there are fewer than two entries in the input collection, or if
		 * no oppositely charged pairs can be found.
		 */
		std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> findOppositelyChargedPair( const std::vector<VHbbEvent::MuonInfo>& muons );

		/** @brief Returns true if the first muon in the pair is more isolated than the second.
		 *
		 * Uses normal relative isolation, i.e. the sum of the charged, photonic and neutral energy around
		 * the particle divided by the pT. The cone size where the energy is summed will have been set earlier
		 * in the configuration sequence.
		 */
		bool firstMuonIsMoreIsolated( std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> diMuons );

		/** @brief Returns true if the first muon in the pair is more isolated than the second.
		 *
		 * Uses combined relative isolation with the delta beta correction, i.e. the sum of the charged, photonic
		 * and corrected neutral energy around the particle divided by the pT.  The corrected neutral energy is
		 * the neutral energy minus half the charged pile up energy (since apparently the neutral pile up energy
		 * is approximately half the charged). If the corrected neutral energy is less than zero, the uncorrected
		 * is used (i.e. reverts to normal combined relative isolation). The cone size where the energy is summed
		 * will have been set earlier in the configuration sequence.
		 */
		bool firstMuonIsMoreDeltaBetaCorrectedIsolated( std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> diMuons );

//		TTree* pLeastIsolatedDiMuonTree_;
//		int numberOfPrimaryVertices_branch_; // All these are variables to hold the branch data
//		float chargedIsolation_branch_;
//		float photonIsolation_branch_;
//		float neutralIsolation_branch_;
//		float pileupIsolation_branch_;
//		float pT_branch_;
		trkupgradeanalysis::MuonInfoCollectionPlotSet cleanedMuons_; ///< Plots for the muons that have had cleaning applied (except isolation)
		trkupgradeanalysis::MuonInfoPlotSet highestPtDiMuon_; ///< Plots for the muon in the pair with the highest pT
		trkupgradeanalysis::MuonInfoPlotSet lowestPtDiMuon_;
		trkupgradeanalysis::MuonInfoPlotSet mostIsolatedDiMuon_; ///< Plots for the muon in the pair that is most isolated
		trkupgradeanalysis::MuonInfoPlotSet leastIsolatedDiMuon_;
		trkupgradeanalysis::MuonInfoPlotSet leastDeltaBetaCorrectedIsolatedDiMuon_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_IsolationStudyPlotSet_h"
