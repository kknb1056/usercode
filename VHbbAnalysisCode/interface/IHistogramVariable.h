#ifndef trkupgradeanalysis_IHistogramVariable_h
#define trkupgradeanalysis_IHistogramVariable_h

#include <string>

namespace trkupgradeanalysis
{
	/** @brief Abstract base class for cuts that return true if a given VHbbCandidate passes, false otherwise.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 24/Nov/2011
	 */
	class IHistogramVariable
	{
	public:
		virtual ~IHistogramVariable() {}

		//* @brief The name of the variable
		virtual std::string variableName() const = 0;

		//* @brief Returns the current value of the variable.
		virtual double histogrammableValue() const = 0;

		//* @brief The suggested number of histogram bins to use when plotting this cut variable. Not always implemented by the sub class.
		virtual size_t suggestedNumberOfBins() const { return 1; }

		//* @brief The suggested lower histogram edge to use when plotting this cut variable. Not always implemented by the sub class.
		virtual double suggestedLowerEdge() const { return -0.5; }

		//* @brief The suggested upper histogram edge to use when plotting this cut variable. Not always implemented by the sub class.
		virtual double suggestedUpperEdge() const { return 0.5; }
	};


} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_IHistogramVariable_h"
