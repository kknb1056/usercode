#ifndef l1menu_triggers_HTM_h
#define l1menu_triggers_HTM_h

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
		/** @brief Base class for all versions of the HTM trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class HTM : public l1menu::ITrigger
		{
		public:
			HTM();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;

			virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample );
			virtual bool apply( const l1menu::IReducedEvent& event ) const;
		protected:
			float threshold1_;
			IReducedEvent::ParameterID reducedSampleParameterID_threshold1_;
		}; // end of the HTM base class

		/** @brief First version of the HTM trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class HTM_v0 : public HTM
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::IEvent& event ) const;
		}; // end of version 0 class

	} // end of namespace triggers

} // end of namespace l1menu

#endif
