#ifndef trkupgradeanalysis_BasicVHbbCandidateCut_h
#define trkupgradeanalysis_BasicVHbbCandidateCut_h

#include "TrkUpgradeAnalysis/VHbb/interface/IBasicCut.h"

//#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

// Forward declarations
class VHbbCandidate;
namespace trkupgradeanalysis
{
	namespace tools
	{
		class NTupleRow;
	}
}

namespace trkupgradeanalysis
{

	class IBasicVHbbCandidateCut : public trkupgradeanalysis::IBasicCut
	{
	public:
		virtual ~IBasicVHbbCandidateCut() {}

		/// @brief Returns true if the supplied VHbbCandidate would pass the cut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const = 0;

		/// @brief Alternative method to work entries previously stored in an NTuple
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const = 0;
	};


} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_BasicVHbbCandidateCut_h"
