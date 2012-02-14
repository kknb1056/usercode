#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCuts.h"

// Required to work out deltaPhi
#include "DataFormats/GeometryVector/interface/VectorUtil.h"


trkupgradeanalysis::CandidateTypeEquals::CandidateTypeEquals( VHbbCandidate::CandidateType candidateType )
	: requiredCandidateType_(candidateType)
{
	// No operation besides the initialiser list
}

trkupgradeanalysis::CandidateTypeEquals::~CandidateTypeEquals()
{
	// No operation
}

std::string trkupgradeanalysis::CandidateTypeEquals::name() const
{
	std::stringstream nameStream;
	nameStream << "CandidateType==";
	if( requiredCandidateType_==VHbbCandidate::Zmumu ) nameStream << "Zmumu";
	else if( requiredCandidateType_==VHbbCandidate::Zee ) nameStream << "Zee";
	else if( requiredCandidateType_==VHbbCandidate::Wmun ) nameStream << "Wmun";
	else if( requiredCandidateType_==VHbbCandidate::Wen ) nameStream << "Wen";
	else if( requiredCandidateType_==VHbbCandidate::Znn ) nameStream << "Znn";
	else if( requiredCandidateType_==VHbbCandidate::UNKNOWN ) nameStream << "UNKNOWN";
	return nameStream.str();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::CandidateTypeEquals::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::CandidateTypeEquals::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.candidateType;
	return lastValue_==requiredCandidateType_;
}

std::string trkupgradeanalysis::CandidateTypeEquals::variableName() const
{
	return "CandidateType";
}

double trkupgradeanalysis::CandidateTypeEquals::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::CandidateTypeEquals::suggestedNumberOfBins() const
{
	return 6;
}

double trkupgradeanalysis::CandidateTypeEquals::suggestedLowerEdge() const
{
	return -0.5;
}

double trkupgradeanalysis::CandidateTypeEquals::suggestedUpperEdge() const
{
	return 5.5;
}





trkupgradeanalysis::NumberOfJets::NumberOfJets( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::NumberOfJets::~NumberOfJets()
{
	// No operation
}

std::string trkupgradeanalysis::NumberOfJets::name() const
{
	return variableName()+pCut_->name();}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::NumberOfJets::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::NumberOfJets::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.H.jets.size();
	return pCut_->apply(lastValue_);
}

std::string trkupgradeanalysis::NumberOfJets::variableName() const { return "numberOfJets"; }
double trkupgradeanalysis::NumberOfJets::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::NumberOfJets::suggestedNumberOfBins() const { return 6; }
double trkupgradeanalysis::NumberOfJets::suggestedLowerEdge() const { return -0.5; }
double trkupgradeanalysis::NumberOfJets::suggestedUpperEdge() const { return 5.5; }





trkupgradeanalysis::PtOfJetN::PtOfJetN( unsigned int jetNumber, const trkupgradeanalysis::cuts::ICutType& cut )
	: jetNumber_(jetNumber), pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::PtOfJetN::~PtOfJetN()
{
	// No operation
}

std::string trkupgradeanalysis::PtOfJetN::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::PtOfJetN::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::PtOfJetN::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	if( vhbbCandidate.H.jets.size()<jetNumber_+1 ) lastValue_=-1; // Plus one because they're numbered from zero
	else lastValue_=const_cast<VHbbCandidate&>(vhbbCandidate).H.jets.at(jetNumber_).Pt(); // const cast needed because this method isn't declared const for some reason

	return pCut_->apply(lastValue_);
}

std::string trkupgradeanalysis::PtOfJetN::variableName() const
{
	std::stringstream nameStream;
	nameStream << "PtOfJet" << jetNumber_;
	return nameStream.str();
}

double trkupgradeanalysis::PtOfJetN::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::PtOfJetN::suggestedNumberOfBins() const { return 60; }
double trkupgradeanalysis::PtOfJetN::suggestedLowerEdge() const { return 0; }
double trkupgradeanalysis::PtOfJetN::suggestedUpperEdge() const { return 150; }






