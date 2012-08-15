#ifndef trkupgradeanalysis_SimpleJetPlotSet_h
#define trkupgradeanalysis_SimpleJetPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"

// Forward declarations
class TH1F;
class TDirectory;
class VHbbEventAuxInfo;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about SimpleJet objects.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 23/May/2012
	 */
	class SimpleJetPlotSet
	{
	public:
		SimpleJetPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbEvent::SimpleJet& jet, const VHbbEventAuxInfo* pAuxInfo=NULL );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* transverseMomentum_;
		TH1F* numberOfTracks_;
		TH1F* numberOfConstituents_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_SimpleJetPlotSet_h"
