#include "TrkUpgradeAnalysis/VHbb/interface/tools.h"

#include <stdexcept>
#include <iostream>

#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TDirectory.h>

trkupgradeanalysis::tools::TFile_auto_ptr::TFile_auto_ptr( const std::string& filename )
{
	this->reset( TFile::Open( filename.c_str() ) );
}

trkupgradeanalysis::tools::TFile_auto_ptr::~TFile_auto_ptr()
{
	// If the pointer is not null, close the file. Let the auto_ptr destructor delete it.
	if( this->get() ) this->get()->Close();
}

bool trkupgradeanalysis::tools::TFile_auto_ptr::operator==( const TFile* pOtherFile )
{
	return this->get()==pOtherFile;
}





trkupgradeanalysis::tools::NTupleRow::NTupleRow( TTree* pTree )
	: pTree_(pTree)
{
	TObjArray* pLeafList=pTree->GetListOfBranches();

	for( int a=0; a<pLeafList->GetEntries(); ++a )
	{
		TBranch* pBranch=(TBranch*)(*pLeafList)[a];
		std::string name=pBranch->GetName();
		std::string title=pBranch->GetTitle();
		std::string typeString=title.substr( name.size() );

		if( typeString=="/D" ) branchAddresses_[name]=0; // Put an entry in the map for this name
	}

	// Now loop over the map names and set the branch addresses. I didn't want to do this earlier
	// because there's the possibility adding new entries to the map might change the addresses.
	for( std::map<std::string,double>::iterator iEntry=branchAddresses_.begin(); iEntry!=branchAddresses_.end(); ++iEntry )
	{
		pTree->SetBranchAddress( iEntry->first.c_str(), &(iEntry->second) );
	}

	maxCandidateNumber_=pTree_->GetEntries();
	candidateNumber_=-1; // Set to minus one, so that the first call to nextCandidate() sets it to the first (0) entry
	pTree_->GetEntry(0); // Load in the first candidate so that the memory contents isn't random. Note the first call to nextCandidate() will keep it on the 0th entry.
}

const double& trkupgradeanalysis::tools::NTupleRow::getDouble( const std::string& name ) const
{
	std::map<std::string,double>::const_iterator iFindResult=branchAddresses_.find(name);
	if( iFindResult!=branchAddresses_.end() ) return iFindResult->second;
	else throw std::runtime_error( "trkupgradeanalysis::tools::NTupleRow::getDouble(..) - TTree doesn't have a branch called "+name );
}

bool trkupgradeanalysis::tools::NTupleRow::nextRow()
{
	++candidateNumber_;
	if( candidateNumber_>=maxCandidateNumber_ ) return false;

	pTree_->GetEntry(candidateNumber_);
	return true;
}

void trkupgradeanalysis::tools::NTupleRow::returnToStart()
{
	candidateNumber_=-1; // Set to minus one, so that the first call to nextCandidate() sets it to the first (0) entry
}






float trkupgradeanalysis::tools::linearInterpolate( std::pair<float,float> point1, std::pair<float,float> point2, float requiredY )
{
	// Using the equation for a straight line of y=mx+c, work out m and c so
	// that the line passes through point1 and point2
	float m=(point1.second-point2.second)/(point1.first-point2.first);
	float c=point2.second-m*point2.first;

	// Then I can return x=(y-c)/m
	return (requiredY-c)/m;
}






float trkupgradeanalysis::tools::findDiscriminator( const TH1F* pEventsVersusDiscriminatorHistogram, const float requiredEfficiency, const bool verbose )
{
	if( pEventsVersusDiscriminatorHistogram==NULL ) throw std::runtime_error("findDiscriminator was called with a NULL histogram pointer");

	TAxis* pHistogramXAxis=pEventsVersusDiscriminatorHistogram->GetXaxis();
	float accumulatedEvents=0; // The number of events that would pass the current cut (i.e. number of events in this bin and all higher ones)
	float previousEfficiency=1; // The efficiency on the last run through the loop. Set high for logic to work

	// Loop backwards over each of the bins in the histogram
	for( int bin=pHistogramXAxis->GetNbins(); bin>=1; --bin )
	{
		accumulatedEvents+=pEventsVersusDiscriminatorHistogram->GetBinContent(bin);

		float currentEfficiency=accumulatedEvents/pEventsVersusDiscriminatorHistogram->GetEntries();
		// See if this loop has gone a bin too far. If so interpolate between this and the previous loop.
		if( currentEfficiency>requiredEfficiency )
		{
			// The working point is somewhere between the previous bin and this bin.
			// Do a linear interpolation between the bins.
			std::pair<float,float> point1( pHistogramXAxis->GetBinLowEdge(bin), currentEfficiency );
			std::pair<float,float> point2( pHistogramXAxis->GetBinLowEdge(bin+1), previousEfficiency );
			float operatingPoint=trkupgradeanalysis::tools::linearInterpolate( point1, point2, requiredEfficiency );

			if( verbose )
			{
				std::cout << "[findDiscriminator verbose] The operating point of " << requiredEfficiency << " efficiency for \"" << pEventsVersusDiscriminatorHistogram->GetName() << "\" is between:" << "\n"
						<< "[findDiscriminator verbose] \t" << pHistogramXAxis->GetBinLowEdge(bin) << " -> " << currentEfficiency << " (bin " << bin << ") and " << "\n"
						<< "[findDiscriminator verbose] \t" << pHistogramXAxis->GetBinLowEdge(bin+1) << " -> " << previousEfficiency << " (bin " << bin+1 << ")." << "\n"
						<< "[findDiscriminator verbose] Linear interpolation gives:" << "\n"
						<< "[findDiscriminator verbose] \t" << operatingPoint << std::endl;
			}

			return operatingPoint;
		}
		previousEfficiency=currentEfficiency; // Set this ready for the next loop around

	} // end of backwards loop over histogram bins

	// If flow has got this far, something has gone wrong
	throw std::runtime_error("findDiscriminator was unable to find an operating point");

} // end of function findDiscriminator







