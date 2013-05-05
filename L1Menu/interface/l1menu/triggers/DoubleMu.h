#ifndef menugeneration_triggers_DoubleMu_h
#define menugeneration_triggers_DoubleMu_h

#include "menugeneration/ITrigger.h"

#include <string>
#include <vector>

namespace menugeneration
{
	namespace triggers
	{
		/** @brief Base class for all versions of the DoubleMu trigger.
		 *
		 * Note that this class is abstract because it doesn't implement the "version"
		 * and "apply" methods. That's left up to the implementations of the different
		 * versions.
		 *
		 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
		 * @date 29/Apr/2013
		 */
		class DoubleMu : public menugeneration::ITrigger
		{
		public:
			DoubleMu();
			virtual const std::string name() const;
			virtual const std::vector<std::string> parameterNames() const;
			virtual float& parameter( const std::string& parameterName );
			virtual const float& parameter( const std::string& parameterName ) const;
		protected:
			float threshold1_;
			float threshold2_;
			float muonQuality_;
		}; // end of the DoubleMu base class

		/** @brief First version of the DoubleMu trigger.
		 *
		 * @author probably Brian Winer
		 * @date sometime
		 */
		class DoubleMu_v0 : public DoubleMu
		{
		public:
			virtual unsigned int version() const;
			virtual bool apply( const L1Analysis::L1AnalysisDataFormat& event ) const;
		}; // end of version 0 class

	} // end of namespace triggers

} // end of namespace menugeneration

#endif
