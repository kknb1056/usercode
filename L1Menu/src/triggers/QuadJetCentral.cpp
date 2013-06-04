#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/IEvent.h"
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/IReducedEvent.h"

#include <stdexcept>
#include "L1AnalysisDataFormat.h"

#include "l1menu/ITrigger.h"

#include <string>
#include <vector>
#include "l1menu/IReducedEvent.h"

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the QuadJetCentral trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class QuadJetCentral : public l1menu::ITrigger
		{
		public:
			QuadJetCentral();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;

			virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample );
			virtual bool apply( const l1menu::IReducedEvent& event ) const;
		protected:
			float threshold1_;
			float threshold2_;
			float threshold3_;
			float threshold4_;
			float regionCut_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold2_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold3_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold4_;
		}; // end of the QuadJetCentral base class

		/** @brief First version of the QuadJetCentral trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class QuadJetCentral_v0 : public QuadJetCentral
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
		REGISTER_TRIGGER_AND_CUSTOMISE( QuadJetCentral_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				QuadJetCentral_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold1", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold2", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold3", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold4", 100, 0, 100 );
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


bool l1menu::triggers::QuadJetCentral_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw = PhysicsBits[0];   // ZeroBias
	if (! raw) return false;


	int n1=0;
	int n2=0;
	int n3=0;
	int n4=0;

	int Nj = analysisDataFormat.Njet ;
	for (int ue=0; ue < Nj; ue++) {
		int bx = analysisDataFormat.Bxjet[ue];
		if (bx != 0) continue;
		bool isFwdJet = analysisDataFormat.Fwdjet[ue];
		if (isFwdJet) continue;
		bool isTauJet = analysisDataFormat.Taujet[ue];
		if (isTauJet) continue;

		float eta = analysisDataFormat.Etajet[ue];
		if (eta < regionCut_ || eta > 21.-regionCut_) continue;  // eta = 5 - 16

		float rank = analysisDataFormat.Etjet[ue];
		float pt = rank; //CorrectedL1JetPtByGCTregions(analysisDataFormat.Etajet[ue],rank*4.,theL1JetCorrection);
		if (pt >= threshold1_) n1++;
		if (pt >= threshold2_) n2++;
		if (pt >= threshold3_) n3++;
		if (pt >= threshold4_) n4++;
	}

	bool ok = ( n1 >=1 && n2 >= 2 && n3 >= 3 && n4 >= 4);
	return ok;
}


unsigned int l1menu::triggers::QuadJetCentral_v0::version() const
{
	return 0;
}

void l1menu::triggers::QuadJetCentral::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,IReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "QuadJetCentral::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold1_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold2");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "QuadJetCentral::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold2_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold3");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "QuadJetCentral::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold3_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold4");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "QuadJetCentral::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold4_=iFindResult->second;
}

bool l1menu::triggers::QuadJetCentral::apply( const l1menu::IReducedEvent& event ) const
{
	return ( threshold1_<=event.parameterValue(reducedSampleParameterID_threshold1_) )
		&& ( threshold2_<=event.parameterValue(reducedSampleParameterID_threshold2_) )
		&& ( threshold3_<=event.parameterValue(reducedSampleParameterID_threshold3_) )
		&& ( threshold4_<=event.parameterValue(reducedSampleParameterID_threshold4_) );
}

l1menu::triggers::QuadJetCentral::QuadJetCentral()
	: threshold1_(20), threshold2_(20), threshold3_(20), threshold4_(20), regionCut_(4.5)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::QuadJetCentral::name() const
{
	return "L1_QuadJetC";
}

const std::vector<std::string> l1menu::triggers::QuadJetCentral::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("threshold2");
	returnValue.push_back("threshold3");
	returnValue.push_back("threshold4");
	returnValue.push_back("regionCut");
	return returnValue;
}

float& l1menu::triggers::QuadJetCentral::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="threshold3" ) return threshold3_;
	else if( parameterName=="threshold4" ) return threshold4_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::QuadJetCentral::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="threshold3" ) return threshold3_;
	else if( parameterName=="threshold4" ) return threshold4_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}