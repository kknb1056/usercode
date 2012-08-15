#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateVariables.h"

#include <sstream>
#include <stdexcept>
#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"
#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for trkupgradeanalysis::tools::NTupleRow

// Required to work out deltaPhi
#include "DataFormats/GeometryVector/interface/VectorUtil.h"


trkupgradeanalysis::variables::VHbbCandidateVariable::VHbbCandidateVariable( const std::string& name, size_t suggestedNumberOfBins, double suggestedLowerEdge, double suggestedUpperEdge ) :
	name_(name), suggestedNumberOfBins_(suggestedNumberOfBins), suggestedLowerEdge_(suggestedLowerEdge), suggestedUpperEdge_(suggestedUpperEdge)
{
	// No operation besides the initialiser list
}

trkupgradeanalysis::variables::VHbbCandidateVariable::~VHbbCandidateVariable()
{
	// No operation
}

std::string trkupgradeanalysis::variables::VHbbCandidateVariable::variableName() const
{
	return name_;
}

double trkupgradeanalysis::variables::VHbbCandidateVariable::histogrammableValue() const
{
	return value_;
}

size_t trkupgradeanalysis::variables::VHbbCandidateVariable::suggestedNumberOfBins() const
{
	return suggestedNumberOfBins_;
}

double trkupgradeanalysis::variables::VHbbCandidateVariable::suggestedLowerEdge() const
{
	return suggestedLowerEdge_;
}

double trkupgradeanalysis::variables::VHbbCandidateVariable::suggestedUpperEdge() const
{
	return suggestedUpperEdge_;
}

void trkupgradeanalysis::variables::VHbbCandidateVariable::set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow )
{
	// The default implementation assumes the variable was saved with a branch name of the variable name
	value_=ntupleRow.getDouble(name_);
}





trkupgradeanalysis::variables::NumberOfAdditionalJets::NumberOfAdditionalJets( bool applyCleaning ) :
	VHbbCandidateVariable("NumberOfAdditionalJets",91,-0.5,90.5), applyCleaning_(applyCleaning)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::NumberOfAdditionalJets::set( const VHbbCandidate& vhbbCandidate )
{
	if( !applyCleaning_ ) value_=vhbbCandidate.additionalJets.size();
	else
	{
		value_=0;
		for( std::vector<VHbbEvent::SimpleJet>::const_iterator iJet=vhbbCandidate.additionalJets.begin(); iJet!=vhbbCandidate.additionalJets.end(); ++iJet )
		{
			if( jetId( *iJet )==true && fabs(iJet->p4.Eta()) < 2.5 && iJet->p4.Pt() > 20 ) ++value_;
		}
	}

}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::NumberOfAdditionalJets::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::NumberOfAdditionalJets(*this) );
}

bool trkupgradeanalysis::variables::NumberOfAdditionalJets::jetId( const VHbbEvent::SimpleJet& jet ) const
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







trkupgradeanalysis::variables::DiJetMass::DiJetMass() :
	VHbbCandidateVariable("DiJetMass",60,0,250)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::DiJetMass::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.H.p4.M();
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::DiJetMass::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::DiJetMass(*this) );
}







trkupgradeanalysis::variables::HighestCSV::HighestCSV( size_t maximumJetNumber ) :
	VHbbCandidateVariable("HighestCSV",60,-1,1.5), maximumJetNumber_(maximumJetNumber)
{
	// Overwrite the name with more information
	std::stringstream nameStream;
	nameStream << "HighestCSVOf";

	if( maximumJetNumber_==0 ) nameStream << "AllJets";
	else nameStream << "First" << maximumJetNumber_ << "Jets";

	name_=nameStream.str();
}

void trkupgradeanalysis::variables::HighestCSV::set( const VHbbCandidate& vhbbCandidate )
{
	unsigned int numberOfJetsToCheck=maximumJetNumber_;
	if( maximumJetNumber_==0 || numberOfJetsToCheck>vhbbCandidate.H.jets.size() ) numberOfJetsToCheck=vhbbCandidate.H.jets.size();

	value_=-1; // Start off with a nonsensical default so I know if things go wrong
	for( unsigned int a=0; a<numberOfJetsToCheck; ++a )
	{
		if( vhbbCandidate.H.jets.at(a).csv > value_ ) value_=vhbbCandidate.H.jets.at(a).csv;
	}

}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::HighestCSV::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::HighestCSV(*this) );
}







trkupgradeanalysis::variables::LowestCSV::LowestCSV( size_t maximumJetNumber ) :
	VHbbCandidateVariable("LowestCSV",60,-1,1.5), maximumJetNumber_(maximumJetNumber)
{
	// Overwrite the name with more information
	std::stringstream nameStream;
	nameStream << "LowestCSVOf";

	if( maximumJetNumber_==0 ) nameStream << "AllJets";
	else nameStream << "First" << maximumJetNumber_ << "Jets";

	name_=nameStream.str();
}

