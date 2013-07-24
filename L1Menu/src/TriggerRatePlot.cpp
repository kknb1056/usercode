#include "l1menu/TriggerRatePlot.h"

#include "l1menu/ITrigger.h"
#include "l1menu/ICachedTrigger.h"
#include "l1menu/L1TriggerDPGEvent.h"
#include "l1menu/IEvent.h"
#include "l1menu/ISample.h"
#include "l1menu/TriggerTable.h"
#include "l1menu/tools/tools.h"
#include "l1menu/tools/stringManipulation.h"
#include <TH1F.h>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

l1menu::TriggerRatePlot::TriggerRatePlot( const l1menu::ITrigger& trigger, std::unique_ptr<TH1> pHistogram, const std::string& versusParameter, const std::vector<std::string> scaledParameters )
	: pHistogram_( std::move(pHistogram) ), versusParameter_(versusParameter), histogramOwnedByMe_(true)
{
	initiate( trigger, scaledParameters );
}

l1menu::TriggerRatePlot::TriggerRatePlot( const l1menu::ITrigger& trigger, const std::string& name, size_t numberOfBins, float lowEdge, float highEdge, const std::string& versusParameter, const std::vector<std::string> scaledParameters )
	: pHistogram_( new TH1F( name.c_str(), "This title gets changed later", numberOfBins, lowEdge, highEdge ) ), versusParameter_(versusParameter), histogramOwnedByMe_(true)
{
	pHistogram_->SetDirectory( NULL ); // Hold in memory only
	initiate( trigger, scaledParameters );
}

