#include "l1menu/TriggerRatePlot.h"

#include "l1menu/ITrigger.h"
#include "l1menu/ICachedTrigger.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include "l1menu/IEvent.h"
#include "l1menu/ISample.h"
#include "l1menu/TriggerTable.h"
#include <TH1F.h>
#include <sstream>
#include <algorithm>

#include <iostream>

l1menu::TriggerRatePlot::TriggerRatePlot( const l1menu::ITrigger& trigger, std::unique_ptr<TH1> pHistogram, const std::string& versusParameter, const std::vector<std::string> scaledParameters )
	: pHistogram_( std::move(pHistogram) ), versusParameter_(versusParameter), histogramOwnedByMe_(true)
{
	// Take a copy of the trigger
	l1menu::TriggerTable& table=l1menu::TriggerTable::instance();
	pTrigger_=table.copyTrigger( trigger );

	// Make sure the versusParameter_ supplied is valid. If it's not then this call will
	// throw an exception. Take a pointer to the parameter so I don't need to keep performing
	// expensive string comparisons.
	pParameter_=&pTrigger_->parameter(versusParameter_);

	// If any parameters have been requested to be scaled along with the versusParameter, figure
	// out what the scaling should be and take a note of pointers.
	for( const auto& parameterToScale : scaledParameters )
	{
		if( parameterToScale!=versusParameter_ )
		{
			otherParameterScalings_.push_back( std::make_pair( &pTrigger_->parameter(parameterToScale), pTrigger_->parameter(parameterToScale)/(*pParameter_) ) );
		}
	}
	// I want to make a note of the other parameters set for the trigger. As far as I know TH1
	// has no way of adding annotations so I'll tag it on the end of the title.
	std::stringstream description;
	description << pTrigger_->name() << " rate versus " << versusParameter;

	// Loop over all of the parameters and add their values to the description
	std::vector<std::string> parameterNames=pTrigger_->parameterNames();
	description << " [v" << pTrigger_->version();
	if( parameterNames.size()>1 ) description << ",";
	for( std::vector<std::string>::const_iterator iName=parameterNames.begin(); iName!=parameterNames.end(); ++iName )
	{
		if( *iName==versusParameter ) continue; // Don't bother adding the parameter I'm plotting against

		// First check to see if this is one of the parameters that are being scaled
		if( std::find(scaledParameters.begin(),scaledParameters.end(),*iName)==scaledParameters.end() )
		{
			// This parameter isn't being scaled, so write the absoulte value in the description
			description << *iName << "=" << pTrigger_->parameter(*iName);
		}
		else
		{
			// This parameter is being scaled, so write what the scaling is in the description
			description << *iName << "=x*" << pTrigger_->parameter(*iName)/(*pParameter_);
		}

		if( iName+1!=parameterNames.end() ) description << ","; // Add delimeter between parameter names
	}
	description << "]";

	pHistogram_->SetTitle( description.str().c_str() );

}

l1menu::TriggerRatePlot::TriggerRatePlot( l1menu::TriggerRatePlot&& otherTriggerRatePlot ) noexcept
	: pTrigger_( std::move(otherTriggerRatePlot.pTrigger_) ), pHistogram_( std::move(otherTriggerRatePlot.pHistogram_) ),
	  versusParameter_( std::move(otherTriggerRatePlot.versusParameter_) ), pParameter_(&pTrigger_->parameter(versusParameter_)),
	  otherParameterScalings_( std::move(otherTriggerRatePlot.otherParameterScalings_) ), histogramOwnedByMe_(histogramOwnedByMe_)
{
	// No operation besides the initaliser list
}

