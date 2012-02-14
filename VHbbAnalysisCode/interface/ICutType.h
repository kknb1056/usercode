#ifndef trkupgradeanalysis_cuts_ICutType_h
#define trkupgradeanalysis_cuts_ICutType_h

#include <memory>
#include <string>

namespace trkupgradeanalysis
{
	namespace cuts
	{
		/** @brief Abstract base class for cuts.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Dec/2011
		 */
		class ICutType
		{
		public:
			virtual ~ICutType() {}
			virtual bool apply( double value ) const = 0;
			virtual std::auto_ptr<ICutType> copy() const = 0;
			virtual std::string name() const = 0;
		};

	} // end of namespace cuts

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_cuts_ICutType_h"
