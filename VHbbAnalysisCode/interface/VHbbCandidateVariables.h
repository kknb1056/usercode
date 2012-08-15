#ifndef trkupgradeanalysis_VHbbCandidateVariables_h
#define trkupgradeanalysis_VHbbCandidateVariables_h

#include "TrkUpgradeAnalysis/VHbb/interface/IHistogramVariable.h"
#include <string>
#include <memory> // required for auto_ptr

// Can't forward declare this because I need declarations of the constituent classes
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEvent.h"
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

// Forward declarations
class VHbbCandidate;
namespace trkupgradeanalysis
{
	namespace tools
	{
		class NTupleRow;
	}
}


namespace trkupgradeanalysis
{
	namespace variables
	{
		/** @brief Abstract base class for implementations of IHistogramVariable that can set themselves from VHbbCandidates.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class VHbbCandidateVariable : public trkupgradeanalysis::IHistogramVariable
		{
		public:
			VHbbCandidateVariable( const std::string& name, size_t suggestedNumberOfBins, double suggestedLowerEdge, double suggestedUpperEdge );
			virtual ~VHbbCandidateVariable();

			//
			// The implementations required by the IHistogramVariable interface.
			// variableName and histogrammableValue are implemented here, the subclass
			// should implement the suggestedNumberOfBins, suggestedLowerEdge and suggestedUpperEdge
			// methods
			//
			virtual std::string variableName() const;
			virtual double histogrammableValue() const;
			virtual size_t suggestedNumberOfBins() const;
			virtual double suggestedLowerEdge() const;
			virtual double suggestedUpperEdge() const;

			/** Method so that implementations can set their values from a VHbbCandidate.
			 * Implementations can override this and use it to set the value_ member for this
			 * base class to do everything else. */
			virtual void set( const VHbbCandidate& vhbbCandidate ) = 0;

			/** Method to set the variable from a previously saved TTree.*/
			virtual void set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow );

			/** Method to easily make copies through the base interface. */
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const = 0;
		protected:
			double value_;
			std::string name_;
			size_t suggestedNumberOfBins_;
			double suggestedLowerEdge_;
			double suggestedUpperEdge_;
		};


		/** @brief Class that calculates the number of additional jets from a VHbbCandidate.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class NumberOfAdditionalJets : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			NumberOfAdditionalJets( bool applyCleaning=true );
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			bool applyCleaning_;
			bool jetId( const VHbbEvent::SimpleJet& jet ) const;
		};



		/** @brief Class that calculates the di-jet mass from a VHbbCandidate.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class DiJetMass : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			DiJetMass();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};



		/** @brief Class that finds the highest CSV tag of a jet collection from a VHbbCandidate.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class HighestCSV : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			/** @brief Constructor that optionally takes a limit on how many jets to check.
			 *
			 * The number of jets to check can be set with "maximumJetNumber". E.g. if this is set to 1 then
			 * only the first jet is checked, if set to 2 the first two jets are checked etcetera. If it is set
			 * to 0 (default) then all jets are checked.
			 */
			HighestCSV( size_t maximumJetNumber=0 );
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			size_t maximumJetNumber_;
		};



		/** @brief Class that finds the lowest CSV tag of a jet collection from a VHbbCandidate.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class LowestCSV : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			/** @brief Constructor that optionally takes a limit on how many jets to check.
			 *
			 * The number of jets to check can be set with "maximumJetNumber". E.g. if this is set to 1 then
			 * only the first jet is checked, if set to 2 the first two jets are checked etcetera. If it is set
			 * to 0 (default) then all jets are checked.
			 */
			LowestCSV( size_t maximumJetNumber=0 );
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			size_t maximumJetNumber_;
		};



		/** @brief Class that gets the number of used jets (i.e. not including additional jets) from a VHbbCandidate.
		 *
		 * One would assume this should always be 2, but there's no harm in checking is there?
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jun/2012
		 */
		class NumberOfJets : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			NumberOfJets();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};



		/** @brief Class that gets the number of pixel hits in one of the muons in the di-muon pair.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 04/Jul/2012
		 */
		class MuonPixelHits : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			/** @brief Constructor where you set whether to look at the first muon or the second.
			 * @parameter checkFirstMuon If this is true the first muon is checked, if false the second is checked.
			 */
			MuonPixelHits( bool checkFirstMuon );
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			bool checkFirstMuon_;
			bool throwExceptions_;
		};



		/** @brief Like MuonPixelHits, but gives the highest value of the two muons.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 04/Jul/2012
		 */
		class HighestMuonPixelHits : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			HighestMuonPixelHits();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual void set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ); ///< Override so that I can check MuonPixelHits as well as HighestMuonPixelHits
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			bool throwExceptions_;
		};



		/** @brief Like MuonPixelHits, but gives the lowest value of the two muons.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 04/Jul/2012
		 */
		class LowestMuonPixelHits : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			LowestMuonPixelHits();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual void set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ); ///< Override so that I can check MuonPixelHits as well as HighestMuonPixelHits
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		protected:
			bool throwExceptions_;
		};



		/** @brief Class
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jul/2012
		 */
		class CandidateType : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			CandidateType();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};

		/** @brief Class
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jul/2012
		 */
		class DiLeptonMass : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			DiLeptonMass();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};

		/** @brief Class
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jul/2012
		 */
		class HiggsCandidatePt : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			HiggsCandidatePt();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};

		/** @brief Class
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jul/2012
		 */
		class VectorBosonCandidatePt : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			VectorBosonCandidatePt();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};

		/** @brief Class
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 06/Jul/2012
		 */
		class DeltaPhiVH : public trkupgradeanalysis::variables::VHbbCandidateVariable
		{
		public:
			DeltaPhiVH();
			virtual void set( const VHbbCandidate& vhbbCandidate );
			virtual std::auto_ptr<VHbbCandidateVariable> copy() const;
		};

	} // end of namespace variables
} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_VHbbCandidateVariables_h"
