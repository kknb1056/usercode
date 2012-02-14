#ifndef trkupgradeanalysis_MCInfoPlotSet_h
#define trkupgradeanalysis_MCInfoPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"

#include <map>
#include <string>

// Forward declarations
class TH1F;
class TDirectory;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about Monte Carlo information in the event.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 15/Jan/2012
	 */
	class MCInfoPlotSet
	{
	public:
		MCInfoPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbEventAuxInfo& eventAuxInfo );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pEventString_;
		std::map<std::string,int> stringToBinMap_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_MCInfoPlotSet_h"
