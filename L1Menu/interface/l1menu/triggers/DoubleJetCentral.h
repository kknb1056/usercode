#ifndef l1menu_triggers_DoubleJetCentral_h
#define l1menu_triggers_DoubleJetCentral_h

#include "l1menu/ITrigger.h"

#include <string>
#include <vector>

namespace l1menu
{
	namespace triggers
	{
		/** @brief Base class for all versions of the DoubleJetCentral trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 29/Apr/2013
		 */
		class DoubleJetCentral : public l1menu::ITrigger
		{
		public:
			DoubleJetCentral();

			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;
		protected:
			float threshold1_;
			float threshold2_;
		}; // end of the DoubleJetCentral base class

		/** @brief First version of the DoubleJetCentral trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class DoubleJetCentral_v0 : public DoubleJetCentral
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const L1Analysis::L1AnalysisDataFormat& event ) const;
		}; // end of version 0 class

	} // end of namespace triggers

} // end of namespace l1menu

#endif
