#ifndef l1menu_implementation_MenuRateImplementation_h
#define l1menu_implementation_MenuRateImplementation_h

#include "l1menu/IMenuRate.h"
#include <vector>
#include "l1menu/implementation/TriggerRateImplementation.h"

//
// Forward declarations
//
namespace l1menu
{
	class ITrigger;
	class ITriggerRate;
}


namespace l1menu
{
	namespace implementation
	{
		/** @brief Implementation of the IMenuRate interface.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 28/Jun/2013
		 */
		class MenuRateImplementation : public l1menu::IMenuRate
		{
		public:
			void setTotalFraction( float fraction );
			void setScaling( float scaling );
			float scaling() const;
			void addTriggerRate( const l1menu::ITrigger& trigger, float fraction );
			// Methods required by the l1menu::IMenuRate interface
			virtual float totalFraction() const;
			virtual float totalRate() const;
			virtual const std::vector<const l1menu::ITriggerRate*>& triggerRates() const;
		protected:
			float totalFraction_;
			float scaling_;
			std::vector<TriggerRateImplementation> triggerRates_;
			mutable std::vector<const l1menu::ITriggerRate*> baseClassReferences_;
		};


	} // end of the implementation namespace
} // end of the l1menu namespace
#endif