l1menu::TriggerRatePlot& l1menu::TriggerRatePlot::operator=( l1menu::TriggerRatePlot&& otherTriggerRatePlot ) noexcept
{
	// Free up whatever was there before
	pTrigger_.reset();
	if( histogramOwnedByMe_ ) pHistogram_.reset();
	else pHistogram_.release();

	// Then copy from the other object
	pTrigger_=std::move(otherTriggerRatePlot.pTrigger_);
	pHistogram_=std::move(otherTriggerRatePlot.pHistogram_);
	versusParameter_=std::move(otherTriggerRatePlot.versusParameter_);
	pParameter_=&pTrigger_->parameter(versusParameter_);
	otherParameterScalings_=std::move(otherTriggerRatePlot.otherParameterScalings_);
	histogramOwnedByMe_=otherTriggerRatePlot.histogramOwnedByMe_;

	return *this;
}

l1menu::TriggerRatePlot::~TriggerRatePlot()
{
	// If the flag has been set telling me that this instance doesn't own the histogram,
	// then I need to tell the unique_ptr not to delete it.
	if( !histogramOwnedByMe_ )
	{
		pHistogram_.release();
	}
}

void l1menu::TriggerRatePlot::addEvent( const l1menu::IEvent& event )
{
	const l1menu::ISample& sample=event.sample();
	float weightPerEvent=sample.eventRate()/sample.sumOfWeights();

	// For some implementations of ISample, it is significantly faster to
	// create ICachedTriggers and then loop over those.
	std::unique_ptr<l1menu::ICachedTrigger> pCachedTrigger=sample.createCachedTrigger( *pTrigger_ );

	for( int binNumber=1; binNumber<pHistogram_->GetNbinsX(); ++binNumber )
	{
		(*pParameter_)=pHistogram_->GetBinCenter(binNumber);

		// Scale accordingly any other parameters that should be scaled.
		for( const auto& parameterScalingPair : otherParameterScalings_ ) *(parameterScalingPair.first)=parameterScalingPair.second*(*pParameter_);

		if( pCachedTrigger->apply(event) )
		{
			pHistogram_->Fill( (*pParameter_), event.weight()*weightPerEvent );
		}
		else break;
		// Not sure if this "else break" is a good idea. I don't know if triggers
		// will run their thresholds the other way. E.g. a trigger could require
		// a minimum amount of energy in the event, in which case the higher
		// bins will pass.
	}
}

void l1menu::TriggerRatePlot::addSample( const l1menu::ISample& sample )
{
	float weightPerEvent=sample.eventRate()/sample.sumOfWeights();

	// Create a cached trigger, which depending on the concrete type of the ISample
	// may or may not significantly increase the speed at which this next loop happens.
	std::unique_ptr<l1menu::ICachedTrigger> pCachedTrigger=sample.createCachedTrigger( *pTrigger_ );

	for( size_t eventNumber=0; eventNumber<sample.numberOfEvents(); ++eventNumber )
	{
		const l1menu::IEvent& event=sample.getEvent( eventNumber );

		for( int binNumber=1; binNumber<pHistogram_->GetNbinsX(); ++binNumber )
		{
			(*pParameter_)=pHistogram_->GetBinCenter(binNumber);

			// Scale accordingly any other parameters that should be scaled.
			for( const auto& parameterScalingPair : otherParameterScalings_ ) *(parameterScalingPair.first)=parameterScalingPair.second*(*pParameter_);

			if( pCachedTrigger->apply(event) ) // If the event passes the trigger
			{
				pHistogram_->Fill( (*pParameter_), event.weight()*weightPerEvent );
			}
			else break;
			// Not sure if this "else break" is a good idea. I don't know if triggers
			// will run their thresholds the other way. E.g. a trigger could require
			// a minimum amount of energy in the event, in which case the higher
			// bins will pass.
		} // end of loop over histogram bins
	} // end of loop over events

}

const l1menu::ITrigger& l1menu::TriggerRatePlot::getTrigger() const
{
	return *pTrigger_;
}

TH1* l1menu::TriggerRatePlot::getPlot()
{
	return pHistogram_.get();
}

TH1* l1menu::TriggerRatePlot::relinquishOwnershipOfPlot()
{
	// Rather than call release() on the unique_ptr, I'll set a flag so that I know
	// to release() in the destructor instead. This way it's possible to relinquish
	// ownership but still perform operations on the histograms.
	histogramOwnedByMe_=false;
	return pHistogram_.get();
}
