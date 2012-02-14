#ifndef trkupgradeanalysis_ElectronInfoPlotSet_h
#define trkupgradeanalysis_ElectronInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

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
		void fill( const VHbbEvent::ElectronInfo& electron );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pEta_;
		TH1F* pPt_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_ElectronInfoPlotSet_h"
