#ifndef trkupgradeanalysis_MuonInfoPlotSet_h
#define trkupgradeanalysis_MuonInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

// Forward declarations
class TH1F;
class TDirectory;

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
		MuonInfoPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbEvent::MuonInfo& muon );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pGlobalChi2_;
		TH1F* pNumberOfPixelHits_;
		TH1F* pNumberOfGlobalHits_;
		TH1F* pNumberOfHits_;
		TH1F* pCategory_;
		TH1F* pNumberOfMatches_;
		TH1F* pIPDB_;
		TH1F* pEta_;
		TH1F* pPt_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_MuonInfoPlotSet_h"
