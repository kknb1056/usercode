#include "l1menu/triggers/DoubleJetCentral.h"

#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/IEvent.h"

#include <stdexcept>
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
		REGISTER_TRIGGER_AND_CUSTOMISE( DoubleJetCentral_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				DoubleJetCentral_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold1", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold2", 100, 0, 100 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call

	} // end of namespace triggers
} // end of namespace l1menu

bool l1menu::triggers::DoubleJetCentral_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw=PhysicsBits[0]; // ZeroBias
	if( !raw ) return false;

	int n1=0;
	int n2=0;
	int Nj=analysisDataFormat.Njet;
	for( int ue=0; ue<Nj; ue++ )
	{
		int bx=analysisDataFormat.Bxjet[ue];
		if( bx!=0 ) continue;
		bool isFwdJet=analysisDataFormat.Fwdjet[ue];
		if( isFwdJet ) continue;
		bool isTauJet=analysisDataFormat.Taujet[ue];
		if( isTauJet ) continue;
		float rank=analysisDataFormat.Etjet[ue];
		float pt=rank; //CorrectedL1JetPtByGCTregions(analysisDataFormat.Etajet[ue],rank*4.,theL1JetCorrection);
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
