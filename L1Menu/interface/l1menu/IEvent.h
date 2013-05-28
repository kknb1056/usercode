#ifndef l1menu_IEvent_h
#define l1menu_IEvent_h


// Forward declarations
namespace L1Analysis
{
	class L1AnalysisDataFormat;
}


namespace l1menu
{
	/** @brief Interface for the event format. Bundles L1AnalysisDataFormat and the trigger bits into one package.
	 *
	 * Currently just wraps L1Analysis::L1AnalysisDataFormat and a boolean array for the trigger bits
	 * into one class so that it can easily be passed around. Also useful because it can be passed
	 * around and have some operations done on it by code that has no knowledge of L1Analysis package.
	 *
	 * Later on I might wrap the L1AnalysisDataFormat more fully so that everything can be done without
	 * knowledge of L1AnalysisDataFormat, just using this lightweight interface.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 21/May/2013
	 */
	class IEvent
	{
	public:
		virtual ~IEvent() {}
		virtual L1Analysis::L1AnalysisDataFormat& rawEvent() = 0;
		virtual const L1Analysis::L1AnalysisDataFormat& rawEvent() const = 0;
		virtual bool* physicsBits() = 0; ///< @brief A 128 element array of the physics bits
		virtual const bool* physicsBits() const = 0; ///< @brief Const access to the 128 element array of the physics bits.
		virtual float weight() const = 0; ///< @brief The weighting this event has been given
	};

} // end of namespace l1menu


#endif
