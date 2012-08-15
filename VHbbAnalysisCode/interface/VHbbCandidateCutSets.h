#ifndef trkupgradeanalysis_VHbbCandidateCutSets_h
#define trkupgradeanalysis_VHbbCandidateCutSets_h

#include <vector>

#include "TrkUpgradeAnalysis/VHbb/interface/ICutSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/IBasicVHbbCandidateCut.h"

// Forward declarations
class VHbbCandidate;
namespace trkupgradeanalysis
{
	namespace variables
	{
		class VHbbCandidateVariable;
	}
}



namespace trkupgradeanalysis
{
	class VHbbCandidateCutSet : public trkupgradeanalysis::ICutSet
	{
	public:
		VHbbCandidateCutSet();
		virtual ~VHbbCandidateCutSet();

		// These are methods from CutSet that are being implemented. Note that "name" is not being
		// implemented so this class is still abstract.
		size_t numberOfCuts() const;
		bool allCutsPassed() const;
		bool cutNPassed( size_t cutNumber ) const;
		bool everythingOtherThanCutNPassed( size_t cutNumber ) const;
		const trkupgradeanalysis::IBasicCut& cutAt( size_t a ) const;

		// Add a method to apply cuts on a VHbbCandidate
		virtual bool applyCuts( const VHbbCandidate& vhbbCandidate );
		// Add a method so that I'm able to use data previously stored in an NTuple
		virtual bool applyCuts( const trkupgradeanalysis::tools::NTupleRow& ntupleRow );
		virtual std::string name() const;

		// Add some methods to access the additional variables
		size_t numberOfAdditionalVariables();
		trkupgradeanalysis::IHistogramVariable& additionalVariableAt( size_t position );
	protected:
		std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*> basicCuts_;

		 /// @brief Other variables that are interesting to look at, but aren't cut on
		std::vector<trkupgradeanalysis::variables::VHbbCandidateVariable*> additionalVariables_;

		bool allCutsPassed_;
		std::vector<bool> cutsPassed_;
		std::string name_;
	};

	/** @brief Doesn't actually apply any cuts, but uses the class to create an NTuple of all the cut variables
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Jul/2012
	 */
	class AllVariables : public VHbbCandidateCutSet
	{
	public:
		AllVariables();
	};

	/** @brief Full selection with cut values taken as constructor arguments
	 *
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/Jul/2012
	 */
	class ParameterisedZmumu : public VHbbCandidateCutSet
	{
	public:
		ParameterisedZmumu( float higherCSVCut, float lowerCSVCut, float lowerZMassCut, float upperZMassCut, float lowerHMassCut, float upperHMassCut, bool requireTrigger=false, bool requireDoubleTrigger=false, int numberOfAdditionalJetsCut=-1 );
	};

	/** @brief Exactly the same as ParameterisedZmumu but provides some default values.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/Jul/2012
	 */
	class FullSelectionZmumu : public ParameterisedZmumu
	{
	public:
		FullSelectionZmumu( float higherCSVCut, float lowerCSVCut, float centralZMass, bool requireTrigger=false, bool requireDoubleTrigger=false );
	};

	/** @brief Full selection with cut values taken as constructor arguments
	 *
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 18/Jul/2012
	 */
	class ParameterisedZee : public VHbbCandidateCutSet
	{
	public:
		ParameterisedZee( float higherCSVCut, float lowerCSVCut, float lowerZMassCut, float upperZMassCut, float lowerHMassCut, float upperHMassCut, int numberOfAdditionalJetsCut=-1 );
	};

	/** @brief Full selection with cut values taken as constructor arguments
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/Jul/2012
	 */
	class FullSelectionZee : public VHbbCandidateCutSet
	{
	public:
		FullSelectionZee( float higherCSVCut, float lowerCSVCut, float centralZMass );
	};

