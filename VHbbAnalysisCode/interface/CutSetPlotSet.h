#ifndef trkupgradeanalysis_CutSetPlotSet_h
#define trkupgradeanalysis_CutSetPlotSet_h

#include <vector>
#include <boost/shared_ptr.hpp>
#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidatePlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"

// Forward declarations
class TH1F;
class TDirectory;
class VHbbCandidate;
class TTree;
class TBranch;
namespace trkupgradeanalysis
{
	namespace tools
	{
		class NTupleRow;
	}
}

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about cuts on VHbbCandidates.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 26/Nov/2011
	 */
	class CutSetPlotSet
	{
	public:
		CutSetPlotSet( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet, bool createNTupleOfAdditionalVariables=true );
		void book( TDirectory* pDirectory );
		void fill( const VHbbCandidate& vhbbCandidate );
		void fill( const trkupgradeanalysis::tools::NTupleRow& ntupleRow );
	private:
		bool histogramHaveBeenBooked_;

		boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet_;

		VHbbCandidatePlotSet passedCandidateHistograms_;
		TH1F* pCutsPassed_;
		TH1F* pCutFlow_;
		std::vector<TH1F*> nMinus1Plots_;
		std::vector<TH1F*> allCandidatesPlots_;
		std::vector<TH1F*> passedCandidatesPlots_;
		std::vector<TH1F*> additionalVariablePlots_;
		bool createNTupleOfAdditionalVariables_;
		std::vector<TBranch*> additionalVariableBranches_; ///< If ntuplising, a branch for each variable
		TTree* pAdditionalVariableTree_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_CutSetPlotSet_h"
