#ifndef l1menu_tools_tools_h
#define l1menu_tools_tools_h

/** @file Miscellaneous functions that are useful.
 */

#include <vector>
#include <string>
#include <utility>

//
// Forward declarations
//
namespace l1menu
{
	class ITrigger;
	class L1TriggerDPGEvent;
}


namespace l1menu
{
	namespace tools
	{
		/** @brief Finds all of the parameter names that refer to thresholds.
		 *
		 * Searches through all the parameter names for things that have the form "threshold1",
		 * "threshold2" etcetera. Also looks for things of the form "leg1threshold1", "leg2threshold1"
		 * etcetera for when I get around to implementing the cross triggers.
		 *
		 * @param[in] trigger    The trigger to check.
		 * @return               A std::vector of strings for all of the value parameter names that
		 *                       refer to thresholds.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 28/May/2013
		 */
		std::vector<std::string> getThresholdNames( const l1menu::ITrigger& trigger );

		/** @brief Finds all of the parameter names that don't refer to thresholds.
		 *
		 * Does the opposite of getThresholdNames, so returns all parameter names that getThresholdNames
		 * doesn't.
		 *
		 * @param[in] trigger    The trigger to check.
		 * @return               A std::vector of strings for all of the valued parameter names that
		 *                       don't refer to thresholds.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 30/May/2013
		 */
		std::vector<std::string> getNonThresholdParameterNames( const l1menu::ITrigger& trigger );

		/** @brief Sets all of the thresholds in the supplied trigger as tight as possible but still passing the supplied event.
		 *
		 * Note that this assumes all of the thresholds are independent. If they're not, behaviour is undefined. The supplied
		 * trigger is modified, so make a copy before hand if that's not what you want. Note only the thresholds are changed, any
		 * other parameters e.g. eta cuts are kept as is.
		 *
		 * If no thresholds can be found that would let the trigger pass the supplied event, a std::runtime_error is thrown.
		 *
		 * @param[in]  event      The event to test the trigger on.
		 * @param[out] trigger    The trigger to check and modify.
		 * @param[in]  tolerance  The trigger thresholds will be modified to be within this tolerance of thresholds that would
		 *                        fail the event.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 28/May/2013
		 */
		void setTriggerThresholdsAsTightAsPossible( const l1menu::L1TriggerDPGEvent& event, l1menu::ITrigger& trigger, float tolerance=0.01 );

		std::pair<float,float> calorimeterRegionEtaBounds( size_t calorimeterRegion );

		/** @brief Converts a value in absolute eta to the calorimeter region. */
		float convertEtaCutToRegionCut( float etaCut );

		/** @brief Converts a value in calorimeter region to absolute eta. */
		float convertRegionCutToEtaCut( float regionCut );

	} // end of the tools namespace
} // end of the l1menu namespace
#endif