float trkupgradeanalysis::tools::findEfficiency( const TH1F* pEventsVersusDiscriminatorHistogram, const float requiredDiscriminator, const bool verbose )
{
	if( pEventsVersusDiscriminatorHistogram==NULL ) throw std::runtime_error("findEfficiency was called with a NULL histogram pointer");

	TAxis* pHistogramXAxis=pEventsVersusDiscriminatorHistogram->GetXaxis();
	float accumulatedEvents=0; // The number of events that would pass the current cut (i.e. number of events in this bin and all higher ones)
	float previousEfficiency=1; // The efficiency on the last run through the loop. Set high for logic to work

	// Loop backwards over each of the bins in the histogram
	for( int bin=pHistogramXAxis->GetNbins(); bin>=1; --bin )
	{
		accumulatedEvents+=pEventsVersusDiscriminatorHistogram->GetBinContent(bin);

		float currentEfficiency=accumulatedEvents/pEventsVersusDiscriminatorHistogram->GetEntries();
		// See if this loop step has gone too far, if so then interpolate between this and the previous step.
		if( pHistogramXAxis->GetBinLowEdge(bin)<requiredDiscriminator )
		{
			// The efficiency is somewhere between the previous bin and this bin.
			// Do a linear interpolation between the bins. This is similar to findDiscriminator but
			// I've switched around the x and y coordinates.
			std::pair<float,float> point1( currentEfficiency, pHistogramXAxis->GetBinLowEdge(bin) );
			std::pair<float,float> point2( previousEfficiency, pHistogramXAxis->GetBinLowEdge(bin+1) );
			float efficiency=trkupgradeanalysis::tools::linearInterpolate( point1, point2, requiredDiscriminator );

			if( verbose )
			{
				std::cout << "[findEfficiency verbose] The discriminator of " << requiredDiscriminator << " for \"" << pEventsVersusDiscriminatorHistogram->GetName() << "\" is between:" << "\n"
						<< "[findEfficiency verbose] \t" << pHistogramXAxis->GetBinLowEdge(bin) << " -> " << currentEfficiency << " (bin " << bin << ") and " << "\n"
						<< "[findEfficiency verbose] \t" << pHistogramXAxis->GetBinLowEdge(bin+1) << " -> " << previousEfficiency << " (bin " << bin+1 << ")." << "\n"
						<< "[findEfficiency verbose] Linear interpolation gives an efficiency of:" << "\n"
						<< "[findEfficiency verbose] \t" << efficiency << std::endl;
			}

			return efficiency;
		}
		previousEfficiency=currentEfficiency; // Set this ready for the next loop around

	} // end of backwards loop over histogram bins

	// If flow has got this far, something has gone wrong
	throw std::runtime_error("findEfficiency was unable to find an operating point");

} // end of function findEfficiency



TDirectory* trkupgradeanalysis::tools::createDirectory( const std::string& fullPath, TDirectory* pParent )
{
	if( pParent==NULL ) throw std::runtime_error( "The parent directory is a Null pointer" );

	TDirectory* pSubDirectory=pParent;
	size_t currentPosition=0;
	size_t nextSlash;
	do
	{
		nextSlash=fullPath.find_first_of('/', currentPosition );
		std::string directoryName=fullPath.substr(currentPosition,nextSlash-currentPosition);
		currentPosition=nextSlash+1;

		TDirectory* pNextSubDirectory=pSubDirectory->GetDirectory( directoryName.c_str() );
		if( pNextSubDirectory==NULL ) pNextSubDirectory=pSubDirectory->mkdir( directoryName.c_str() );
		if( pNextSubDirectory==NULL ) throw std::runtime_error( "Couldn't create the root directory \""+directoryName+"\"" );
		pSubDirectory=pNextSubDirectory;

	} while( nextSlash!=std::string::npos );

	return pSubDirectory;
}