void trkupgradeanalysis::variables::LowestCSV::set( const VHbbCandidate& vhbbCandidate )
{
	unsigned int numberOfJetsToCheck=maximumJetNumber_;
	if( maximumJetNumber_==0 || numberOfJetsToCheck>vhbbCandidate.H.jets.size() ) numberOfJetsToCheck=vhbbCandidate.H.jets.size();

	value_=-1; // Start off with a nonsensical default so I know if things go wrong
	for( unsigned int a=0; a<numberOfJetsToCheck; ++a )
	{
		if( (vhbbCandidate.H.jets.at(a).csv<value_) || value_==-1 ) value_=vhbbCandidate.H.jets.at(a).csv;
	}

}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::LowestCSV::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::LowestCSV(*this) );
}







trkupgradeanalysis::variables::NumberOfJets::NumberOfJets() :
	VHbbCandidateVariable("NumberOfJets",6,-0.5,5.5)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::NumberOfJets::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.H.jets.size();
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::NumberOfJets::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::NumberOfJets(*this) );
}







trkupgradeanalysis::variables::MuonPixelHits::MuonPixelHits( bool checkFirstMuon ) :
	VHbbCandidateVariable("MuonPixelHits",7,-1.5,5.5), checkFirstMuon_(checkFirstMuon), throwExceptions_(false)
{
	// Need to add which muon I'm checking to the name
	if( checkFirstMuon_ ) name_="MuonPixelHitsOfFirstMuon";
	else name_="MuonPixelHitsOfSecondMuon";
}

// TODO - Get rid of this once fully debugged.
#include <iostream>
void trkupgradeanalysis::variables::MuonPixelHits::set( const VHbbCandidate& vhbbCandidate )
{
	value_=-1; // Set to an invalid value in case of error

	if( vhbbCandidate.V.muons.size()<2 )
	{
		if( throwExceptions_ ) throw std::runtime_error( "trkupgradeanalysis::variables::MuonPixelHits::set(..) - There are fewer than 2 muons");
		else
		{
//			std::cerr << "trkupgradeanalysis::variables::MuonPixelHits::set(..) - There are fewer than 2 muons" << std::endl;
			return;
		}
	}

	if( checkFirstMuon_ )
	{
		value_=vhbbCandidate.V.muons[0].nPixelHits;
//		if( vhbbCandidate.V.firstLepton<vhbbCandidate.V.muons.size() ) value_=vhbbCandidate.V.muons[vhbbCandidate.V.firstLepton].nPixelHits;
//		else std::cerr << "trkupgradeanalysis::variables::MuonPixelHits::set(..) - vhbbCandidate.V.firstLepton=" << vhbbCandidate.V.firstLepton
//				<< " (candidateType=" << vhbbCandidate.candidateType << ")"
//				<< "[" << vhbbCandidate.V.muons.size() << "," << vhbbCandidate.V.firstLepton << "," << vhbbCandidate.V.secondLepton << "]" << std::endl;
	}
	else
	{
		value_=vhbbCandidate.V.muons[1].nPixelHits;
//		if( vhbbCandidate.V.secondLepton<vhbbCandidate.V.muons.size() ) value_=vhbbCandidate.V.muons[vhbbCandidate.V.secondLepton].nPixelHits;
//		else std::cerr << "trkupgradeanalysis::variables::MuonPixelHits::set(..) - vhbbCandidate.V.secondLepton=" << vhbbCandidate.V.secondLepton
//				<< " (candidateType=" << vhbbCandidate.candidateType << ")"
//				<< "[" << vhbbCandidate.V.muons.size() << "," << vhbbCandidate.V.firstLepton << "," << vhbbCandidate.V.secondLepton << "]" << std::endl;
	}
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::MuonPixelHits::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::MuonPixelHits(*this) );
}






trkupgradeanalysis::variables::HighestMuonPixelHits::HighestMuonPixelHits() :
	VHbbCandidateVariable("HighestMuonPixelHits",7,-1.5,5.5), throwExceptions_(false)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::HighestMuonPixelHits::set( const VHbbCandidate& vhbbCandidate )
{
	value_=-1; // Set to an invalid value in case of error

	if( vhbbCandidate.V.muons.size()<2 || vhbbCandidate.V.firstLepton>vhbbCandidate.V.muons.size() || vhbbCandidate.V.secondLepton>vhbbCandidate.V.muons.size() )
	{
		if( throwExceptions_ ) throw std::runtime_error( "trkupgradeanalysis::variables::HighestMuonPixelHits::set(..) - There are not enough muons");
		else
		{
//			std::cerr << "trkupgradeanalysis::variables::HighestMuonPixelHits::set(..) - There are fewer than 2 muons" << std::endl;
			return;
		}
	}

	int firstMuon=vhbbCandidate.V.muons[vhbbCandidate.V.firstLepton].nPixelHits;
	int secondMuon=vhbbCandidate.V.muons[vhbbCandidate.V.secondLepton].nPixelHits;
	firstMuon > secondMuon ? value_=firstMuon : value_=secondMuon;
}


