#include "TrkUpgradeAnalysis/VHbb/interface/AdditionalJetStudyCutSets.h"

#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCuts.h"

trkupgradeanalysis::additionaljetstudy::WilkenSlide2::WilkenSlide2()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	std::vector<VHbbCandidate::CandidateType> allowedCandidateTypes;
	allowedCandidateTypes.push_back( VHbbCandidate::Zmumu );
	allowedCandidateTypes.push_back( VHbbCandidate::Zee );

	basicCuts_.push_back( new CandidateTypeEquals( allowedCandidateTypes ) );

	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), false ) ); // The "false" means don't apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide2::name() const
{
	return "WilkenSlide2";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot1::WilkenSlide3Plot1()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), false ) ); // The "false" means don't apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot1::name() const
{
	return "WilkenSlide3Plot1";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot2::WilkenSlide3Plot2()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	// Not sure if this should be applied. It's not explicitly specified.
	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );

	basicCuts_.push_back( new CSVOfAllJetsGreaterThan( -1, 2 ) ); // CSV of both jets greater than -1
	basicCuts_.push_back( new MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), false ) ); // The "false" means don't apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot2::name() const
{
	return "WilkenSlide3Plot2";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot3::WilkenSlide3Plot3()
{
	double mass=120;
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	// These aren't explicitly specified, but I'm pretty sure she means to use
	// the cuts for the previous two plots as well.
	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );

	basicCuts_.push_back( new MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new CSVOfAnyJetGreaterThan( 0.898, 2 ) );
	basicCuts_.push_back( new CSVOfAllJetsGreaterThan( 0.5, 2 ) );
	basicCuts_.push_back( new DeltaPhiVH( GreaterThan( 2.9 ) ) );
	basicCuts_.push_back( new MassOfHiggsBoson( Within( mass-20, mass+10 ) ) ); // N.B. Rachel's requirement was 95<Mass<125, but using a 115GeV Higgs.
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), false ) ); // The "false" means don't apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide3Plot3::name() const
{
	return "WilkenSlide3Plot3";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide4Plot2::WilkenSlide4Plot2()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	// Not sure if this should be applied. It's not explicitly specified.
	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );

	basicCuts_.push_back( new CSVOfAllJetsGreaterThan( -1, 2 ) ); // CSV of both jets greater than -1
	basicCuts_.push_back( new MassOfVectorBoson( Within( 70, 110 ) ) );
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), false ) ); // The "false" means don't apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide4Plot2::name() const
{
	return "WilkenSlide4Plot2";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot1::WilkenSlide5Plot1()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), true ) ); // The "true" means apply cleaning.
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot1::name() const
{
	return "WilkenSlide5Plot1";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot2::WilkenSlide5Plot2()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new CSVOfAllJetsGreaterThan( 0.244, 2 ) );
	basicCuts_.push_back( new MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new PtOfHiggs( LessThan( 100 ) ) ); // Not sure if this is a typo on the slide and should be GreaterThan
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), true ) );
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot2::name() const
{
	return "WilkenSlide5Plot2";
}







trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot2AssumingTypo::WilkenSlide5Plot2AssumingTypo()
{
	using namespace trkupgradeanalysis;
	using namespace trkupgradeanalysis::cuts;

	basicCuts_.push_back( new CandidateTypeEquals( VHbbCandidate::Zmumu ) );
	basicCuts_.push_back( new CSVOfAllJetsGreaterThan( 0.244, 2 ) );
	basicCuts_.push_back( new MassOfVectorBoson( Within( 75, 105 ) ) );
	basicCuts_.push_back( new PtOfVectorBoson( GreaterThan( 100 ) ) );
	basicCuts_.push_back( new PtOfHiggs( GreaterThan( 100 ) ) ); // I think this is the typo on the slide
	basicCuts_.push_back( new NumberOfAdditionalJets( AlwaysPasses(), true ) );
	cutsPassed_.resize( basicCuts_.size() );
}

std::string trkupgradeanalysis::additionaljetstudy::WilkenSlide5Plot2AssumingTypo::name() const
{
	return "WilkenSlide5Plot2AssumingTypo";
}
