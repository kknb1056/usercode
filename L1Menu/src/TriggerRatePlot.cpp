#include "l1menu/TriggerRatePlot.h"

#include "l1menu/ITrigger.h"
#include "l1menu/IEvent.h"
#include "l1menu/TriggerTable.h"
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/IReducedEvent.h"
#include <TH1F.h>
#include <iostream>
#include <sstream>

l1menu::TriggerRatePlot::TriggerRatePlot( const l1menu::ITrigger& trigger, std::unique_ptr<TH1> pHistogram, const std::string versusParameter )
	: pHistogram_( std::move(pHistogram) ), versusParameter_(versusParameter), histogramOwnedByMe_(true)
{
	// Take a copy of the trigger
	l1menu::TriggerTable& table=l1menu::TriggerTable::instance();
	pTrigger_=table.copyTrigger( trigger );

	// Make sure the versusParameter_ supplied is valid. If it's not then this call will
	// throw an exception.
	pParameter_=&pTrigger_->parameter(versusParameter_);

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
		description << *iName << "=" << pTrigger_->parameter(*iName);
		if( iName+1!=parameterNames.end() ) description << ","; // Add delimeter between parameter names
	}
	description << "]";

	pHistogram_->SetTitle( description.str().c_str() );

}

l1menu::TriggerRatePlot::TriggerRatePlot( l1menu::TriggerRatePlot&& otherTriggerRatePlot ) noexcept
	: pTrigger_( std::move(otherTriggerRatePlot.pTrigger_) ), pHistogram_( std::move(otherTriggerRatePlot.pHistogram_) ),
	  versusParameter_( std::move(otherTriggerRatePlot.versusParameter_) ), pParameter_(otherTriggerRatePlot.pParameter_),
	  histogramOwnedByMe_(histogramOwnedByMe_)
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
	pParameter_=otherTriggerRatePlot.pParameter_;
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
	// loop over all of the bins in the histogram, and see if the trigger passes
	// for the value at the center of the bin.
	for( int binNumber=1; binNumber<pHistogram_->GetNbinsX(); ++binNumber )
	{
		(*pParameter_)=pHistogram_->GetBinCenter(binNumber);
		if( pTrigger_->apply( event ) )
		{
			pHistogram_->Fill( (*pParameter_), event.weight() );
		}
		// could put an "else break" in here, but I don't know if triggers
		// will run their thresholds the other way. E.g. a trigger could require
		// a minimum amount of energy in the event, in which case the higher
		// bins will pass.
	}
}

void l1menu::TriggerRatePlot::initiateForReducedSample( const l1menu::ReducedMenuSample& sample )
{
	pTrigger_->initiateForReducedSample( sample );
}

void l1menu::TriggerRatePlot::addEvent( const l1menu::IReducedEvent& event )
{
	// Basically exactly the same as for the l1menu::IEvent version. I
	// should probably template this.
	for( int binNumber=1; binNumber<pHistogram_->GetNbinsX(); ++binNumber )
	{
		(*pParameter_)=pHistogram_->GetBinCenter(binNumber);
		if( pTrigger_->apply( event ) )
		{
			pHistogram_->Fill( (*pParameter_), event.weight() );
		}
	}

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