void trkupgradeanalysis::variables::HighestMuonPixelHits::set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow )
{
	try // Try the default behaviour, if that doesn't work check for MuonPixelHitsOfFirstMuon and MuonPixelHitsOfSecondMuon
	{
		value_=ntupleRow.getDouble(name_);
	}
	catch( std::exception& error )
	{
		double firstMuon=ntupleRow.getDouble("MuonPixelHitsOfFirstMuon");
		double secondMuon=ntupleRow.getDouble("MuonPixelHitsOfSecondMuon");
		firstMuon > secondMuon ? value_=firstMuon : value_=secondMuon;
	}
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::HighestMuonPixelHits::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::HighestMuonPixelHits(*this) );
}






trkupgradeanalysis::variables::LowestMuonPixelHits::LowestMuonPixelHits() :
	VHbbCandidateVariable("LowestMuonPixelHits",7,-1.5,5.5), throwExceptions_(false)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::LowestMuonPixelHits::set( const VHbbCandidate& vhbbCandidate )
{
	value_=-1; // Set to an invalid value in case of error

	if( vhbbCandidate.V.muons.size()<2 || vhbbCandidate.V.firstLepton>vhbbCandidate.V.muons.size() || vhbbCandidate.V.secondLepton>vhbbCandidate.V.muons.size() )
	{
		if( throwExceptions_ ) throw std::runtime_error( "trkupgradeanalysis::variables::LowestMuonPixelHits::set(..) - There are not enough muons");
		else
		{
//			std::cerr << "trkupgradeanalysis::variables::LowestMuonPixelHits::set(..) - There are fewer than 2 muons" << std::endl;
			return;
		}
	}

	int firstMuon=vhbbCandidate.V.muons[vhbbCandidate.V.firstLepton].nPixelHits;
	int secondMuon=vhbbCandidate.V.muons[vhbbCandidate.V.secondLepton].nPixelHits;
	firstMuon < secondMuon ? value_=firstMuon : value_=secondMuon;
}


void trkupgradeanalysis::variables::LowestMuonPixelHits::set( const trkupgradeanalysis::tools::NTupleRow& ntupleRow )
{
	try // Try the default behaviour, if that doesn't work check for MuonPixelHitsOfFirstMuon and MuonPixelHitsOfSecondMuon
	{
		value_=ntupleRow.getDouble(name_);
	}
	catch( std::exception& error )
	{
		double firstMuon=ntupleRow.getDouble("MuonPixelHitsOfFirstMuon");
		double secondMuon=ntupleRow.getDouble("MuonPixelHitsOfSecondMuon");
		firstMuon < secondMuon ? value_=firstMuon : value_=secondMuon;
	}
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::LowestMuonPixelHits::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::LowestMuonPixelHits(*this) );
}







trkupgradeanalysis::variables::CandidateType::CandidateType() :
	VHbbCandidateVariable("CandidateType",6,-0.5,5.5)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::CandidateType::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.candidateType;
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::CandidateType::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::CandidateType(*this) );
}







trkupgradeanalysis::variables::DiLeptonMass::DiLeptonMass() :
	VHbbCandidateVariable("DiLeptonMass",60,0,250)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::DiLeptonMass::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.V.p4.M();
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::DiLeptonMass::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::DiLeptonMass(*this) );
}







trkupgradeanalysis::variables::HiggsCandidatePt::HiggsCandidatePt() :
	VHbbCandidateVariable("HiggsCandidatePt",60,0,250)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::HiggsCandidatePt::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.H.p4.Pt();;
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::HiggsCandidatePt::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::HiggsCandidatePt(*this) );
}







trkupgradeanalysis::variables::VectorBosonCandidatePt::VectorBosonCandidatePt() :
	VHbbCandidateVariable("VectorBosonCandidatePt",60,0,250)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::VectorBosonCandidatePt::set( const VHbbCandidate& vhbbCandidate )
{
	value_=vhbbCandidate.V.p4.Pt();
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::VectorBosonCandidatePt::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::VectorBosonCandidatePt(*this) );
}







trkupgradeanalysis::variables::DeltaPhiVH::DeltaPhiVH() :
	VHbbCandidateVariable("DeltaPhiVH",60,0,M_PI)
{
	// No operation besides the initialiser list
}

void trkupgradeanalysis::variables::DeltaPhiVH::set( const VHbbCandidate& vhbbCandidate )
{
	value_=TMath::Abs( Geom::deltaPhi( vhbbCandidate.H.p4.Phi(), vhbbCandidate.V.p4.Phi() ) );
}

std::auto_ptr<trkupgradeanalysis::variables::VHbbCandidateVariable> trkupgradeanalysis::variables::DeltaPhiVH::copy() const
{
	return std::auto_ptr<VHbbCandidateVariable>( new trkupgradeanalysis::variables::DeltaPhiVH(*this) );
}
