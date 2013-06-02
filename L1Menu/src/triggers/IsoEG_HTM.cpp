// This trigger was created programatically. I need to check everything over before
// I can use it properly so I'll disable it with this ifdef until I can do that.
#ifdef TRIGGER_HASNT_BEEN_CHECKED_YET

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
		/** @brief Base class for all versions of the IsoEG_HTM trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class IsoEG_HTM : public l1menu::ITrigger
		{
		public:
			IsoEG_HTM();

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
			float muonQuality_;
			float etaCut_;
			float regionCut_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold2_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold3_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold4_;
		}; // end of the IsoEG_HTM base class

		/** @brief First version of the IsoEG_HTM trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class IsoEG_HTM_v0 : public IsoEG_HTM
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
		REGISTER_TRIGGER_AND_CUSTOMISE( IsoEG_HTM_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				IsoEG_HTM_v0 tempTriggerInstance;
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


bool l1menu::triggers::IsoEG_HTM_v0::apply( const l1menu::IEvent& event ) const
{
	const L1Analysis::L1AnalysisDataFormat& analysisDataFormat=event.rawEvent();
	const bool* PhysicsBits=event.physicsBits();

}


unsigned int l1menu::triggers::IsoEG_HTM_v0::version() const
{
	return 0;
}

void l1menu::triggers::IsoEG_HTM::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	const auto& parameterIdentifiers=sample.getTriggerParameterIdentifiers( *this );

	std::map<std::string,IReducedEvent::ParameterID>::const_iterator iFindResult=parameterIdentifiers.find("threshold1");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "IsoEG_HTM::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold1_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold2");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "IsoEG_HTM::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold2_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold3");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "IsoEG_HTM::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold3_=iFindResult->second;

	iFindResult=parameterIdentifiers.find("threshold4");
	if( iFindResult==parameterIdentifiers.end() ) throw std::runtime_error( "IsoEG_HTM::initiateForReducedSample() - it appears this reduced sample wasn't created with this trigger. You can only run over a l1menu::ReducedMenuSample with triggers that were on when the sample was created." );
	else reducedSampleParameterID_threshold4_=iFindResult->second;
}

bool l1menu::triggers::IsoEG_HTM::apply( const l1menu::IReducedEvent& event ) const
{
	return ( threshold1_<=event.parameterValue(reducedSampleParameterID_threshold1_) )
		&& ( threshold2_<=event.parameterValue(reducedSampleParameterID_threshold2_) )
		&& ( threshold3_<=event.parameterValue(reducedSampleParameterID_threshold3_) )
		&& ( threshold4_<=event.parameterValue(reducedSampleParameterID_threshold4_) );
}

l1menu::triggers::IsoEG_HTM::IsoEG_HTM()
	: threshold1_(20), threshold2_(20), threshold3_(20), threshold4_(20), muonQuality_(4), etaCut_(2.1), regionCut_(4.5)
{
	// No operation other than the initialiser list
}

const std::string l1menu::triggers::IsoEG_HTM::name() const
{
	return "L1_SingleIsoEG_HTM";
}

const std::vector<std::string> l1menu::triggers::IsoEG_HTM::parameterNames() const
{
	std::vector<std::string> returnValue;
	returnValue.push_back("threshold1");
	returnValue.push_back("threshold2");
	returnValue.push_back("threshold3");
	returnValue.push_back("threshold4");
	returnValue.push_back("muonQuality");
	returnValue.push_back("etaCut");
	returnValue.push_back("regionCut");
	return returnValue;
}

float& l1menu::triggers::IsoEG_HTM::parameter( const std::string& parameterName )
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="threshold3" ) return threshold3_;
	else if( parameterName=="threshold4" ) return threshold4_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

const float& l1menu::triggers::IsoEG_HTM::parameter( const std::string& parameterName ) const
{
	if( parameterName=="threshold1" ) return threshold1_;
	else if( parameterName=="threshold2" ) return threshold2_;
	else if( parameterName=="threshold3" ) return threshold3_;
	else if( parameterName=="threshold4" ) return threshold4_;
	else if( parameterName=="muonQuality" ) return muonQuality_;
	else if( parameterName=="etaCut" ) return etaCut_;
	else if( parameterName=="regionCut" ) return regionCut_;
	else throw std::logic_error( "Not a valid parameter name" );
}

#endif // End of the ifdef that stops this code compiling until I've checked it over.
