#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"

#include <stdexcept>

#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCuts.h"
#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateVariables.h"
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

trkupgradeanalysis::VHbbCandidateCutSet::VHbbCandidateCutSet() :
	allCutsPassed_( false ), name_( "unset" )
{
	// No operation besides the initialiser list
}

trkupgradeanalysis::VHbbCandidateCutSet::~VHbbCandidateCutSet()
{
	for( std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*>::iterator iCut=basicCuts_.begin(); iCut != basicCuts_.end(); ++iCut )
	{
		delete *iCut;
	}

	for( std::vector<trkupgradeanalysis::variables::VHbbCandidateVariable*>::iterator iAdditionalVariable=additionalVariables_.begin(); iAdditionalVariable!=additionalVariables_.end(); ++iAdditionalVariable )
	{
		delete *iAdditionalVariable;
	}
}

size_t trkupgradeanalysis::VHbbCandidateCutSet::numberOfCuts() const
{
	return basicCuts_.size();
}

bool trkupgradeanalysis::VHbbCandidateCutSet::allCutsPassed() const
{
	return allCutsPassed_;
}

bool trkupgradeanalysis::VHbbCandidateCutSet::cutNPassed( size_t cutNumber ) const
{
	if( cutNumber >= cutsPassed_.size() ) throw std::runtime_error( "trkupgradeanalysis::VHbbCandidateCutSet::cutNPassed(cutNumber) - invalid cutNumber" );
	return cutsPassed_[cutNumber];
}

bool trkupgradeanalysis::VHbbCandidateCutSet::everythingOtherThanCutNPassed( size_t cutNumber ) const
{
	if( cutNumber >= cutsPassed_.size() ) throw std::runtime_error( "trkupgradeanalysis::VHbbCandidateCutSet::everythingOtherThanCutNPassed(cutNumber) - invalid cutNumber" );

	for( size_t a=0; a < cutsPassed_.size(); ++a )
	{
		if( a == cutNumber ) continue;
		if( cutsPassed_[a] == false ) return false;
	}
	// If control has made it this far then all cuts (bar possibly the one stated) have passed
	return true;
}

const trkupgradeanalysis::IBasicCut& trkupgradeanalysis::VHbbCandidateCutSet::cutAt( size_t a ) const
{
	return *basicCuts_[a];
}

bool trkupgradeanalysis::VHbbCandidateCutSet::applyCuts( const VHbbCandidate& vhbbCandidate )
{
	allCutsPassed_=true; // Start off assuming this is true then change it if not.
	std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*>::const_iterator iCut;
	std::vector<bool>::iterator iCutPassed;
	for( iCut=basicCuts_.begin(), iCutPassed=cutsPassed_.begin(); iCut != basicCuts_.end() && iCutPassed != cutsPassed_.end(); ++iCut, ++iCutPassed )
	{
		bool cutPassed=( *iCut)->applyCut( vhbbCandidate );
		*iCutPassed=cutPassed;
		if( cutPassed == false ) allCutsPassed_=false;
	}

	// Fill the additional variables
	for( std::vector<trkupgradeanalysis::variables::VHbbCandidateVariable*>::const_iterator iVariable=additionalVariables_.begin(); iVariable!=additionalVariables_.end(); ++iVariable )
	{
		(*iVariable)->set( vhbbCandidate );
	}

	return allCutsPassed_;
}

bool trkupgradeanalysis::VHbbCandidateCutSet::applyCuts( const trkupgradeanalysis::tools::NTupleRow& ntupleRow )
{
	allCutsPassed_=true; // Start off assuming this is true then change it if not.
	std::vector<trkupgradeanalysis::IBasicVHbbCandidateCut*>::const_iterator iCut;
	std::vector<bool>::iterator iCutPassed;
	for( iCut=basicCuts_.begin(), iCutPassed=cutsPassed_.begin(); iCut != basicCuts_.end() && iCutPassed != cutsPassed_.end(); ++iCut, ++iCutPassed )
	{
		bool cutPassed=( *iCut)->applyCut( ntupleRow );
		*iCutPassed=cutPassed;
		if( cutPassed == false ) allCutsPassed_=false;
	}

	// Fill the additional variables
	for( std::vector<trkupgradeanalysis::variables::VHbbCandidateVariable*>::const_iterator iVariable=additionalVariables_.begin(); iVariable!=additionalVariables_.end(); ++iVariable )
	{
		(*iVariable)->set( ntupleRow );
	}

	return allCutsPassed_;
}

