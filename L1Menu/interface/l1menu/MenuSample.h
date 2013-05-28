#ifndef l1menu_MenuSample_h
#define l1menu_MenuSample_h

#include <string>
#include <memory>

// Forward declarations
namespace l1menu
{
	class IEvent;
}


namespace l1menu
{

	class MenuSample
	{
	public:
		MenuSample();
		virtual ~MenuSample();
		MenuSample( const MenuSample& otherMenuSample );
		MenuSample( MenuSample&& otherMenuSample ) noexcept;
		MenuSample& operator=( const MenuSample& otherMenuSample );
		MenuSample& operator=( MenuSample&& otherMenuSample ) noexcept;

		void loadFile( const std::string& filename );
		size_t numberOfEvents() const;
		const l1menu::IEvent& getEvent( size_t eventNumber ) const;
	private:
		class MenuSamplePrivateMembers* pImple_;
	}; // end of class MenuSample

} // end of namespace l1menu

#endif
