#ifndef trkupgradeanalysis_SimpleJetCollectionPlotSet_h
#define trkupgradeanalysis_SimpleJetCollectionPlotSet_h

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"
#include "TrkUpgradeAnalysis/VHbb/interface/SimpleJetPlotSet.h"

// Forward declarations
class TH1F;
class TDirectory;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about SimpleJet object collections.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 23/May/2012
	 */
	class SimpleJetCollectionPlotSet
	{
	public:
		SimpleJetCollectionPlotSet();
		void book( TDirectory* pDirectory );
		void fill( const std::vector<VHbbEvent::SimpleJet>& jetCollection, const VHbbEventAuxInfo* pAuxInfo=NULL );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pNumberOfJets_;
		trkupgradeanalysis::SimpleJetPlotSet simpleJetPlotSet_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_SimpleJetCollectionPlotSet_h"
