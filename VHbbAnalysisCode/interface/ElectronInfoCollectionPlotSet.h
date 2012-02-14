#ifndef trkupgradeanalysis_ElectronInfoCollectionPlotSet_h
#define trkupgradeanalysis_ElectronInfoCollectionPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"
#include "TrkUpgradeAnalysis/VHbb/interface/ElectronInfoPlotSet.h"

// Forward declarations
class TH1F;
class TDirectory;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about ElectronInfo objects.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 27/Nov/2011
	 */
	class ElectronInfoCollectionPlotSet
	{
	public:
		ElectronInfoCollectionPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const std::vector<VHbbEvent::ElectronInfo>& electronCollection );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pNumberOfElectrons_;
		trkupgradeanalysis::ElectronInfoPlotSet electronInfoPlotSet_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_ElectronInfoCollectionPlotSet_h"
