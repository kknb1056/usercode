#include "l1menu/triggers/DoubleMu.h"

#include "l1menu/RegisterTriggerMacro.h"

#include <stdexcept>
#include "L1AnalysisDataFormat.h"

namespace l1menu
{
	namespace triggers
	{
		REGISTER_TRIGGER( DoubleMu_v0 );
	}
}

bool l1menu::triggers::DoubleMu_v0::apply( const L1Analysis::L1AnalysisDataFormat& event ) const
{
//	bool raw=PhysicsBits[0];   // ZeroBias
//	if( !raw ) return false;

	int n1=0;
	int n2=0;
	int Nmu=event.Nmu;
	for( int imu=0; imu<Nmu; imu++ )
	{
		int bx=event.Bxmu.at( imu );
		if( bx!=0 ) continue;
		float pt=event.Ptmu.at( imu );
		//float eta=event.Etamu.at( imu ); // Commented out to stop unused variable compile warning
		int qual=event.Qualmu.at( imu );
		if( qual<muonQuality_ ) continue;

		if( pt>=threshold1_ ) n1++;
		if( pt>=threshold2_ ) n2++;
	}

	bool ok=( n1>=1 && n2>=2 );
	return ok;
}

unsigned int l1menu::triggers::DoubleMu_v0::version() const
{
	return 0;
}

l1menu::triggers::DoubleMu::DoubleMu()
	: threshold1_(0), threshold2_(0), muonQuality_(0)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::DoubleMu::name() const
{
	return "L1_DoubleMu";
}

const std::vector<std::string> l1menu::triggers::DoubleMu::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("threshold2");
	returnValue.push_back("muonQuality");
	return returnValue;
}

float& l1menu::triggers::DoubleMu::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::DoubleMu::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else throw std::logic_error( "Not a valid parameter name" );
}
