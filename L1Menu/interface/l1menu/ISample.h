#ifndef l1menu_ISample_h
#define l1menu_ISample_h

#include <memory>

//
// Forward declarations
//
namespace l1menu
{
	class IMenuRate;
	class TriggerMenu;
}


namespace l1menu
{
	class ISample
	{
	public:
		virtual ~ISample() {}
		/** @brief The rate at which events are occurring. I.e. the trigger rate if every event passed. */
		virtual float eventRate() const = 0;
		virtual void setEventRate( float rate ) const = 0;
		virtual std::unique_ptr<const l1menu::IMenuRate> rate( const l1menu::TriggerMenu& menu ) const = 0;
	};

} // end of namespace l1menu

#endif
