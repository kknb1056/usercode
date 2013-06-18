#ifndef l1menu_TriggerMenu_h
#define l1menu_TriggerMenu_h

#include <memory>
#include <vector>
#include "l1menu/TriggerTable.h"

// Forward declarations
namespace l1menu
{
	class ITrigger;
	class IEvent;
}

namespace l1menu
{
	class TriggerMenu
	{
	public:
		TriggerMenu();
		virtual ~TriggerMenu();
		TriggerMenu( const TriggerMenu& otherTriggerMenu );
		TriggerMenu( TriggerMenu&& otherTriggerMenu ) noexcept;
		TriggerMenu& operator=( const TriggerMenu& otherTriggerMenu );
		TriggerMenu& operator=( TriggerMenu&& otherTriggerMenu ) noexcept;

		ITrigger& addTrigger( const std::string& triggerName );
		ITrigger& addTrigger( const std::string& triggerName, unsigned int version );

		size_t numberOfTriggers() const;
		ITrigger& getTrigger( size_t position );
		const ITrigger& getTrigger( size_t position ) const;

		std::unique_ptr<l1menu::ITrigger> getTriggerCopy( size_t position ) const;

		bool apply( const l1menu::IEvent& event ) const;

		void loadMenuFromFile( const std::string& filename );
	private:
		void loadMenuInOldFormat( std::ifstream& file );

		TriggerTable& triggerTable_;
		std::vector< std::unique_ptr<l1menu::ITrigger> > triggers_;
		std::vector<bool> triggerResults_; ///< @brief Stores the result of each trigger for the last call of "apply"
	};

}
#endif
