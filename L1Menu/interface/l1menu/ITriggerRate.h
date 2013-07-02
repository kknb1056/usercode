#ifndef l1menu_ITriggerRate_h
#define l1menu_ITriggerRate_h

#include <vector>

//
// Forward declarations
//
namespace l1menu
{
	class ITrigger;
}

namespace l1menu
{
	/** @brief Interface to get information about the trigger rate. Read only.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 24/Jun/2013
	 */
	class ITriggerRate
	{
	public:
		virtual ~ITriggerRate() {}

		virtual const l1menu::ITrigger& trigger() const = 0;

		/** @brief The fraction of events that this trigger passed, so before applying any scaling. */
		virtual float fraction() const = 0;

		/** @brief The rate, so fraction multiplied by the scaling. */
		virtual float rate() const = 0;
	};

} // end of namespace l1menu

#endif
