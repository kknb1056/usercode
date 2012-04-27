#ifndef trkupgradeanalysis_MuonInfoCollectionPlotSet_h
#define trkupgradeanalysis_MuonInfoCollectionPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"
#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoPlotSet.h"

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
	class MuonInfoCollectionPlotSet
	{
	public:
		/** @brief Only constructor
		 *
		 * createIsolationTree - set whether to record the isolation values in a TTree or not. Default
		 * is off to save space.
		 */
		MuonInfoCollectionPlotSet( bool createIsolationTree=false );
		void book( TDirectory* pDirectory );
		void fill( const std::vector<VHbbEvent::MuonInfo>& muonCollection, const VHbbEventAuxInfo* pAuxInfo=NULL );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pNumberOfMuons_;
		trkupgradeanalysis::MuonInfoPlotSet muonInfoPlotSet_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_MuonInfoCollectionPlotSet_h"
