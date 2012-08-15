#ifndef trkupgradeanalysis_VHbbCandidateCuts_h
#define trkupgradeanalysis_VHbbCandidateCuts_h

#include <sstream>
#include <memory>

#include "TrkUpgradeAnalysis/VHbb/interface/IBasicVHbbCandidateCut.h"
#include "TrkUpgradeAnalysis/VHbb/interface/IHistogramVariable.h"
#include "TrkUpgradeAnalysis/VHbb/interface/CutTypes.h"

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

// Forward declarations
namespace trkupgradeanalysis
{
	namespace variables
	{
		class VHbbCandidateVariable;
	}
}


namespace trkupgradeanalysis
{
	/** @brief Cut that takes a HistogramVariable and a CutType to create a cut
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 07/Jul/2012
	 */
	class CutOnVariable : public trkupgradeanalysis::IBasicVHbbCandidateCut
	{
	public:
		CutOnVariable( const trkupgradeanalysis::variables::VHbbCandidateVariable& variable, const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~CutOnVariable();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> pVHbbCandidateVariable_;
	};

	class CandidateTypeEquals : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		CandidateTypeEquals( VHbbCandidate::CandidateType candidateType );
		CandidateTypeEquals( const std::vector<VHbbCandidate::CandidateType>& candidateTypes );
		virtual ~CandidateTypeEquals();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::vector<VHbbCandidate::CandidateType> requiredCandidateTypes_;
		mutable VHbbCandidate::CandidateType lastValue_;
	};

	class NumberOfJets : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		NumberOfJets( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~NumberOfJets();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable unsigned int lastValue_;
	};

	class PtOfJetN : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		PtOfJetN( unsigned int jetNumber, const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~PtOfJetN();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		unsigned int jetNumber_;
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class PtOfHiggs : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		PtOfHiggs( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~PtOfHiggs();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class PtOfVectorBoson : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		PtOfVectorBoson( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~PtOfVectorBoson();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class NumberOfAdditionalJetsCut : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		NumberOfAdditionalJetsCut( const trkupgradeanalysis::cuts::ICutType& cut, bool applyCleaning=true );
		virtual ~NumberOfAdditionalJetsCut();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable unsigned int lastValue_;
		bool jetId( const VHbbEvent::SimpleJet& jet ) const;
		bool applyCleaning_;
	};

	class NumberOfAdditionalLeptons : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		NumberOfAdditionalLeptons( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~NumberOfAdditionalLeptons();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable unsigned int lastValue_;
	};

	class METSigma : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		METSigma( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~METSigma();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class PtOfElectronN : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		PtOfElectronN( unsigned int electronNumber, const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~PtOfElectronN();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		unsigned int electronNumber_;
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class MassOfVectorBoson : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		MassOfVectorBoson( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~MassOfVectorBoson();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class MassOfHiggsBoson : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		MassOfHiggsBoson( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~MassOfHiggsBoson();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	/** @brief Requires that at least one of the jets has a CSV b-tag value greater than the cut given.
	 *
	 * The value that is queried is
	 * vhbbCandidate.H.jets[jetNumber].csv
	 *
	 * Note that histogrammableValue() returns the highest CSV of the jets checked.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Dec/2011
	 */
	class CSVOfAnyJetGreaterThan : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		/** @brief Constructor that takes the CSV value and optionally a limit on how many jets to check.
		 *
		 * The number of jets to check can be set with "maximumJetNumber". E.g. if this is set to 1 then
		 * only the first jet is checked, if set to 2 the first two jets are checked etcetera. If it is set
		 * to 0 (default) then all jets are checked.
		 */
		CSVOfAnyJetGreaterThan( double csvValue, unsigned int maximumJetNumber=0 );
		virtual ~CSVOfAnyJetGreaterThan();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const; ///< @brief Returns the highest CSV of the jets checked
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		unsigned int maximumJetNumber_;
		double requiredCSV_;
		mutable double lastValue_;
	};

	/** @brief Requires that all of the jets have a CSV b-tag value greater than the cut given.
	 *
	 * The value that is queried is
	 * vhbbCandidate.H.jets[jetNumber].csv
	 *
	 * Note that histogrammableValue() returns the lowest CSV of the jets checked.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Dec/2011
	 */
	class CSVOfAllJetsGreaterThan : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		/** @brief Constructor that takes the CSV value and optionally a limit on how many jets to check.
		 *
		 * The number of jets to check can be set with "maximumJetNumber". E.g. if this is set to 1 then
		 * only the first jet is checked, if set to 2 the first two jets are checked etcetera. If it is set
		 * to 0 (default) then all jets are checked.
		 */
		CSVOfAllJetsGreaterThan( double csvValue, unsigned int maximumJetNumber=0 );
		virtual ~CSVOfAllJetsGreaterThan();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const; ///< @brief Returns the lowest CSV of the jets checked
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		unsigned int maximumJetNumber_;
		double requiredCSV_;
		mutable double lastValue_;
	};

	/** @brief Requires that there are more MET info objects than the given cut
	 *
	 * Note that this has nothing to do with the amount of missing transverse energy, just
	 * that there is the given number of objects. More of a software cut than a physics one.
	 * To be honest I think the cut is only in the original framework to stop a segmentation
	 * fault when the amount of MET is checked.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Dec/2011
	 */
	class NumberOfMETObjects : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		NumberOfMETObjects( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~NumberOfMETObjects();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable unsigned int lastValue_;
	};

	/** @brief Checks the MET object requested to see if the pT is greater than the given cut.
	 *
	 * MET objects are numbered from 0. Defaults to checking the first MET object (number 0) if
	 * no object is specified in the constructor.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 06/Dec/2011
	 */
	class PtOfMETN : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		PtOfMETN( unsigned int metNumber, const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~PtOfMETN();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		unsigned int metNumber_;
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};

	class DeltaPhiVHCut : public trkupgradeanalysis::IBasicVHbbCandidateCut, public trkupgradeanalysis::IHistogramVariable
	{
	public:
		DeltaPhiVHCut( const trkupgradeanalysis::cuts::ICutType& cut );
		virtual ~DeltaPhiVHCut();

		// The methods from BasicCut
		virtual std::string name() const;
		virtual const trkupgradeanalysis::IHistogramVariable& cutVariable() const;

		// The method from BasicVHbbCandidateCut
		virtual bool applyCut( const VHbbCandidate& vhbbCandidate ) const;
		virtual bool applyCut( const trkupgradeanalysis::tools::NTupleRow& ntupleRow ) const;

		// The methods from HistogramVariable I need to implement
		std::string variableName() const;
		double histogrammableValue() const;
		size_t suggestedNumberOfBins() const;
		double suggestedLowerEdge() const;
		double suggestedUpperEdge() const;
	private:
		std::auto_ptr<trkupgradeanalysis::cuts::ICutType> pCut_;
		mutable double lastValue_;
	};


} // end of namespace trkupgradeanalysis



#endif // end of "#ifndef trkupgradeanalysis_VHbbCandidateCuts_h"
