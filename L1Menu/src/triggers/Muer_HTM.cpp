#include "CrossTrigger.h"
#include "SingleMuEta.h"
#include "HTM.h"
#include "l1menu/implementation/RegisterTriggerMacro.h"

namespace l1menu
{
	namespace triggers
	{

		/** @brief Cross trigger of the SingleIsoEGEta and HTM triggers.
		 *
		 * Combines the following triggers: <br/>
		 * L1_SingleIsoEG version 0 <br/>
		 * L1_HTM version 0 <br/>
		 *
		 * @author Individual triggers coded by Brian Winer, re-factored into a derived class of
		 * CrossTrigger by Mark Grimes (mark.grimes@bristol.ac.uk).
		 * @date 03/Jun/2013
		 */
		class Muer_HTM_v0 : public l1menu::triggers::CrossTrigger
		{
		public:
			Muer_HTM_v0();
			virtual const std::string name() const;
			virtual unsigned int version() const;
		}; // end of version 0 class


		/* The REGISTER_TRIGGER macro will make sure that the given trigger is registered in the
		 * l1menu::TriggerTable when the program starts. I also want to provide some suggested binning
		 * however. The REGISTER_TRIGGER_AND_CUSTOMISE macro does exactly the same but lets me pass
		 * a pointer to a function that will be called directly after the trigger has been registered
		 * at program startup. The function takes no parameters and returns void. In this case I'm
		 * giving it a lambda function.
		 */
		REGISTER_TRIGGER_AND_CUSTOMISE( Muer_HTM_v0,
			[]() // Use a lambda function to customise rather than creating a named function that never gets used again.
			{
				l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();
				Muer_HTM_v0 tempTriggerInstance;
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg1threshold1", 100, 0, 100 );
				triggerTable.registerSuggestedBinning( tempTriggerInstance.name(), "leg2threshold1", 100, 0, 300 );
			} // End of customisation lambda function
		) // End of REGISTER_TRIGGER_AND_CUSTOMISE macro call


	} // end of namespace triggers

} // end of namespace l1menu


l1menu::triggers::Muer_HTM_v0::Muer_HTM_v0()
	: CrossTrigger( new l1menu::triggers::SingleMuEta_v0, new l1menu::triggers::HTM_v0 )
{
	// No operation besides passing the sub-triggers onto the base class
}

const std::string l1menu::triggers::Muer_HTM_v0::name() const
{
	return "L1_SingleMu_HTM";
}

unsigned int l1menu::triggers::Muer_HTM_v0::version() const
{
	return 0;
}
