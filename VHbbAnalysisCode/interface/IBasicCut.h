#ifndef trkupgradeanalysis_IBasicCut_h
#define trkupgradeanalysis_IBasicCut_h

// Forward declarations
namespace trkupgradeanalysis
{
	class IHistogramVariable;
}

namespace trkupgradeanalysis
{
	/** @brief Abstract base class for cuts.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 26/Nov/2011
	 */
	class IBasicCut
	{
	public:
		virtual ~IBasicCut() {}

		/// @brief The name of the cut
		virtual std::string name() const = 0;

		/// @brief Returns a HistogramVariable instance with the value from the last call of applyCut
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const = 0;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_IBasicCut_h"
