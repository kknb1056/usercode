#ifndef trkupgradeanalysis_VHbbCandidateCutSets_h
#define trkupgradeanalysis_VHbbCandidateCutSets_h

#include "TrkUpgradeAnalysis/VHbb/interface/ICutSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/IBasicVHbbCandidateCut.h"

// Forward declarations
class VHbbCandidate;

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
		virtual std::string name() const;
	protected:
		std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*> basicCuts_;
		bool allCutsPassed_;
		std::vector<bool> cutsPassed_;
		std::string name_;
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

