#ifndef l1menu_triggers_SingleIsoEGEta_h
#define l1menu_triggers_SingleIsoEGEta_h

#include <string>
#include <vector>
#include "l1menu/ITrigger.h"

//
// Forward declarations
//
namespace l1menu
{
	class L1TriggerDPGEvent;
}

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the SingleIsoEGEta trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 02/Jun/2013
		 */
		class SingleIsoEGEta : public l1menu::ITrigger
		{
		public:
			SingleIsoEGEta();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;
		protected:
			float threshold1_;
			float regionCut_;
		}; // end of the SingleIsoEGEta base class

		/** @brief First version of the SingleIsoEGEta trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class SingleIsoEGEta_v0 : public SingleIsoEGEta
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const l1menu::L1TriggerDPGEvent& event ) const;
		}; // end of version 0 class

	} // end of namespace triggers

} // end of namespace l1menu

#endif
