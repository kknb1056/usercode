#include "l1menu/ReducedMenuSample.h"

#include <vector>
#include "l1menu/IReducedEvent.h"
#include "l1menu/MenuSample.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/tools.h"

namespace // unnamed namespace
{
	class ReducedEventImplementation : public l1menu::IReducedEvent
	{
	public:
		virtual ~ReducedEventImplementation() {}
		virtual float parameterValue( size_t parameterNumber ) { return pThresholdValues->at(parameterNumber); }
		virtual size_t parameterNumber( const l1menu::ITrigger& trigger, const std::string& parameterName ) { return 0; }
		virtual float weight() const { return 1; }
		std::vector<float>* pThresholdValues;
	};
}

namespace l1menu
{
	/** @brief Private members for the ReducedMenuSample class
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 28/May/2013
	 */
	class ReducedMenuSamplePrivateMembers
	{
	public:
		ReducedMenuSamplePrivateMembers( size_t newNumberOfEvents, size_t numberOfParameters )
			: thresholdsForAllEvents( newNumberOfEvents, std::vector<float>(numberOfParameters) )
		{
			// No operation besides the initialiser list
		}
		std::vector< std::vector<float> > thresholdsForAllEvents;
		::ReducedEventImplementation event;
		size_t numberOfEvents;
	};
}

l1menu::ReducedMenuSample::ReducedMenuSample( const l1menu::MenuSample& originalSample, const l1menu::TriggerMenu& triggerMenu )
{
	size_t numberOfEvents=originalSample.numberOfEvents();
	// Need to find out how many parameters there are for each event. Basically the sum
	// of the number of thresholds for all triggers.
	size_t numberOfParameters=0;
	for( size_t triggerNumber=0; triggerNumber<triggerMenu.numberOfTriggers(); ++triggerNumber )
	{
		const l1menu::ITrigger& trigger=triggerMenu.getTrigger(triggerNumber);
		numberOfParameters+=l1menu::getThresholdNames(trigger).size();
	}

	// Now I know how many events there are and how many parameters, I can create the pimple
	// with the correct parameters.
	pImple_.reset( new l1menu::ReducedMenuSamplePrivateMembers( numberOfEvents, numberOfParameters ) );
}

size_t l1menu::ReducedMenuSample::numberOfEvents() const
{
	return pImple_->numberOfEvents;
}

const l1menu::IReducedEvent& l1menu::ReducedMenuSample::getEvent( size_t eventNumber ) const
{
	pImple_->event.pThresholdValues=&pImple_->thresholdsForAllEvents[eventNumber];
	return pImple_->event;
}
