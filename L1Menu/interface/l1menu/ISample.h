#ifndef l1menu_ISample_h
#define l1menu_ISample_h

#include "l1menu/TriggerRates.h"

// Forward declarations
namespace l1menu
{
	class TriggerMenu;
}


namespace l1menu
{
	class ISample
	{
	public:
		virtual ~ISample() {}
		virtual const l1menu::TriggerRates rate( const l1menu::TriggerMenu& menu ) const = 0;
	};

} // end of namespace l1menu

#endif
