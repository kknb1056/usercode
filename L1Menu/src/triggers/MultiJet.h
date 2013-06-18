#ifndef l1menu_triggers_MultiJet_h
#define l1menu_triggers_MultiJet_h

#include <string>
#include <vector>
#include "l1menu/ITrigger.h"
#include "l1menu/IReducedEvent.h"

//
// Forward declarations
//
namespace l1menu
{
	class IEvent;
	class ReducedMenuSample;
}

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the MultiJet trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class MultiJet : public l1menu::ITrigger
		{
		public:
			MultiJet();

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
			float numberOfJets_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold2_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold3_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold4_;
		}; // end of the MultiJet base class

		/** @brief First version of the MultiJet trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class MultiJet_v0 : public MultiJet
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::IEvent& event ) const;
		}; // end of version 0 class


	} // end of namespace triggers

} // end of namespace l1menu

#endif