l1menu::TriggerRatePlot::TriggerRatePlot( const TH1* pPreExisitingHistogram )
{
	// All of the information about the trigger is stored in the title, so examine
	// that to see what the trigger is, the version, and all of the parameters.
	// Format is "<name> rate versus <thresholdName> [v<version>,<param1>=<value>,<param2>=<value>,...]"
	// so first split by whitespace.
	std::vector<std::string> titleSplitByWhitespace=l1menu::tools::splitByWhitespace( pPreExisitingHistogram->GetTitle() );
	std::cout << "titleSplitByWhitespace="; for( const auto& element : titleSplitByWhitespace ) std::cout << "\"" << element << "\","; std::cout << std::endl;

	if( titleSplitByWhitespace.size()!=5 ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because the title is not as expected" );
	std::string triggerName=titleSplitByWhitespace[0];
	versusParameter_=titleSplitByWhitespace[3];

	// Check that the final element begins with "[" and ends with "]"
	if( titleSplitByWhitespace[4].front()!='[' || titleSplitByWhitespace[4].back()!=']' ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because the title is not as expected" );
	// If so, chop off those two parts
	std::string parameterList=titleSplitByWhitespace[4].substr( 1, titleSplitByWhitespace[4].size()-2 );
	// and split by commas
	std::vector<std::string> parameterListSplitByCommas=l1menu::tools::splitByDelimeters( parameterList, "," );
	std::cout << "parameterListSplitByCommas="; for( const auto& element : parameterListSplitByCommas ) std::cout << "\"" << element << "\","; std::cout << std::endl;
	// Need to at least have the version listed
	if( parameterListSplitByCommas.empty() ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because the title is not as expected" );

	// Figure out the version number of the trigger. It should be prefixed with a "v".
	if( parameterListSplitByCommas[0].front()!='v' ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because the title is not as expected" );
	std::string versionAsString=parameterListSplitByCommas[0].substr( 1 );
	std::cout << "versionAsString=" << versionAsString << std::endl;
	unsigned int versionNumber=l1menu::tools::convertStringToFloat( versionAsString );

	// Now I have the trigger name and version number, I can create a copy from the TriggerTable.
	std::unique_ptr<l1menu::ITrigger> pTemporaryTriggerCopy=l1menu::TriggerTable::instance().getTrigger( triggerName, versionNumber );
	if( pTemporaryTriggerCopy.get()==NULL ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because trigger is not in the TriggerTable" );

	// Set the main threshold to 1 just as an arbitrary starting point. I might need to scale thresholds
	// off that in a minute.
	pTemporaryTriggerCopy->parameter( versusParameter_ )=1;

	std::vector<std::string> scaledParameters; // This will build into a list of anything that should scale with the main threshold

	// Then loop through the other parameters and set them to whatever they're listed as.
	// Start at 1 because I've already done the version.
	for( size_t index=1; index<parameterListSplitByCommas.size(); ++index )
	{
		std::cout << "Checking string " << parameterListSplitByCommas[index] << std::endl;
		// Split by equals sign
		std::vector<std::string> nameAndValue=l1menu::tools::splitByDelimeters( parameterListSplitByCommas[index], "=" );
		if( nameAndValue.size()!=2 ) throw std::runtime_error( "TriggerRatePlot cannot be loaded from the supplied histogram because the title is not as expected" );

		// The value could be scaled against the main threshold, so I need to check for that.
		std::string valueAsString=nameAndValue[1];
		if( valueAsString.size()>2 )
		{
			if( valueAsString[0]=='x' && valueAsString[1]=='*' )
			{
				// Parameter is scaled with the main threshold. Since I've set that to 1
				// I can just use the number after the "x*" as is, so just chop off the
				// first two characters.
				valueAsString=valueAsString.substr( 2 );
				scaledParameters.push_back( nameAndValue[0] );
			}
		}

		pTemporaryTriggerCopy->parameter( nameAndValue[0] )=l1menu::tools::convertStringToFloat( valueAsString );
	}

	// Everything seems to have worked okay, so I can take a copy of the histogram
	// and delegate to the normal initialisation routine.
	pHistogram_.reset( static_cast<TH1*>( pPreExisitingHistogram->Clone() ) );
	pHistogram_->SetDirectory( NULL ); // Hold in memory only
	initiate( *pTemporaryTriggerCopy, scaledParameters );
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

void l1menu::TriggerRatePlot::initiate( const l1menu::ITrigger& trigger, const std::vector<std::string>& scaledParameters )
{
	// Before making any histograms make sure errors are done properly
	TH1::SetDefaultSumw2();

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
	description << pTrigger_->name() << " rate versus " << versusParameter_;

	// Loop over all of the parameters and add their values to the description
	std::vector<std::string> parameterNames=pTrigger_->parameterNames();
	description << " [v" << pTrigger_->version();
	if( parameterNames.size()>1 ) description << ",";
	for( std::vector<std::string>::const_iterator iName=parameterNames.begin(); iName!=parameterNames.end(); ++iName )
	{
		if( *iName==versusParameter_ ) continue; // Don't bother adding the parameter I'm plotting against

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
	// create ICachedTriggers and then loop over those. The addEvent overload
	// I'm about to delegate to loops over the histogram bins, so even for
	// one event this trigger can be called multiple times.
	std::unique_ptr<l1menu::ICachedTrigger> pCachedTrigger=sample.createCachedTrigger( *pTrigger_ );

	addEvent( event, pCachedTrigger, weightPerEvent );
}

void l1menu::TriggerRatePlot::addSample( const l1menu::ISample& sample )
{
	float weightPerEvent=sample.eventRate()/sample.sumOfWeights();

	// Create a cached trigger, which depending on the concrete type of the ISample
	// may or may not significantly increase the speed at which this next loop happens.
	std::unique_ptr<l1menu::ICachedTrigger> pCachedTrigger=sample.createCachedTrigger( *pTrigger_ );

	for( size_t eventNumber=0; eventNumber<sample.numberOfEvents(); ++eventNumber )
	{
		addEvent( sample.getEvent(eventNumber), pCachedTrigger, weightPerEvent );
	} // end of loop over events

}

void l1menu::TriggerRatePlot::addEvent( const l1menu::IEvent& event, const std::unique_ptr<l1menu::ICachedTrigger>& pCachedTrigger, float weightPerEvent )
{
	//
	// Use bisection to find the bin that passes the trigger and the one
	// immediately after it that fails.
	//
	size_t lowBin=1;
	size_t highBin=pHistogram_->GetNbinsX();

	//
	// First need to perform a check that the first bin passes. If it doesn't then
	// the histogram doesn't need filling at all and I can return.
	//
	(*pParameter_)=pHistogram_->GetBinLowEdge(lowBin);
	// Scale accordingly any other parameters that should be scaled. Remember that
	// in parameterScalingPair, 'first' is a pointer to the threshold to be changed
	// and 'second' is the ratio of the first threshold it should be.
	for( const auto& parameterScalingPair : otherParameterScalings_ ) *(parameterScalingPair.first)=parameterScalingPair.second*(*pParameter_);
	if( !pCachedTrigger->apply(event) ) return;

	//
	// Also check the highest bin. If that passes then I just fill every bin,
	// otherwise I need to find the point at which the trigger fails.
	//
	(*pParameter_)=pHistogram_->GetBinLowEdge(highBin);
	for( const auto& parameterScalingPair : otherParameterScalings_ ) *(parameterScalingPair.first)=parameterScalingPair.second*(*pParameter_);

	if( pCachedTrigger->apply(event) ) lowBin=highBin;
	else
	{
		while( highBin-lowBin>1 ) // Loop until I find two bins next to each other
		{
			size_t middleBin=(highBin+lowBin)/2;

			(*pParameter_)=pHistogram_->GetBinLowEdge(middleBin);
			for( const auto& parameterScalingPair : otherParameterScalings_ ) *(parameterScalingPair.first)=parameterScalingPair.second*(*pParameter_);

			if( pCachedTrigger->apply(event) ) lowBin=middleBin;
			else highBin=middleBin;
		}
	}

	//
	// Now I know which bins need filling, loop over them and fill.
	//
	for( size_t binNumber=1; binNumber<=lowBin; ++binNumber )
	{
		pHistogram_->Fill( pHistogram_->GetBinCenter(binNumber), event.weight()*weightPerEvent );
	}

}

const l1menu::ITrigger& l1menu::TriggerRatePlot::getTrigger() const
{
	return *pTrigger_;
}

float l1menu::TriggerRatePlot::findThreshold( float targetRate ) const
{
	//
	// Loop over all of the bins in the plot and find the first one
	// that is less than the requested rate.
	//
	int binNumber;
	for( binNumber=1; binNumber<pHistogram_->GetNbinsX(); ++binNumber )
	{
		if( pHistogram_->GetBinContent(binNumber)<targetRate ) break;
	}

	//if( binNumber==pHistogram_->GetNbinsX() ) throw std::runtime_error( "l1menu::TriggerRatePlot::findThreshold() was unable to find a threshold for the given rate" );

	// I now have the bin number of the bin after the point I'm looking
	// for. Now do a linear fit to interpolate between the bins using the
	// two bins for the point and the two after.
	std::vector<int> binNumberForLinearFit;
	binNumberForLinearFit.push_back( binNumber-2 );
	binNumberForLinearFit.push_back( binNumber-1 );
	binNumberForLinearFit.push_back( binNumber );
	binNumberForLinearFit.push_back( binNumber+1 );

	std::vector< std::pair<float,float> > dataPoints;
	for( auto& number : binNumberForLinearFit )
	{
		// Make sure all of of the bin numbers are valid
		if( number<1 ) number=1;
		else if( number>=pHistogram_->GetNbinsX() ) number=pHistogram_->GetNbinsX()-1;
		dataPoints.push_back( std::make_pair( pHistogram_->GetBinCenter(number), pHistogram_->GetBinContent(number) ));
	}

	// Now do a simple linear fit on the data points
	std::pair<float,float> slopeAndIntercept=l1menu::tools::simpleLinearFit( dataPoints );

	// The straight line formula "y=m*x+c" equates to
	// "targetRate=slopeAndIntercept.first*threshold+slopeAndIntercept.second"
	// so rearrange and return the interpolated threshold.
	return (targetRate-slopeAndIntercept.second)/slopeAndIntercept.first;
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
