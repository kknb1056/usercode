#ifndef l1menu_TriggerRatePlot_h
#define l1menu_TriggerRatePlot_h

#include <memory>
#include <string>
#include <vector>

//
// Forward declarations
//
class TH1;
namespace l1menu
{
	class IEvent;
	class ITrigger;
	class ISample;
}


namespace l1menu
{
	/** @brief Class that plots trigger rates versus threshold.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 24/May/2013
	 */
	class TriggerRatePlot
	{
	public:
		/** @brief Constructor that sets the trigger to plot for, the histogram to plot into, and the trigger parameter to vary.
		 *
		 * Note that the title of the histogram is modified to record the state of the trigger (all other parameters).
		 *
		 * @param[in] trigger           The trigger that the plot will be created for. The trigger should be set exactly as you want
		 *                              the plots made for it, e.g. with the correct eta cuts already set. The other thresholds are
		 *                              not modified so you should set these to zero beforehand if that's what you want. The trigger
		 *                              is copied so these cannot be changed afterwards.
		 * @param[in] pHistogram        The root histogram to fill with the rates. This should be set up with the required binning
		 *                              beforehand. The title is changed to record the trigger name, version, parameter plotted against
		 *                              and other parameter values.
		 * @param[in] versusParameter   The trigger parameter to plot along the x-axis. Defaults to "threshold1".
		 * @param[in] scaledParameters  A list of parameters that should be scaled along with the versusParameter. The ratio between each
		 *                              parameter and the versusParameter at the time of construction is kept as the versusParameter is
		 *                              changed. A check is made to ensure the versusParameter is taken out of the list if it is present.
		 */
		TriggerRatePlot( const l1menu::ITrigger& trigger, std::unique_ptr<TH1> pHistogram, const std::string& versusParameter="threshold1", const std::vector<std::string> scaledParameters=std::vector<std::string>() );
		TriggerRatePlot( l1menu::TriggerRatePlot& otherTriggerRatePlot ) = delete;
		TriggerRatePlot& operator=( l1menu::TriggerRatePlot& otherTriggerRatePlot ) = delete;

		/** @brief Explicit rvalue constructor to allow moving of TriggerRatePlot objects. */
		TriggerRatePlot( l1menu::TriggerRatePlot&& otherTriggerRatePlot ) noexcept;
		/** @brief Explicit rvalue assignment operator to allow moving of TriggerRatePlot objects. */
		TriggerRatePlot& operator=( l1menu::TriggerRatePlot&& otherTriggerRatePlot ) noexcept;

		virtual ~TriggerRatePlot();

		void addEvent( const l1menu::IEvent& event );
		void addSample( const l1menu::ISample& sample );

		/** @brief Returns the trigger being used to create the plot. */
		const l1menu::ITrigger& getTrigger() const;

		/** @brief Returns the internal pointer to the root histogram. Ownership is retained by TriggerRatePlot. */
		TH1* getPlot();

		/** @brief Tells TriggerRatePlot to relinquish ownership of the internal root histogram. A pointer to the
		 * histogram is also returned.
		 *
		 * Note that the pointer is still held so that further operations on this object are still valid,
		 * just that the TH1 won't be deleted when the instance goes out of scope. */
		TH1* relinquishOwnershipOfPlot();
	protected:
		std::unique_ptr<l1menu::ITrigger> pTrigger_;
		std::unique_ptr<TH1> pHistogram_;
		/// The parameter to plot against, usually "threshold1";
		std::string versusParameter_;
		/// Pointer to the versusParameter_ reference in pTrigger_ to avoid expensive string comparison look ups
		float* pParameter_;
		/// A vector of pointers of any other parameters that should be scaled, and what the scaling should be.
		std::vector< std::pair<float*,float> > otherParameterScalings_;
		/// Flag to say whether the histogram should be deleted when this instance goes out of scope.
		bool histogramOwnedByMe_;
	};
}
#endif
