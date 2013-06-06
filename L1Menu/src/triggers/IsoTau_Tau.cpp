#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/IEvent.h"
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/IReducedEvent.h"

#include <stdexcept>
#include "UserCode/L1TriggerUpgrade/interface/L1AnalysisDataFormat.h"

#include "l1menu/ITrigger.h"

#include <string>
#include <vector>
#include "l1menu/IReducedEvent.h"

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the isoTau_Tau trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class isoTau_Tau : public l1menu::ITrigger
		{
		public:
			isoTau_Tau();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;

			virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample );
			virtual bool apply( const l1menu::IReducedEvent& event ) const;
		protected:
			float leg1threshold1_;
			float leg2threshold1_;
			float regionCut_;
			IReducedEvent::ParameterID reducedSampleParameterID_leg1threshold1_;
			IReducedEvent::ParameterID reducedSampleParameterID_leg2threshold1_;
		}; // end of the isoTau_Tau base class

		/** @brief First version of the isoTau_Tau trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class isoTau_Tau_v0 : public isoTau_Tau
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::IEvent& event ) const;
		}; // end of version 0 class


		/* The REGISTER_TRIGGER macro will make sure that the given trigger is registered in the
		 * l1menu::TriggerTable when the program starts. I also want to provide some suggested binning
		 * however. The REGISTER_TRIGGER_AND_CUSTOMISE macro does exactly the same but lets me pass
		 * a pointer to a function that will be called directly after the trigger has been registered
		 * at program startup. The function takes no parameters and returns void. In this case I'm
		 * giving it a lambda function.
		 */
		REGISTER_TRIGGER_AND_CUSTOMISE( isoTau_Tau_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				isoTau_Tau_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg1threshold1", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg2threshold1", 100, 0, 100 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call


	} // end of namespace triggers

} // end of namespace l1menu


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//---------------  Definitions below         ---------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


bool l1menu::triggers::isoTau_Tau_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw = PhysicsBits[0];  // ZeroBias
	if (! raw) return false;

	int n1=0;
	int n2=0;
	int Nj = analysisDataFormat.Njet ;
	for (int ue=0; ue < Nj; ue++) {
		int bx = analysisDataFormat.Bxjet[ue];
		if (bx != 0) continue;
		bool isTauJet = analysisDataFormat.Taujet[ue];
		if (! isTauJet) continue;
		float rank = analysisDataFormat.Etjet[ue];    // the rank of the electron
		float pt = rank; //CorrectedL1JetPtByGCTregions(analysisDataFormat.Etajet[ue],rank*4.,theL1JetCorrection);
		float eta = analysisDataFormat.Etajet[ue];
		if (eta < regionCut_ || eta > 21.-regionCut_) continue;  // eta = 5 - 16
		if (pt >= leg1threshold1_ && analysisDataFormat.isoTaujet[ue]) n1++;
		if (pt >= leg2threshold1_) n2++;
	}  // end loop over jets

	bool ok = ( n1 >=1 && n2 >= 2);
	return ok;
}


unsigned int l1menu::triggers::isoTau_Tau_v0::version() const
{
	return 0;
}

void l1menu::triggers::isoTau_Tau::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,IReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("leg1threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "isoTau_Tau::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_leg1threshold1_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("leg2threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "isoTau_Tau::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_leg2threshold1_=iFindResult->second;
}

bool l1menu::triggers::isoTau_Tau::apply( const l1menu::IReducedEvent& event ) const
{
	return ( leg1threshold1_<=event.parameterValue(reducedSampleParameterID_leg1threshold1_) )
		&& ( leg2threshold1_<=event.parameterValue(reducedSampleParameterID_leg2threshold1_) );
}

l1menu::triggers::isoTau_Tau::isoTau_Tau()
	: leg1threshold1_(20), leg2threshold1_(20), regionCut_(4.5)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::isoTau_Tau::name() const
{
	return "L1_isoTau_Tau";
}

const std::vector<std::string> l1menu::triggers::isoTau_Tau::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("leg1threshold1");
	returnValue.push_back("leg2threshold1");
	returnValue.push_back("regionCut");
	return returnValue;
}

float& l1menu::triggers::isoTau_Tau::parameter( const std::string& parameterName )
{
	if( parameterName=="leg1threshold1" ) return leg1threshold1_;
	else if( parameterName=="leg2threshold1" ) return leg2threshold1_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::isoTau_Tau::parameter( const std::string& parameterName ) const
{
	if( parameterName=="leg1threshold1" ) return leg1threshold1_;
	else if( parameterName=="leg2threshold1" ) return leg2threshold1_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}
