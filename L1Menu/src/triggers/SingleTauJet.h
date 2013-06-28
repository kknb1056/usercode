#ifndef l1menu_triggers_SingleTauJet_h
#define l1menu_triggers_SingleTauJet_h

#include <string>
#include <vector>
#include "l1menu/ITrigger.h"
#include "l1menu/ReducedEvent.h"

//
// Forward declarations
//
namespace l1menu
{
	class L1TriggerDPGEvent;
	class ReducedMenuSample;
}

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the SingleTauJet trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 04/Jun/2013
		 */
		class SingleTauJet : public l1menu::ITrigger
		{
		public:
			SingleTauJet();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;

			virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample );
			virtual bool apply( const l1menu::ReducedEvent& event ) const;
		protected:
			float threshold1_;
			float regionCut_;
			ReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
		}; // end of the SingleTauJet base class

		/** @brief First version of the SingleTauJet trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class SingleTauJet_v0 : public SingleTauJet
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::L1TriggerDPGEvent& event ) const;
		}; // end of version 0 class


	} // end of namespace triggers

} // end of namespace l1menu

#endif
