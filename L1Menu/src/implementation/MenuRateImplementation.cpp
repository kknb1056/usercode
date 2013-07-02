#include "l1menu/implementation/MenuRateImplementation.h"

#include <string>
#include <utility>
#include "l1menu/ITrigger.h"
#include "l1menu/ITriggerRate.h"
#include "l1menu/implementation/TriggerRateImplementation.h"

void l1menu::implementation::MenuRateImplementation::setTotalFraction( float fraction )
{
	totalFraction_=fraction;
}

void l1menu::implementation::MenuRateImplementation::setScaling( float scaling )
{
	scaling_=scaling;
}

float l1menu::implementation::MenuRateImplementation::scaling() const
{
	return scaling_;
}

void l1menu::implementation::MenuRateImplementation::addTriggerRate( const l1menu::ITrigger& trigger, float fraction )
{
	triggerRates_.push_back( std::move(TriggerRateImplementation(trigger,fraction,*this)) );
}

float l1menu::implementation::MenuRateImplementation::totalFraction() const
{
	return totalFraction_;
}

float l1menu::implementation::MenuRateImplementation::totalRate() const
{
	return totalFraction_*scaling_;
}

const std::vector<const l1menu::ITriggerRate*>& l1menu::implementation::MenuRateImplementation::triggerRates() const
{
	// If the sizes are the same I'll assume nothing has changed and the references
	// are still valid. I don't expect this method to be called until the triggerRates_
	// vector is complete anyway.
	if( triggerRates_.size()!=baseClassReferences_.size() )
	{
		baseClassReferences_.clear();
		for( const auto& triggerRate : triggerRates_ )
		{
			baseClassReferences_.push_back( &triggerRate );
		}
	}

	return baseClassReferences_;
}
