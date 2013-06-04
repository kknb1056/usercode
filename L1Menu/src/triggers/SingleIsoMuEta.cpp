#include "SingleIsoMuEta.h"

#include <cmath>
#include <stdexcept>
#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/IEvent.h"
#include "l1menu/ReducedMenuSample.h"
#include "L1AnalysisDataFormat.h"



namespace l1menu
{
	namespace triggers
	{

		/* The REGISTER_TRIGGER macro will make sure that the given trigger is registered in the
		 * l1menu::TriggerTable when the program starts. I also want to provide some suggested binning
		 * however. The REGISTER_TRIGGER_AND_CUSTOMISE macro does exactly the same but lets me pass
		 * a pointer to a function that will be called directly after the trigger has been registered
		 * at program startup. The function takes no parameters and returns void. In this case I'm
		 * giving it a lambda function.
		 */
		REGISTER_TRIGGER_AND_CUSTOMISE( SingleIsoMuEta_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				SingleIsoMuEta_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold1", 100, 0, 100 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call

	} // end of namespace triggers

} // end of namespace l1menu



bool l1menu::triggers::SingleIsoMuEta_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw = PhysicsBits[0];   // ZeroBias
	if (! raw) return false;

	bool muon = false;

	int Nmu = analysisDataFormat.Nmu;
	for (int imu=0; imu < Nmu; imu++) {
		int bx = analysisDataFormat.Bxmu.at(imu);
//		if (bx != 0 || !analysisDataFormat.Isomu.at(imu)) continue;
		if (bx != 0) continue;
		float pt = analysisDataFormat.Ptmu.at(imu);
		int qual = analysisDataFormat.Qualmu.at(imu);
		if ( qual < muonQuality_) continue;
		float eta = analysisDataFormat.Etamu.at(imu);

		if (fabs(eta) > etaCut_) continue;
		if (pt >= threshold1_) muon = true;

//		if(pt>16. && threshold1_<23.) printf("%i Muon Pt %f  Cut %f  Pass/Fail %i \n",imu,pt,threshold1_,muon);
	}


	bool ok = muon;
	return ok;
}


unsigned int l1menu::triggers::SingleIsoMuEta_v0::version() const
{
	return 0;
}

void l1menu::triggers::SingleIsoMuEta::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,IReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "SingleIsoMuEta::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold1_=iFindResult->second;
}

bool l1menu::triggers::SingleIsoMuEta::apply( const l1menu::IReducedEvent& event ) const
{
	return ( threshold1_<=event.parameterValue(reducedSampleParameterID_threshold1_) );
}

l1menu::triggers::SingleIsoMuEta::SingleIsoMuEta()
	: threshold1_(20), muonQuality_(4), etaCut_(2.1)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::SingleIsoMuEta::name() const
{
	return "L1_SingleIsoMu";
}

const std::vector<std::string> l1menu::triggers::SingleIsoMuEta::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("muonQuality");
	returnValue.push_back("etaCut");
	return returnValue;
}

float& l1menu::triggers::SingleIsoMuEta::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::SingleIsoMuEta::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}
