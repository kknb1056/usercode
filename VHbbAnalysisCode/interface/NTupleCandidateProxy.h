#ifndef trkupgradeanalysis_NTupleCandidateProxy_h
#define trkupgradeanalysis_NTupleCandidateProxy_h

#include <memory>

// Forward declarations
class TTree;

namespace trkupgradeanalysis
{
	/** @brief Proxy object to handle passing information from the VHbb ntuples.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/Feb/2012
	 */
	class NTupleCandidateProxy
	{
	public:
		NTupleCandidateProxy( TTree* pTree );
		~NTupleCandidateProxy();

		bool nextCandidate();
		int numberOfRawAdditionalJets();
		int numberOfAdditionalJets();

		int candidateType();
		int numberOfHiggsCandidateJets();
		float csvOfHiggsCandidateJet( int jetNumber ); ///<@ Jet number is counted from 0

		float zMass();
	protected:
		TTree* pTree_;
		int candidateNumber_;
		int maxCandidateNumber_;

//		int Vtype_; ///< @brief The CandidateType.
//		int naJets_; ///< @brief The number of additional jets before any cleaning.
//		bool aJet_id_[JETS_ARRAY_MAX];
//		float aJet_eta_[JETS_ARRAY_MAX];
//		float aJet_pt_[JETS_ARRAY_MAX];

		//std::auto_ptr<struct NTupleCandidateProxy_pImple> pImple_;
		struct NTupleCandidateProxy_pImple* pImple_;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_NTupleCandidateProxy_h"
