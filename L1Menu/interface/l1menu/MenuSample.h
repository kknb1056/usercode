#ifndef l1menu_MenuSample_h
#define l1menu_MenuSample_h

#include <string>
#include <memory>
#include "l1menu/ISample.h"

// Forward declarations
namespace l1menu
{
	class L1TriggerDPGEvent;
}


namespace l1menu
{

	class MenuSample : public l1menu::ISample
	{
	public:
		MenuSample();
		virtual ~MenuSample();
		MenuSample( const MenuSample& otherMenuSample );
		MenuSample( MenuSample&& otherMenuSample ) noexcept;
		MenuSample& operator=( const MenuSample& otherMenuSample );
		MenuSample& operator=( MenuSample&& otherMenuSample ) noexcept;

		void loadFile( const std::string& filename );
		const l1menu::L1TriggerDPGEvent& getFullEvent( size_t eventNumber ) const;

		virtual size_t numberOfEvents() const;
		virtual const l1menu::IEvent& getEvent( size_t eventNumber ) const;
		virtual std::unique_ptr<l1menu::ICachedTrigger> createCachedTrigger( const l1menu::ITrigger& trigger ) const;
		virtual float eventRate() const;
		virtual void setEventRate( float rate );
		virtual float sumOfWeights() const;
		virtual std::unique_ptr<const l1menu::IMenuRate> rate( const l1menu::TriggerMenu& menu ) const;
	private:
		class MenuSamplePrivateMembers* pImple_;
	}; // end of class MenuSample

} // end of namespace l1menu

#endif
