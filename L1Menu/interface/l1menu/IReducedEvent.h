#ifndef l1menu_IReducedEvent_h
#define l1menu_IReducedEvent_h


namespace l1menu
{
	/** @brief Interface for a simplified event format. The event just has the minimum threshold to pass for each trigger recorded.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/May/2013
	 */
	class IReducedEvent
	{
	public:
		virtual ~IReducedEvent() {}
		virtual float parameterValue( size_t parameterNumber ) = 0;
		virtual size_t parameterNumber( const l1menu::ITrigger& trigger, const std::string& parameterName ) = 0;
		virtual float weight() const = 0; ///< @brief The weighting this event has been given
	};

} // end of namespace l1menu


#endif