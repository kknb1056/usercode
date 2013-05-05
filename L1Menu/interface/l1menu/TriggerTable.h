#ifndef l1menu_TriggerTable_h
#define l1menu_TriggerTable_h

#include <memory>

// Forward declarations
namespace l1menu
{
	class ITrigger;
}

namespace l1menu
{
	class TriggerTable
	{
	public:
		struct TriggerDetails
		{
			std::string name;
			unsigned int version;
			bool operator==( const TriggerDetails& otherTriggerDetails ) const;
		};
	public:
		/** @brief Get the latest version of the trigger with the supplied name. */
		std::auto_ptr<l1menu::ITrigger> getTrigger( const std::string& name ) const;

		/** @brief Get a specific version of the trigger with the supplied name. */
		std::auto_ptr<l1menu::ITrigger> getTrigger( const std::string& name, unsigned int version ) const;
		std::auto_ptr<l1menu::ITrigger> getTrigger( const TriggerDetails& details ) const;

		std::auto_ptr<l1menu::ITrigger> copyTrigger( const l1menu::ITrigger& triggerToCopy ) const;

		/** @brief List the triggers available.
		 *
		 * Returns a vector of pairs, where first is the trigger name and second is the trigger version.
		 */
		std::vector<TriggerDetails> listTriggers() const;

		static void registerTrigger( const std::string name, unsigned int version, std::auto_ptr<l1menu::ITrigger> (*creationFunctionPointer)() );
	private:
		struct TriggerRegistryEntry
		{
			TriggerDetails details;
			std::auto_ptr<l1menu::ITrigger> (*creationFunctionPointer)();
		};
		static std::vector<TriggerRegistryEntry> registeredTriggers;
	};

}
#endif
