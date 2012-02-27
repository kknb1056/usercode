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
		CutSetPlotSet( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet );
		void book( TDirectory* pDirectory );
		void fill( const VHbbCandidate& vhbbCandidate );
	private:
		bool histogramHaveBeenBooked_;

		boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet_;

		VHbbCandidatePlotSet passedCandidateHistograms_;
		TH1F* pCutsPassed_;
		TH1F* pCutFlow_;
		std::vector<TH1F*> nMinus1Plots_;
		std::vector<TH1F*> allCandidatesPlots_;
		std::vector<TH1F*> passedCandidatesPlots_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_CutSetPlotSet_h"
