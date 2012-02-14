#ifndef trkupgradeanalysis_VHbbCandidatePlotSet_h
#define trkupgradeanalysis_VHbbCandidatePlotSet_h

// Forward declarations
class TH1F;
class TDirectory;
class VHbbCandidate;

namespace trkupgradeanalysis
{
	/** @brief A class to take care of booking and filling histograms about VHbbCandidates.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 26/Nov/2011
	 */
	class VHbbCandidatePlotSet
	{
	public:
		VHbbCandidatePlotSet();
		void book( TDirectory* pDirectory );
		void fill( const VHbbCandidate& vhbbCandidate );
	private:
		bool histogramHaveBeenBooked_;

		TH1F* pCandidateType_;
		TH1F* pHiggsMass_;
		TH1F* pZMass_;
		TH1F* pNumberOfElectrons_;
		TH1F* pNumberOfMuons_;
		TH1F* pNumberOfTaus_;
		TH1F* pNumberOfMETs_;
		TH1F* pNumberOfAdditionalJets_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_VHbbCandidatePlotSet_h"
