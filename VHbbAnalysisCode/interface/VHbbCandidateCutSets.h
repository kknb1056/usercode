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
	protected:
		std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*> basicCuts_;
		bool allCutsPassed_;
		std::vector<bool> cutsPassed_;
	};

	class SignalSelectionZee : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZee( float mass );
		std::string name() const;
	};

	class SignalSelectionZmumu : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionZmumu( float mass );
		std::string name() const;
	};


	class SignalSelectionWen : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionWen( float mass );
		std::string name() const;
	};

	class SignalSelectionWmun : public VHbbCandidateCutSet
	{
	public:
		SignalSelectionWmun( float mass );
		std::string name() const;
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
		std::string name() const;
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
		std::string name() const;
	};


	class VlightRegionHWen : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHWen();
		std::string name() const;
	};


	class VlightRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHZmumu();
		std::string name() const;
	};


	class VlightRegionHZee : public VHbbCandidateCutSet
	{
	public:
		VlightRegionHZee();
		std::string name() const;
	};


	class TTbarRegionHWmun : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHWmun();
		std::string name() const;
	};


	class TTbarRegionHWen : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHWen();
		std::string name() const;
	};


	class TTbarRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHZmumu();
		std::string name() const;
	};


	class TTbarRegionHZee : public VHbbCandidateCutSet
	{
	public:
		TTbarRegionHZee();
		std::string name() const;
	};


	class VbbRegionHWmun : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHWmun();
		std::string name() const;
	};


	class VbbRegionHWen : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHWen();
		std::string name() const;
	};


	class VbbRegionHZmumu : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHZmumu();
		std::string name() const;
	};


	class VbbRegionHZee : public VHbbCandidateCutSet
	{
	public:
		VbbRegionHZee();
		std::string name() const;
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_VHbbCandidateCutSets_h"