std::string trkupgradeanalysis::VHbbCandidateCutSet::name() const
{
	return name_;
}

size_t trkupgradeanalysis::VHbbCandidateCutSet::numberOfAdditionalVariables()
{
	return additionalVariables_.size();
}

trkupgradeanalysis::IHistogramVariable& trkupgradeanalysis::VHbbCandidateCutSet::additionalVariableAt( size_t position )
{
	// Use the "at" method rather than array subscripts so that an exception is thrown if it's out of bounds
	return *additionalVariables_.at(position);
}



trkupgradeanalysis::AllVariables::AllVariables()
{
	additionalVariables_.push_back( new trkupgradeanalysis::variables::MuonPixelHits(true) ); // True means check first muon
	additionalVariables_.push_back( new trkupgradeanalysis::variables::MuonPixelHits(false) ); // False means check second muon
	additionalVariables_.push_back( new trkupgradeanalysis::variables::CandidateType );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiLeptonMass );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::HiggsCandidatePt );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::VectorBosonCandidatePt );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DeltaPhiVH );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::HighestCSV );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::LowestCSV );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );

	name_="AllVariables";
}

trkupgradeanalysis::ParameterisedZmumu::ParameterisedZmumu( float higherCSVCut, float lowerCSVCut, float lowerZMassCut, float upperZMassCut, float lowerHMassCut, float upperHMassCut, bool requireTrigger, bool requireDoubleTrigger, int numberOfAdditionalJetsCut )
{
	using namespace trkupgradeanalysis::cuts;
	using namespace trkupgradeanalysis::variables;

	if( requireTrigger || requireDoubleTrigger )
	{
		// Emulate HLT trigger requirements by requiring 3 or more pixel hits in the muons
		if( requireDoubleTrigger ) basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( LowestMuonPixelHits(), GreaterThan( 2 ) ) );
		else basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HighestMuonPixelHits(), GreaterThan( 2 ) ) );
	}

	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( CandidateType(), Equals( VHbbCandidate::Zmumu, 0.01 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DiLeptonMass(), Within( lowerZMassCut, upperZMassCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HiggsCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( VectorBosonCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HighestCSV(), GreaterThan( higherCSVCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( LowestCSV(), GreaterThan( lowerCSVCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DeltaPhiVH(), GreaterThan( 2.9 ) ) );

	if( numberOfAdditionalJetsCut>=0 ) basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( NumberOfAdditionalJets(), LessThanOrEqual(numberOfAdditionalJetsCut) ) );
	else additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );

	if( lowerHMassCut>0 && upperHMassCut> 0 ) basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DiJetMass(), Within( lowerHMassCut, upperHMassCut ) ) );
	else additionalVariables_.push_back( new DiJetMass );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "FullSelectionZmumu_higherCSVCut=" << higherCSVCut << "_lowerCSVCut=" << lowerCSVCut << "_lowerZMassCut=" << lowerZMassCut << "_upperZMassCut=" << upperZMassCut;
	if( requireTrigger || requireDoubleTrigger )
	{
		if( requireDoubleTrigger ) floatToStringConversion << "_withDoubleTrigger";
		else  floatToStringConversion << "_withSingleTrigger";
	}
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::FullSelectionZmumu::FullSelectionZmumu( float higherCSVCut, float lowerCSVCut, float centralZMass, bool requireTrigger, bool requireDoubleTrigger )
	: ParameterisedZmumu( higherCSVCut, lowerCSVCut, centralZMass-16, centralZMass+14, -1, -1, -1, requireTrigger, requireDoubleTrigger)
{
	// No operation besides the initialiser list
}

trkupgradeanalysis::ParameterisedZee::ParameterisedZee( float higherCSVCut, float lowerCSVCut, float lowerZMassCut, float upperZMassCut, float lowerHMassCut, float upperHMassCut, int numberOfAdditionalJetsCut )
{
	using namespace trkupgradeanalysis::cuts;
	using namespace trkupgradeanalysis::variables;
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( CandidateType(), Equals( VHbbCandidate::Zee, 0.01 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DiLeptonMass(), Within( lowerZMassCut, upperZMassCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HiggsCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( VectorBosonCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HighestCSV(), GreaterThan( higherCSVCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( LowestCSV(), GreaterThan( lowerCSVCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DeltaPhiVH(), GreaterThan( 2.9 ) ) );

	if( numberOfAdditionalJetsCut>=0 ) basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( NumberOfAdditionalJets(), LessThanOrEqual(numberOfAdditionalJetsCut) ) );
	else additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );

	if( lowerHMassCut>0 && upperHMassCut> 0 ) basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DiJetMass(), Within( lowerHMassCut, upperHMassCut ) ) );
	else additionalVariables_.push_back( new DiJetMass );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "FullSelectionZee_higherCSVCut=" << higherCSVCut << "_lowerCSVCut=" << lowerCSVCut << "_lowerZMassCut=" << lowerZMassCut << "_upperZMassCut=" << upperZMassCut;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::FullSelectionZee::FullSelectionZee( float higherCSVCut, float lowerCSVCut, float centralZMass )
{
	using namespace trkupgradeanalysis::cuts;
	using namespace trkupgradeanalysis::variables;
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( CandidateType(), Equals( VHbbCandidate::Zee, 0.01 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DiLeptonMass(), Within( centralZMass-16, centralZMass+14 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HiggsCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( VectorBosonCandidatePt(), GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( DeltaPhiVH(), GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( HighestCSV(), GreaterThan( higherCSVCut ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CutOnVariable( LowestCSV(), GreaterThan( lowerCSVCut ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new DiJetMass );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "FullSelectionZee_higherCSVCut=" << higherCSVCut << "_lowerCSVCut=" << lowerCSVCut << "_centralZMass=" << centralZMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionZee::SignalSelectionZee( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	// These lines are from the code
	//	result.add( new SignalPreSelectionZee );
	//	result.add( new HPtCut( 100 ) );
	//	result.add( new VPtCut( 100 ) );
	//	result.add( new DoubleBTagCut( 0.5 ) );
	//	result.add( new SingleBTagCut( CSVT ) );
	//	result.add( new AdditionalJetsCut( 2 ) ); // < 2
	//	result.add( new DiJetMassMinCut( centralMass - 15. - 5. ) );
	//	result.add( new DiJetMassMaxCut( centralMass + 15. - 5. ) );
	// These extra cuts are from the paper
	//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThanOrEqual( 105 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );


	// This first set aren't listed in the paper as separate cuts, so they must come under
	// the header "pre-selection".
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThanOrEqual( 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );

	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( GreaterThan( centralMass-20 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( LessThanOrEqual( centralMass+10 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );


	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZee_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionZee::SignalSelectionZee( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );


	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZee_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}


trkupgradeanalysis::SignalSelectionZmumu::SignalSelectionZmumu( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;

	// The lines below are how the cuts are applied in the code
	//	result.add( new SignalPreSelectionZee );    // Eh? Zee? I assume this is a bug
	//	result.add( new HPtCut( 100 ) );
	//	result.add( new VPtCut( 100 ) );
	//	result.add( new DoubleBTagCut( 0.5 ) );
	//	result.add( new SingleBTagCut( CSVT ) );
	//	result.add( new AdditionalJetsCut( 2 ) ); // < 2
	//	result.add( new DiJetMassMinCut( centralMass - 15. - 5. ) );
	//	result.add( new DiJetMassMaxCut( centralMass + 15. - 5. ) );
	// These lines I added from the cuts listed in the paper
	//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThanOrEqual( 105 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	//	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );


	// This first set aren't listed in the paper as separate cuts, so they must come under
	// the header "pre-selection".
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThanOrEqual( 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );

	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( GreaterThan( centralMass-20 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( LessThanOrEqual( centralMass+10 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZmumu_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionZmumu::SignalSelectionZmumu( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZmumu_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::InitialSignalSelectionZee::InitialSignalSelectionZee()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::HighestCSV );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::LowestCSV );

	name_="InitialSignalSelectionZee";
}

trkupgradeanalysis::InitialSignalSelectionZmumu::InitialSignalSelectionZmumu()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::HighestCSV );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::LowestCSV );

	name_="InitialSignalSelectionZmumu";
}

trkupgradeanalysis::SignalSelectionZmumuWithoutAdditionalJetsCut::SignalSelectionZmumuWithoutAdditionalJetsCut( float mass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );

	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( mass-20, mass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZmumuWithoutAdditionalJetsCut_mass=" << mass;
	name_=floatToStringConversion.str();
}


trkupgradeanalysis::SignalSelectionZmumuWithCSVCutsSwitched::SignalSelectionZmumuWithCSVCutsSwitched( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZmumuWithCSVCutsSwitched_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionZmumuWithCSVCutsSwitched::SignalSelectionZmumuWithCSVCutsSwitched( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionZmumuWithCSVCutsSwitched_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}



trkupgradeanalysis::SignalSelectionStdGeom50PUZmumu::SignalSelectionStdGeom50PUZmumu( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.717621, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.395486, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionStdGeom50PUZmumu_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionStdGeom50PUZmumu::SignalSelectionStdGeom50PUZmumu( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.717621, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.395486, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionStdGeom50PUZmumu_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}


trkupgradeanalysis::SignalSelectionPhase150PUZmumu::SignalSelectionPhase150PUZmumu( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.776769, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.536104, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionPhase150PUZmumu_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionPhase150PUZmumu::SignalSelectionPhase150PUZmumu( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.776769, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.536104, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionPhase150PUZmumu_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}



trkupgradeanalysis::SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut::SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.717621, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.395486, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut::SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.717621, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.395486, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}


trkupgradeanalysis::SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut::SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut( float centralMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.776769, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.536104, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( centralMass-20, centralMass+10 ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut_centralMass=" << centralMass;
	name_=floatToStringConversion.str();
}

trkupgradeanalysis::SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut::SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut( float lowerMass, float upperMass )
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.776769, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.536104, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::DeltaPhiVHCut( GreaterThan( 2.9 ) ) );
//	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( Within( lowerMass, upperMass ) ) );

	cutsPassed_.resize( basicCuts_.size() );

	additionalVariables_.push_back( new trkupgradeanalysis::variables::NumberOfAdditionalJets );
	additionalVariables_.push_back( new trkupgradeanalysis::variables::DiJetMass );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut_massBetween" << lowerMass << "And" << upperMass;
	name_=floatToStringConversion.str();
}


trkupgradeanalysis::SignalSelectionWen::SignalSelectionWen( float mass )
{
	using namespace trkupgradeanalysis::cuts;
//	result.add( new SignalPreSelectionWen );
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wen ) );
//	result.add( new HPtCut( 165 ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 165 ) ) );
//	result.add( new VPtCut( 160 ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 160 ) ) );
//	result.add( new DoubleBTagCut( 0.5 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
//	result.add( new SingleBTagCut( CSVT ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
//	result.add( new AdditionalJetsCut( 1 ) ); // < 1
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 1 ) ) );
//	result.add( new AdditionalLeptonsCut( 1 ) ); // < 1
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalLeptons( LessThan( 1 ) ) );
//	result.add( new METCut( 30 ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfMETN( 0, GreaterThan( 30 ) ) );
//	result.add( new DiJetMassMinCut( mass - 15. ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( GreaterThan( mass-15 ) ) );
//	result.add( new DiJetMassMaxCut( mass + 15. ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( LessThanOrEqual( mass+15 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionWen_mass=" << mass;
	name_=floatToStringConversion.str();
}




trkupgradeanalysis::SignalSelectionWmun::SignalSelectionWmun( float mass )
{
	using namespace trkupgradeanalysis::cuts;
//	result.add( new SignalPreSelectionWmun );
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wmun ) );
//	result.add( new HPtCut( 165 ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 165 ) ) );
//	result.add( new VPtCut( 160 ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 160 ) ) );
//	result.add( new DoubleBTagCut( 0.5 ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAllJetsGreaterThan( 0.5, 2 ) );
//	result.add( new SingleBTagCut( CSVT ) );
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
//	result.add( new AdditionalJetsCut( 1 ) ); // < 1
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 1 ) ) );
//	result.add( new AdditionalLeptonsCut( 1 ) ); // < 1
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalLeptons( LessThan( 1 ) ) );
//	result.add( new DiJetMassMinCut( mass - 15. ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( GreaterThan( mass-15 ) ) );
//	result.add( new DiJetMassMaxCut( mass + 15. ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfHiggsBoson( LessThanOrEqual( mass+15 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	std::stringstream floatToStringConversion;
	floatToStringConversion << "SignalSelectionWmun_mass=" << mass;
	name_=floatToStringConversion.str();
}




trkupgradeanalysis::VlightRegionHWmun::VlightRegionHWmun()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wmun ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::METSigma( GreaterThan( 2.5 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VlightRegionHWmun";
}

trkupgradeanalysis::VlightRegionHWen::VlightRegionHWen()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wen ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfElectronN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::METSigma( GreaterThan( 2.5 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VlightRegionHWen";
}

trkupgradeanalysis::VlightRegionHZmumu::VlightRegionHZmumu()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThan( 105 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VlightRegionHZmumu";
}

trkupgradeanalysis::VlightRegionHZee::VlightRegionHZee()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThan( 105 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VlightRegionHZee";
}

trkupgradeanalysis::TTbarRegionHWmun::TTbarRegionHWmun()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wmun ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	// require that at least one of the first two jets (jets 0 and 1) b-tag is greater than 0.898
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( GreaterThan( 1 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="TTbarRegionHWmun";
}

trkupgradeanalysis::TTbarRegionHWen::TTbarRegionHWen()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wen ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfElectronN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( GreaterThan( 100 ) ) );
	// require that at least one of the first two jets (jets 0 and 1) b-tag is greater than 0.898
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( GreaterThan( 1 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="TTbarRegionHWen";
}

trkupgradeanalysis::TTbarRegionHZmumu::TTbarRegionHZmumu()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfMETObjects( GreaterThan( 0 ) ) );
	// require that the pT of the 0th (i.e. the first) MET object is greater than 50 GeV
	basicCuts_.push_back( new trkupgradeanalysis::PtOfMETN( 0, GreaterThan( 50 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 120 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="TTbarRegionHZmumu";
}

trkupgradeanalysis::TTbarRegionHZee::TTbarRegionHZee()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfMETObjects( GreaterThan( 0 ) ) );
	// require that the pT of the 0th (i.e. the first) MET object is greater than 50 GeV
	basicCuts_.push_back( new trkupgradeanalysis::PtOfMETN( 0, GreaterThan( 50 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 120 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="TTbarRegionHZee";
}

trkupgradeanalysis::VbbRegionHWmun::VbbRegionHWmun()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wmun ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( LessThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( LessThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( LessThan( 150 ) ) );
	// Add V.Mt(VHbbCandidate::Wmun) < 120
	// Add V.Mt(VHbbCandidate::Wmun) > 40
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( Equals( 0 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::METSigma( GreaterThan( 2.5 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VbbRegionHWmun";
}

trkupgradeanalysis::VbbRegionHWen::VbbRegionHWen()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Wen ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfElectronN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 30 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( LessThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfHiggs( LessThan( 150 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfVectorBoson( LessThan( 150 ) ) );
	// Add V.Mt(VHbbCandidate::Wmun) < 120
	// Add V.Mt(VHbbCandidate::Wmun) > 40
	// require that at least one of the first two jets (jets 0 and 1) b-tag is greater than 0.898
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( Equals( 0 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::METSigma( GreaterThan( 2.5 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VbbRegionHWen";
}

trkupgradeanalysis::VbbRegionHZmumu::VbbRegionHZmumu()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	// Add ( H.p4.M() < 90 ||  H.p4.M() > 145)
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	// Add TMath::Abs( Geom::deltaPhi(H.p4.Phi(), V.p4.Phi()) ) > 2.9
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThan( 105 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VbbRegionHZmumu";
}

trkupgradeanalysis::VbbRegionHZee::VbbRegionHZee()
{
	using namespace trkupgradeanalysis::cuts;
	basicCuts_.push_back( new trkupgradeanalysis::CandidateTypeEquals( VHbbCandidate::Zee ) );
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfJets( GreaterThanOrEqual( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 0, GreaterThan( 20 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::PtOfJetN( 1, GreaterThan( 20 ) ) );
	// Add ( H.p4.M() < 90 ||  H.p4.M() > 145)
	basicCuts_.push_back( new trkupgradeanalysis::CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	// Add TMath::Abs( Geom::deltaPhi(H.p4.Phi(), V.p4.Phi()) ) > 2.9
	basicCuts_.push_back( new trkupgradeanalysis::NumberOfAdditionalJetsCut( LessThan( 2 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( GreaterThan( 75 ) ) );
	basicCuts_.push_back( new trkupgradeanalysis::MassOfVectorBoson( LessThan( 105 ) ) );
	cutsPassed_.resize( basicCuts_.size() );

	name_="VbbRegionHZee";
}
