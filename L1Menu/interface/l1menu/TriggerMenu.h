#ifndef l1menu_TriggerMenu_h
#define l1menu_TriggerMenu_h

#include <memory>
#include "l1menu/TriggerTable.h"

// Forward declarations
namespace l1menu
{
	class ITrigger;
}
namespace L1Analysis
{
	class L1AnalysisDataFormat;
}

namespace l1menu
{
	class TriggerMenu
	{
	public:
		TriggerMenu( const TriggerTable& table=TriggerTable() );
		virtual ~TriggerMenu();
		TriggerMenu( const TriggerMenu& otherTriggerMenu );
		TriggerMenu& operator=( const TriggerMenu& otherTriggerMenu );

		bool addTrigger( const std::string& triggerName );
		bool addTrigger( const std::string& triggerName, unsigned int version );

		size_t numberOfTriggers() const;
		ITrigger& getTrigger( size_t position );
		const ITrigger& getTrigger( size_t position ) const;

		bool apply( const L1Analysis::L1AnalysisDataFormat& event ) const;

		void loadMenuFromFile( const std::string& filename );
	private:
		void loadMenuInOldFormat( std::ifstream& file );

		TriggerTable triggerTable_;
		std::vector<l1menu::ITrigger*> triggers_;
		std::vector<bool> triggerResults_; ///< @brief Stores the result of each trigger for the last call of "apply"
	};

}
#endif
