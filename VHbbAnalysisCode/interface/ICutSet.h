#ifndef trkupgradeanalysis_ICutSet_h
#define trkupgradeanalysis_ICutSet_h

#include <string>

// Forward declarations
namespace trkupgradeanalysis
{
	class IBasicCut;
}

namespace trkupgradeanalysis
{

	class ICutSet
	{
	public:
		virtual std::string name() const = 0;
		virtual size_t numberOfCuts() const = 0;
		virtual bool allCutsPassed() const = 0;
		virtual bool cutNPassed( size_t cutNumber ) const = 0;
		virtual bool everythingOtherThanCutNPassed( size_t cutNumber ) const = 0;
		virtual const trkupgradeanalysis::IBasicCut& cutAt( size_t a ) const = 0;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_ICutSet_h"
