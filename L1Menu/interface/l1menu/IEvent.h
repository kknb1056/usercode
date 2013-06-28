#ifndef l1menu_IEvent_h
#define l1menu_IEvent_h

//
// Forward declarations
//
namespace l1menu
{
	class ITrigger;
}

namespace l1menu
{
	/** @brief Interface for the most basic form of event.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 25/Jun/2013
	 */
	class IEvent
	{
	public:
		virtual ~IEvent() {}
		virtual bool passesTrigger( const l1menu::ITrigger& trigger ) const = 0;
		virtual float weight() const = 0; ///< @brief The weighting this event has been given
	};

} // end of namespace l1menu


#endif
