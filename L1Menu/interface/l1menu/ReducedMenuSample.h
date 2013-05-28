#ifndef l1menu_ReducedMenuSample_h
#define l1menu_ReducedMenuSample_h

#include <string>
#include <memory>

// Forward declarations
namespace l1menu
{
	class MenuSample;
	class TriggerMenu;
	class IReducedEvent;
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
		ReducedMenuSample( const l1menu::MenuSample& originalSample, const l1menu::TriggerMenu& triggerMenu );

		size_t numberOfEvents() const;
		const l1menu::IReducedEvent& getEvent( size_t eventNumber ) const;
	private:
		std::unique_ptr<class ReducedMenuSamplePrivateMembers> pImple_;
	}; // end of class ReducedMenuSample

} // end of namespace l1menu

#endif
