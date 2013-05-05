#include "l1menu/triggers/DoubleJetCentral.h"

#include "l1menu/RegisterTriggerMacro.h"

#include <stdexcept>
#include "L1AnalysisDataFormat.h"

namespace l1menu
{
	namespace triggers
	{
		REGISTER_TRIGGER( DoubleJetCentral_v0 );
	}
}

bool l1menu::triggers::DoubleJetCentral_v0::apply( const L1Analysis::L1AnalysisDataFormat& event ) const
{
//	bool raw=PhysicsBits[0];  // ZeroBias
//	if( !raw ) return false;

	int n1=0;
	int n2=0;
	int Nj=event.Njet;
	for( int ue=0; ue<Nj; ue++ )
	{
		int bx=event.Bxjet[ue];
		if( bx!=0 ) continue;
		bool isFwdJet=event.Fwdjet[ue];
		if( isFwdJet ) continue;
		bool isTauJet=event.Taujet[ue];
		if( isTauJet ) continue;
		float rank=event.Etjet[ue];
		float pt=rank; //CorrectedL1JetPtByGCTregions(event.Etajet[ue],rank*4.,theL1JetCorrection);
		if( pt>=threshold1_ ) n1++;
		if( pt>=threshold2_ ) n2++;
	}
	bool ok=(n1>=1&&n2>=2);

	return ok;
}

unsigned int l1menu::triggers::DoubleJetCentral_v0::version() const
{
	return 0;
}

l1menu::triggers::DoubleJetCentral::DoubleJetCentral()
	: threshold1_(20), threshold2_(20)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::DoubleJetCentral::name() const
{
	return "L1_DoubleJet"; // I don't know why this doesn't have "Central" in the name
}

const std::vector<std::string> l1menu::triggers::DoubleJetCentral::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("threshold2");
	return returnValue;
}

float& l1menu::triggers::DoubleJetCentral::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::DoubleJetCentral::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else throw std::logic_error( "Not a valid parameter name" );
}
