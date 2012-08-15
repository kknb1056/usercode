#ifndef trkupgradeanalysis_ElectronInfoPlotSet_h
#define trkupgradeanalysis_ElectronInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"
#include "TrkUpgradeAnalysis/VHbb/interface/LeptonInfoPlotSet.h"

// Forward declarations
class TH1F;
class TDirectory;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about ElectronInfo objects.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/Nov/2011
	 */
	class ElectronInfoPlotSet
	{
	public:
		ElectronInfoPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbEvent::ElectronInfo& electron, const VHbbEventAuxInfo* pAuxInfo=NULL );
	private:
		bool histogramHaveBeenBooked_;

		trkupgradeanalysis::LeptonInfoPlotSet<VHbbEvent::ElectronInfo> leptonPlotSet_; ///< Common plots used by electron and muon plotsets

		// Plots specific to electrons
		TH1F* pID95_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_ElectronInfoPlotSet_h"
