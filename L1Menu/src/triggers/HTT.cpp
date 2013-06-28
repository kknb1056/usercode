#include "l1menu/RegisterTriggerMacro.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/ReducedEvent.h"

#include <stdexcept>
#include "UserCode/L1TriggerUpgrade/interface/L1AnalysisDataFormat.h"

#include "l1menu/ITrigger.h"

#include <string>
#include <vector>
#include "l1menu/ReducedEvent.h"

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the HTT trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class HTT : public l1menu::ITrigger
		{
		public:
			HTT();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;

			virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample );
			virtual bool apply( const l1menu::ReducedEvent& event ) const;
		protected:
			float threshold1_;
			ReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
		}; // end of the HTT base class

		/** @brief First version of the HTT trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class HTT_v0 : public HTT
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::L1TriggerDPGEvent& event ) const;
		}; // end of version 0 class


		/* The REGISTER_TRIGGER macro will make sure that the given trigger is registered in the
		 * l1menu::TriggerTable when the program starts. I also want to provide some suggested binning
		 * however. The REGISTER_TRIGGER_AND_CUSTOMISE macro does exactly the same but lets me pass
		 * a pointer to a function that will be called directly after the trigger has been registered
		 * at program startup. The function takes no parameters and returns void. In this case I'm
		 * giving it a lambda function.
		 */
		REGISTER_TRIGGER_AND_CUSTOMISE( HTT_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				HTT_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "threshold1", 100, 0, 400 );
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


bool l1menu::triggers::HTT_v0::apply( const l1menu::L1TriggerDPGEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

	bool raw = PhysicsBits[0];   // ZeroBias
	if (! raw) return false;

	float adc = analysisDataFormat.HTT ;
	float TheHTT = adc; // / 2. ;

	if (TheHTT < threshold1_) return false;
	return true;
}


unsigned int l1menu::triggers::HTT_v0::version() const
{
	return 0;
}

void l1menu::triggers::HTT::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,ReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "HTT::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold1_=iFindResult->second;
}

bool l1menu::triggers::HTT::apply( const l1menu::ReducedEvent& event ) const
{
	return ( threshold1_<=event.parameterValue(reducedSampleParameterID_threshold1_) );
}

l1menu::triggers::HTT::HTT()
	: threshold1_(100)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::HTT::name() const
{
	return "L1_HTT";
}

const std::vector<std::string> l1menu::triggers::HTT::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	return returnValue;
}

float& l1menu::triggers::HTT::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::HTT::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else throw std::logic_error( "Not a valid parameter name" );
}