	class SignalSelectionZee : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZee( float centralMass );
		SignalSelectionZee( float lowerMass, float upperMass );
	};

	class SignalSelectionZmumu : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZmumu( float centralMass );
		SignalSelectionZmumu( float lowerMass, float upperMass );
	};


	class SignalSelectionWen : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionWen( float mass );
	};

	class SignalSelectionWmun : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionWmun( float mass );
	};

	/** @brief Most of the cuts in SignalSelectionZmumu but a few of the cuts have been moved to additional variables.
	 *
	 * The following cuts have been changed to additional variables, i.e. they are not actually cut on
	 * but are recorded for study:
	 *    * di-jet mass cut
	 *    * higher CSV cut
	 *    * lower CSV cut
	 *    * number of additional jets
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Jun/2012
	 */
	class InitialSignalSelectionZmumu : public VHbbCandidateCutSet
	{
	public:
		InitialSignalSelectionZmumu();
	};

	/** @brief Most of the cuts in SignalSelectionZee but a few of the cuts have been moved to additional variables.
	 *
	 * The following cuts have been changed to additional variables, i.e. they are not actually cut on
	 * but are recorded for study:
	 *    * di-jet mass cut
	 *    * higher CSV cut
	 *    * lower CSV cut
	 *    * number of additional jets
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Jun/2012
	 */
	class InitialSignalSelectionZee : public VHbbCandidateCutSet
	{
	public:
		InitialSignalSelectionZee();
	};

	/** @brief All of the cuts in SignalSelectionZmumu except for the additonal jets less than 2 cut.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 30/Jan/2012
	 */
	class SignalSelectionZmumuWithoutAdditionalJetsCut : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZmumuWithoutAdditionalJetsCut( float mass );
	};

	/** @brief All of the cuts in SignalSelectionZmumu except that the order of the CSV cuts is switched.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 27/Feb/2012
	 */
	class SignalSelectionZmumuWithCSVCutsSwitched : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZmumuWithCSVCutsSwitched( float mass );
		SignalSelectionZmumuWithCSVCutsSwitched( float lowerMass, float upperMass );
	};

	/** @brief All of the cuts in SignalSelectionZmumu but with the CSV cuts changed for StdGeom at 50 pile up.
	 *
	 * The btag operating points have been changed from the official CSVT to CSVT at 50 pile up. Analysis of
	 * ttbar events at 50 pile up shows a 0.1% light mistag efficiency can be given by a discriminator of
	 * 0.717621. Instead of using the 0.5 discriminator used in the current analysis I've changed to using
	 * a 1% light mistag efficiency (i.e. CSVM).
	 *
	 * | *Discriminator* | *btag efficiency* | *c mistag efficiency* | *light mistag efficiency* |
	 * | 0.717621        | 0.393179          | 0.0465483             | 0.000999997               |
	 * | 0.395486        | 0.542436          | 0.125649              | 0.00999999                |
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 03/May/2012
	 * Updated for the most recent b-tagging performance 06/Jul/2012
	 */
	class SignalSelectionStdGeom50PUZmumu : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionStdGeom50PUZmumu( float mass );
		SignalSelectionStdGeom50PUZmumu( float lowerMass, float upperMass );
	};

	/** @brief All of the cuts in SignalSelectionZmumu but with the CSV cuts changed for Phase1 at 50 pile up.
	 *
	 * The higher CSV cut I've changed to give a light mistag efficiency of 0.1% (CSVT), and the lower one I've
	 * changed to give a light mistag efficiency of 1% (CSVM).
	 *
	 * | *Discriminator* | *btag efficiency* | *c mistag efficiency* | *light mistag efficiency* |
	 * | 0.776769        | 0.492322          | 0.0452924             | 0.000999999               |
	 * | 0.536104        | 0.644404          | 0.161445              | 0.01                      |
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 03/May/2012
	 * Updated for the most recent b-tagging performance 06/Jul/2012
	 */
	class SignalSelectionPhase150PUZmumu : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionPhase150PUZmumu( float mass );
		SignalSelectionPhase150PUZmumu( float lowerMass, float upperMass );
	};

	/** @brief All of the cuts in SignalSelectionStdGeom50PUZmumu but without the additional jets cut.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 03/May/2012
	 */
	class SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut( float mass );
		SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut( float lowerMass, float upperMass );
	};

	/** @brief All of the cuts in SignalSelectionPhase150PUZmumu but without the additional jets cut.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 03/May/2012
	 */
	class SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut( float mass );
		SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut( float lowerMass, float upperMass );
	};

	/** @brief Straight copy of the VlightRegionHWmun cut in the VHllbb analysis framework.
	 *
	 * Note that it doesn't cut on the trigger because that information is not stored in
	 * VHbbCandidate, which this class operates on.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 26/Nov/2011
	 */
	class VlightRegionHWmun : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHWmun();
	};


	class VlightRegionHWen : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHWen();
	};


	class VlightRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHZmumu();
	};


	class VlightRegionHZee : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHZee();
	};


	class TTbarRegionHWmun : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHWmun();
	};


	class TTbarRegionHWen : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHWen();
	};


	class TTbarRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHZmumu();
	};


	class TTbarRegionHZee : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHZee();
	};


	class VbbRegionHWmun : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHWmun();
	};


	class VbbRegionHWen : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHWen();
	};


	class VbbRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHZmumu();
	};


	class VbbRegionHZee : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHZee();
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_VHbbCandidateCutSets_h"

