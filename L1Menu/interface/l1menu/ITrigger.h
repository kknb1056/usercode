#ifndef l1menu_ITrigger_h
#define l1menu_ITrigger_h

#include <string>
#include <vector>

// Forward declarations
namespace l1menu
{
	class IEvent;
	class IReducedEvent;
	class ReducedMenuSample;
}


namespace l1menu
{
	class ITrigger
	{
	public:
		virtual ~ITrigger() {}
		virtual const std::string name() const = 0;
		virtual unsigned int version() const = 0;
		virtual const std::vector<std::string> parameterNames() const = 0;
		virtual float& parameter( const std::string& parameterName ) = 0;
		virtual const float& parameter( const std::string& parameterName ) const = 0;
		virtual bool apply( const l1menu::IEvent& event ) const = 0;

		virtual void initiateForReducedSample( const l1menu::ReducedMenuSample& sample ) = 0;
		virtual bool apply( const l1menu::IReducedEvent& event ) const = 0;
	};

} // end of namespace l1menu

#endif
