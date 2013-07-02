#include "l1menu/implementation/TriggerRateImplementation.h"

#include "l1menu/implementation/MenuRateImplementation.h"
#include "l1menu/TriggerTable.h"
#include "l1menu/ITrigger.h"

l1menu::implementation::TriggerRateImplementation::TriggerRateImplementation( const l1menu::ITrigger& trigger, float fraction, const MenuRateImplementation& menuRate )
	: fraction_(fraction), menuRate_(menuRate)
{
	pTrigger_=std::move( l1menu::TriggerTable::instance().copyTrigger(trigger) );
}

l1menu::implementation::TriggerRateImplementation& l1menu::implementation::TriggerRateImplementation::operator=( TriggerRateImplementation&& otherTriggerRate )
{
	pTrigger_=std::move( otherTriggerRate.pTrigger_ );
	fraction_=otherTriggerRate.fraction_;
	// I can't change the menuRate_ reference, but that should already be set to the right one anyway.
	return *this;
}

l1menu::implementation::TriggerRateImplementation::~TriggerRateImplementation()
{
	// No operation
}

const l1menu::ITrigger& l1menu::implementation::TriggerRateImplementation::trigger() const
{
	return *pTrigger_;
}

float l1menu::implementation::TriggerRateImplementation::fraction() const
{
	return fraction_;
}

float l1menu::implementation::TriggerRateImplementation::rate() const
{
	return fraction_*menuRate_.scaling();
}