trkupgradeanalysis::PtOfHiggs::PtOfHiggs( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::PtOfHiggs::~PtOfHiggs()
{
	// No operation
}

std::string trkupgradeanalysis::PtOfHiggs::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::PtOfHiggs::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::PtOfHiggs::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.H.p4.Pt();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::PtOfHiggs::variableName() const { return "PtOfHiggs"; }
double trkupgradeanalysis::PtOfHiggs::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::PtOfHiggs::suggestedNumberOfBins() const { return 60; }
double trkupgradeanalysis::PtOfHiggs::suggestedLowerEdge() const { return 0; }
double trkupgradeanalysis::PtOfHiggs::suggestedUpperEdge() const { return 250; }





trkupgradeanalysis::PtOfVectorBoson::PtOfVectorBoson( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::PtOfVectorBoson::~PtOfVectorBoson()
{
	// No operation
}

std::string trkupgradeanalysis::PtOfVectorBoson::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::PtOfVectorBoson::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::PtOfVectorBoson::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.V.p4.Pt();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::PtOfVectorBoson::variableName() const
{
	return "PtOfVectorBoson";
}

double trkupgradeanalysis::PtOfVectorBoson::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::PtOfVectorBoson::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::PtOfVectorBoson::suggestedLowerEdge() const
{
	return 0;
}

double trkupgradeanalysis::PtOfVectorBoson::suggestedUpperEdge() const
{
	return 250;
}






trkupgradeanalysis::NumberOfAdditionalJets::NumberOfAdditionalJets( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::NumberOfAdditionalJets::~NumberOfAdditionalJets()
{
	// No operation
}

std::string trkupgradeanalysis::NumberOfAdditionalJets::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::NumberOfAdditionalJets::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::NumberOfAdditionalJets::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=0;
	for( std::vector<VHbbEvent::SimpleJet>::const_iterator iJet=vhbbCandidate.additionalJets.begin(); iJet!=vhbbCandidate.additionalJets.end(); ++iJet )
	{
		if( jetId( *iJet )==true && fabs(iJet->p4.Eta()) < 2.5 && iJet->p4.Pt() > 20 ) ++lastValue_;
	}
	return pCut_->apply( lastValue_ );
}

bool trkupgradeanalysis::NumberOfAdditionalJets::jetId( const VHbbEvent::SimpleJet& jet ) const
{
	if( jet.neutralHadronEFraction > 0.99 ) return false;
	if( jet.neutralEmEFraction > 0.99 ) return false;
	if( jet.nConstituents <= 1 ) return false;
	if( fabs(jet.p4.Eta()) < 2.5 )
	{
		if( jet.chargedEmEFraction > 0.99 ) return false;
		if( jet.chargedHadronEFraction == 0 ) return false;
		if( jet.ntracks== 0 ) return false;
	}

	return true;
}

std::string trkupgradeanalysis::NumberOfAdditionalJets::variableName() const
{
	return "NumberOfAdditionalJets";
}

double trkupgradeanalysis::NumberOfAdditionalJets::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::NumberOfAdditionalJets::suggestedNumberOfBins() const
{
	return 91;
}

double trkupgradeanalysis::NumberOfAdditionalJets::suggestedLowerEdge() const
{
	return -0.5;
}

double trkupgradeanalysis::NumberOfAdditionalJets::suggestedUpperEdge() const
{
	return 90.5;
}






trkupgradeanalysis::NumberOfAdditionalLeptons::NumberOfAdditionalLeptons( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::NumberOfAdditionalLeptons::~NumberOfAdditionalLeptons()
{
	// No operation
}

std::string trkupgradeanalysis::NumberOfAdditionalLeptons::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::NumberOfAdditionalLeptons::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::NumberOfAdditionalLeptons::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	int expectedLeptons=0;
	if( vhbbCandidate.candidateType == VHbbCandidate::Wmun || vhbbCandidate.candidateType == VHbbCandidate::Wen ) expectedLeptons=1;
	if( vhbbCandidate.candidateType == VHbbCandidate::Zmumu || vhbbCandidate.candidateType == VHbbCandidate::Zee ) expectedLeptons=2;

	lastValue_=vhbbCandidate.V.muons.size()+vhbbCandidate.V.electrons.size()-expectedLeptons;
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::NumberOfAdditionalLeptons::variableName() const
{
	return "NumberOfAdditionalLeptons";
}

double trkupgradeanalysis::NumberOfAdditionalLeptons::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::NumberOfAdditionalLeptons::suggestedNumberOfBins() const
{
	return 31;
}

double trkupgradeanalysis::NumberOfAdditionalLeptons::suggestedLowerEdge() const
{
	return -0.5;
}

double trkupgradeanalysis::NumberOfAdditionalLeptons::suggestedUpperEdge() const
{
	return 30.5;
}








trkupgradeanalysis::METSigma::METSigma( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::METSigma::~METSigma()
{
	// No operation
}

std::string trkupgradeanalysis::METSigma::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::METSigma::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::METSigma::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	if( vhbbCandidate.V.mets.empty() ) lastValue_=-1;
	else lastValue_=vhbbCandidate.V.mets[0].metSig;

	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::METSigma::variableName() const
{
	return "METSigma";
}

double trkupgradeanalysis::METSigma::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::METSigma::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::METSigma::suggestedLowerEdge() const
{
	return 0;
}

double trkupgradeanalysis::METSigma::suggestedUpperEdge() const
{
	return 100;
}







trkupgradeanalysis::PtOfElectronN::PtOfElectronN( unsigned int electronNumber, const trkupgradeanalysis::cuts::ICutType& cut )
	: electronNumber_(electronNumber), pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::PtOfElectronN::~PtOfElectronN()
{
	// No operation
}

std::string trkupgradeanalysis::PtOfElectronN::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::PtOfElectronN::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::PtOfElectronN::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	if( vhbbCandidate.V.electrons.size()<electronNumber_+1 ) lastValue_=-1; // Plus one because they're numbered from zero
	else lastValue_=vhbbCandidate.V.electrons.at(electronNumber_).p4.Pt();

	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::PtOfElectronN::variableName() const
{
	std::stringstream nameStream;
	nameStream << "PtOfElectron" << electronNumber_;
	return nameStream.str();
}

double trkupgradeanalysis::PtOfElectronN::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::PtOfElectronN::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::PtOfElectronN::suggestedLowerEdge() const
{
	return 0;
}

double trkupgradeanalysis::PtOfElectronN::suggestedUpperEdge() const
{
	return 150;
}







trkupgradeanalysis::MassOfVectorBoson::MassOfVectorBoson( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::MassOfVectorBoson::~MassOfVectorBoson()
{
	// No operation
}

std::string trkupgradeanalysis::MassOfVectorBoson::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::MassOfVectorBoson::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::MassOfVectorBoson::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.V.p4.M();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::MassOfVectorBoson::variableName() const
{
	return "MassOfVectorBoson";
}

double trkupgradeanalysis::MassOfVectorBoson::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::MassOfVectorBoson::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::MassOfVectorBoson::suggestedLowerEdge() const
{
	return 0;
}

double trkupgradeanalysis::MassOfVectorBoson::suggestedUpperEdge() const
{
	return 250;
}







trkupgradeanalysis::MassOfHiggsBoson::MassOfHiggsBoson( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::MassOfHiggsBoson::~MassOfHiggsBoson()
{
	// No operation
}

std::string trkupgradeanalysis::MassOfHiggsBoson::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::MassOfHiggsBoson::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::MassOfHiggsBoson::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.H.p4.M();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::MassOfHiggsBoson::variableName() const
{
	return "MassOfHiggsBoson";
}

double trkupgradeanalysis::MassOfHiggsBoson::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::MassOfHiggsBoson::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::MassOfHiggsBoson::suggestedLowerEdge() const
{
	return 0;
}

double trkupgradeanalysis::MassOfHiggsBoson::suggestedUpperEdge() const
{
	return 250;
}







trkupgradeanalysis::CSVOfAnyJetGreaterThan::CSVOfAnyJetGreaterThan( double csvValue, unsigned int maximumJetNumber )
	: maximumJetNumber_(maximumJetNumber), requiredCSV_(csvValue)
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::CSVOfAnyJetGreaterThan::~CSVOfAnyJetGreaterThan()
{
	// No operation
}

std::string trkupgradeanalysis::CSVOfAnyJetGreaterThan::name() const
{
	std::stringstream nameStream;
	nameStream << variableName() << "GreaterThan" << requiredCSV_;
	return nameStream.str();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::CSVOfAnyJetGreaterThan::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::CSVOfAnyJetGreaterThan::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	unsigned int numberOfJetsToCheck;
	if( maximumJetNumber_==0 ) numberOfJetsToCheck=vhbbCandidate.H.jets.size();
	else numberOfJetsToCheck=maximumJetNumber_;

	lastValue_=-1; // Start off with a nonsensical default so I know if things go wrong
	for( unsigned int a=0; a<numberOfJetsToCheck && a<vhbbCandidate.H.jets.size(); ++a )
	{
		if( vhbbCandidate.H.jets.at(a).csv > lastValue_ ) lastValue_=vhbbCandidate.H.jets.at(a).csv;
	}

	return lastValue_ > requiredCSV_;
}

std::string trkupgradeanalysis::CSVOfAnyJetGreaterThan::variableName() const
{
	std::stringstream nameStream;
	nameStream << "CSVOf";

	if( maximumJetNumber_==0 ) nameStream << "AnyJet";
	else nameStream << "AnyOfFirst" << maximumJetNumber_ << "Jets";

	return nameStream.str();
}

double trkupgradeanalysis::CSVOfAnyJetGreaterThan::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::CSVOfAnyJetGreaterThan::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::CSVOfAnyJetGreaterThan::suggestedLowerEdge() const
{
	return -1;
}

double trkupgradeanalysis::CSVOfAnyJetGreaterThan::suggestedUpperEdge() const
{
	return 1.5;
}







trkupgradeanalysis::CSVOfAllJetsGreaterThan::CSVOfAllJetsGreaterThan( double csvValue, unsigned int maximumJetNumber )
	: maximumJetNumber_(maximumJetNumber), requiredCSV_(csvValue)
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::CSVOfAllJetsGreaterThan::~CSVOfAllJetsGreaterThan()
{
	// No operation
}

std::string trkupgradeanalysis::CSVOfAllJetsGreaterThan::name() const
{
	std::stringstream nameStream;
	nameStream << variableName() << "GreaterThan" << requiredCSV_;
	return nameStream.str();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::CSVOfAllJetsGreaterThan::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::CSVOfAllJetsGreaterThan::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	unsigned int numberOfJetsToCheck;
	if( maximumJetNumber_==0 ) numberOfJetsToCheck=vhbbCandidate.H.jets.size();
	else numberOfJetsToCheck=maximumJetNumber_;

	lastValue_=-1; // Start off with a nonsensical default so I know if things go wrong
	for( unsigned int a=0; a<numberOfJetsToCheck && a<vhbbCandidate.H.jets.size(); ++a )
	{
		if( (vhbbCandidate.H.jets.at(a).csv<lastValue_) || lastValue_==-1 ) lastValue_=vhbbCandidate.H.jets.at(a).csv;
	}

	return lastValue_ > requiredCSV_;
}

std::string trkupgradeanalysis::CSVOfAllJetsGreaterThan::variableName() const
{
	std::stringstream nameStream;
	nameStream << "CSVOf";

	if( maximumJetNumber_==0 ) nameStream << "AllJets";
	else nameStream << "AllOfFirst" << maximumJetNumber_ << "Jets";

	return nameStream.str();
}

double trkupgradeanalysis::CSVOfAllJetsGreaterThan::histogrammableValue() const
{
	return lastValue_;
}

size_t trkupgradeanalysis::CSVOfAllJetsGreaterThan::suggestedNumberOfBins() const
{
	return 60;
}

double trkupgradeanalysis::CSVOfAllJetsGreaterThan::suggestedLowerEdge() const
{
	return -1;
}

double trkupgradeanalysis::CSVOfAllJetsGreaterThan::suggestedUpperEdge() const
{
	return 1.5;
}






trkupgradeanalysis::NumberOfMETObjects::NumberOfMETObjects( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::NumberOfMETObjects::~NumberOfMETObjects()
{
	// No operation
}

std::string trkupgradeanalysis::NumberOfMETObjects::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::NumberOfMETObjects::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::NumberOfMETObjects::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=vhbbCandidate.V.mets.size();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::NumberOfMETObjects::variableName() const { return "numberOfMETObjects"; }
double trkupgradeanalysis::NumberOfMETObjects::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::NumberOfMETObjects::suggestedNumberOfBins() const { return 6; }
double trkupgradeanalysis::NumberOfMETObjects::suggestedLowerEdge() const { return -0.5; }
double trkupgradeanalysis::NumberOfMETObjects::suggestedUpperEdge() const { return 5.5; }









trkupgradeanalysis::PtOfMETN::PtOfMETN( unsigned int metNumber, const trkupgradeanalysis::cuts::ICutType& cut )
	: metNumber_(metNumber), pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::PtOfMETN::~PtOfMETN()
{
	// No operation
}

std::string trkupgradeanalysis::PtOfMETN::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::PtOfMETN::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::PtOfMETN::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	if( vhbbCandidate.V.mets.size()<metNumber_+1 ) // Plus one because they're numbered from zero
	{
		lastValue_=-1;
		return false;
	}

	lastValue_=vhbbCandidate.V.mets.at(metNumber_).p4.Pt();
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::PtOfMETN::variableName() const
{
	std::stringstream nameStream;
	nameStream << "PtOfMET" << metNumber_;
	return nameStream.str();
}

double trkupgradeanalysis::PtOfMETN::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::PtOfMETN::suggestedNumberOfBins() const { return 60; }
double trkupgradeanalysis::PtOfMETN::suggestedLowerEdge() const { return 0; }
double trkupgradeanalysis::PtOfMETN::suggestedUpperEdge() const { return 150; }







trkupgradeanalysis::DeltaPhiVH::DeltaPhiVH( const trkupgradeanalysis::cuts::ICutType& cut )
	: pCut_( cut.copy() )
{
	// No operation apart from the initialiser list
}

trkupgradeanalysis::DeltaPhiVH::~DeltaPhiVH()
{
	// No operation
}

std::string trkupgradeanalysis::DeltaPhiVH::name() const
{
	return variableName()+pCut_->name();
}

const trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::DeltaPhiVH::cutVariable() const
{
	// This class implements the HistogramVariable interface to save on the
	// number of classes, so just return a reference to this instance.
	return *this;
}

bool trkupgradeanalysis::DeltaPhiVH::applyCut( const VHbbCandidate& vhbbCandidate ) const
{
	lastValue_=TMath::Abs( Geom::deltaPhi( vhbbCandidate.H.p4.Phi(), vhbbCandidate.V.p4.Phi() ) );
	return pCut_->apply( lastValue_ );
}

std::string trkupgradeanalysis::DeltaPhiVH::variableName() const { return "DeltaPhiVH";}
double trkupgradeanalysis::DeltaPhiVH::histogrammableValue() const { return lastValue_; }
size_t trkupgradeanalysis::DeltaPhiVH::suggestedNumberOfBins() const { return 60; }
double trkupgradeanalysis::DeltaPhiVH::suggestedLowerEdge() const { return 0; }
double trkupgradeanalysis::DeltaPhiVH::suggestedUpperEdge() const { return M_PI; }
