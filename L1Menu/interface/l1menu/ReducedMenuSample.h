#ifndef l1menu_ReducedMenuSample_h
#define l1menu_ReducedMenuSample_h

#include <string>
#include <memory>
#include <map>

#include "l1menu/IReducedEvent.h"

// Forward declarations
namespace l1menu
{
	class MenuSample;
	class TriggerMenu;
	class ITrigger;
}


namespace l1menu
{
	/** @brief A simplified sample that has been reduced to just storing the thresholds required for each trigger to pass.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/May/2013
	 */
	class ReducedMenuSample
	{
	public:
		/** @brief Load from a file in protobuf format. */
		ReducedMenuSample( const std::string& filename );
		ReducedMenuSample( const l1menu::MenuSample& originalSample, const l1menu::TriggerMenu& triggerMenu );
		ReducedMenuSample( const l1menu::TriggerMenu& triggerMenu );
		virtual ~ReducedMenuSample();

		void addSample( const l1menu::MenuSample& originalSample );

		/** @brief Save to a file in protobuf format (protobuf in src/protobuf/l1menu.proto). */
		void saveToFile( const std::string& filename ) const;

		size_t numberOfEvents() const;
		const l1menu::IReducedEvent& getEvent( size_t eventNumber ) const;

		const l1menu::TriggerMenu& getTriggerMenu() const;
		bool containsTrigger( const l1menu::ITrigger& trigger, bool allowOlderVersion=false ) const;
		const std::map<std::string,IReducedEvent::ParameterID> getTriggerParameterIdentifiers( const l1menu::ITrigger& trigger, bool allowOlderVersion=false ) const;
	private:
		std::unique_ptr<class ReducedMenuSamplePrivateMembers> pImple_;
	}; // end of class ReducedMenuSample

} // end of namespace l1menu

#endif
