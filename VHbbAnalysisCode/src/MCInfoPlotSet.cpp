#include "TrkUpgradeAnalysis/VHbb/interface/MCInfoPlotSet.h"

#include <stdexcept>
#include <sstream>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::MCInfoPlotSet::MCInfoPlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::MCInfoPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MCInfoPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//


	pEventString_=new TH1F( "mcEventType", "Event type from MC", 10, 0.5, 10.5 );
	pEventString_->SetDirectory(pDirectory);

	pNumberOfPrimaryVertices_=new TH1F( "numberOfPrimaryVertices", "Number of primary vertices", 71, -0.5, 70.5 );
	pNumberOfPrimaryVertices_->SetDirectory(pDirectory);

	pNumberOfBunchCrossings_=new TH1F( "numberOfBunchCrossings", "Number of bunch crossings", 71, -0.5, 70.5 );
	pNumberOfInteractionsPerBunchCrossing_=new TH1F( "numberOfInteractionsPerBunchCrossing", "Number of interactions per bunch crossing", 71, -0.5, 70.5 );
	pTotalInteractionsPerEvent_=new TH1F( "totalInteractionsPerEvent", "Total number of interactions per event", 71, -0.5, 70.5 );
	pNumberOfBunchCrossings_->SetDirectory(pDirectory);
	pNumberOfInteractionsPerBunchCrossing_->SetDirectory(pDirectory);
	pTotalInteractionsPerEvent_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::MCInfoPlotSet::fill( const VHbbEventAuxInfo& eventAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::MCInfoPlotSet::book() - histograms have not been booked" );

	std::stringstream eventStringStream;

	std::vector<VHbbEventAuxInfo::ParticleMCInfo> zMCInfo=eventAuxInfo.mcZ;
	std::vector<VHbbEventAuxInfo::ParticleMCInfo> higgsMCInfo=eventAuxInfo.mcH;

	for( std::vector<VHbbEventAuxInfo::ParticleMCInfo>::const_iterator higgsInfo=higgsMCInfo.begin(); higgsInfo!=higgsMCInfo.end(); ++higgsInfo )
	{
		if( higgsInfo->dauid.size()==1 && std::abs(higgsInfo->dauid[0])==25 ) continue;

		eventStringStream << "H->(";
		for( std::vector<int>::const_iterator daughterID=higgsInfo->dauid.begin(); daughterID!=higgsInfo->dauid.end(); ++daughterID )
		{
			if( daughterID!=higgsInfo->dauid.begin() ) eventStringStream << ",";
			eventStringStream << *daughterID;
		}
		eventStringStream << ") ";
	}

	for( std::vector<VHbbEventAuxInfo::ParticleMCInfo>::const_iterator zInfo=zMCInfo.begin(); zInfo!=zMCInfo.end(); ++zInfo )
	{
		if( zInfo->dauid.empty() ) continue;
		if( zInfo->dauid.size()==1 && std::abs(zInfo->dauid[0])==23 ) continue;
		if( zInfo->dauid.size()==2 && std::abs(zInfo->dauid[0])==23 && std::abs(zInfo->dauid[1])==25 ) continue;

		eventStringStream << "Z->(";
		for( std::vector<int>::const_iterator daughterID=zInfo->dauid.begin(); daughterID!=zInfo->dauid.end(); ++daughterID )
		{
			if( *daughterID==23 ) continue;
			if( zInfo->dauid.size()>2 && *daughterID==22 ) continue;
			if( daughterID!=zInfo->dauid.begin() ) eventStringStream << ",";
			eventStringStream << *daughterID;
		}
		eventStringStream << ") ";
	}

	// Now that I've created the string describing the MC truth type of the event, see
	// if it's already been used. If so find out which bin it's being plotted in. Note that
	// "binNumber" here isn't strictly speaking the bin number, it's the x-coord used to fill
	// the correct bin.
	int binNumber=0;
	std::map<std::string,int>::const_iterator iBinNumber=stringToBinMap_.find( eventStringStream.str() );
	if( iBinNumber==stringToBinMap_.end() )
	{
		// hasn't been plotted before, so I need to create an entry in the map
		// and use the next available bin.
		binNumber=stringToBinMap_.size()+1;
		stringToBinMap_[eventStringStream.str()]=binNumber;

		// Give the histogram bin the correct label
		pEventString_->GetXaxis()->SetBinLabel( binNumber, eventStringStream.str().c_str() );
	}
	else binNumber=iBinNumber->second;

	pEventString_->Fill( binNumber );

	pNumberOfPrimaryVertices_->Fill( eventAuxInfo.pvInfo.nVertices );

	// Loop over the pile up data
	unsigned int totalNumberOfInteractions=0;
	unsigned int numberOfBunchCrossings=0;
	for( std::map<int,unsigned int>::const_iterator iBXInteractionPair=eventAuxInfo.puInfo.pus.begin(); iBXInteractionPair!=eventAuxInfo.puInfo.pus.end(); ++iBXInteractionPair )
	{
		const unsigned int& numberOfInteractions=iBXInteractionPair->second;
		totalNumberOfInteractions+=numberOfInteractions;
		pNumberOfInteractionsPerBunchCrossing_->Fill( numberOfInteractions );
		++numberOfBunchCrossings;
	}
	pNumberOfBunchCrossings_->Fill( numberOfBunchCrossings );
	pTotalInteractionsPerEvent_->Fill( totalNumberOfInteractions );

}

