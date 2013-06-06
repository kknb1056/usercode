#include "SingleIsoEGEta.h"

#include <stdexcept>
#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/IEvent.h"
#include "l1menu/ReducedMenuSample.h"
#include "UserCode/L1TriggerUpgrade/interface/L1AnalysisDataFormat.h"


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
		REGISTER_TRIGGER_AND_CUSTOMISE( SingleIsoEGEta_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				SingleIsoEGEta_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold1", 100, 0, 100 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call


	} // end of namespace triggers

} // end of namespace l1menu



bool l1menu::triggers::SingleIsoEGEta_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw = PhysicsBits[0];   // ZeroBias
	if (! raw) return false;

	bool ok=false;
	int Nele = analysisDataFormat.Nele;
	for (int ue=0; ue < Nele; ue++) {
		int bx = analysisDataFormat.Bxel[ue];
		if (bx != 0) continue;
		bool iso = analysisDataFormat.Isoel[ue];
		if (! iso) continue;
		float eta = analysisDataFormat.Etael[ue];
		if (eta < regionCut_ || eta > 21.-regionCut_) continue;  // eta = 5 - 16
		float rank = analysisDataFormat.Etel[ue];    // the rank of the electron
		float pt = rank ;
		if (pt >= threshold1_) ok = true;
	}  // end loop over EM objects

	return ok;
}


unsigned int l1menu::triggers::SingleIsoEGEta_v0::version() const
{
	return 0;
}

void l1menu::triggers::SingleIsoEGEta::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,IReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "SingleIsoEGEta::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold1_=iFindResult->second;
}

bool l1menu::triggers::SingleIsoEGEta::apply( const l1menu::IReducedEvent& event ) const
{
	return ( threshold1_<=event.parameterValue(reducedSampleParameterID_threshold1_) );
}

l1menu::triggers::SingleIsoEGEta::SingleIsoEGEta()
	: threshold1_(20), regionCut_(4.5)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::SingleIsoEGEta::name() const
{
	return "L1_SingleIsoEG";
}

const std::vector<std::string> l1menu::triggers::SingleIsoEGEta::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("regionCut");
	return returnValue;
}

float& l1menu::triggers::SingleIsoEGEta::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::SingleIsoEGEta::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}
