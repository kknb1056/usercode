#include "TrkUpgradeAnalysis/VHbb/interface/LightweightAnalyser.h"

#include <sstream>

#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"
#include "TrkUpgradeAnalysis/VHbb/interface/AdditionalJetStudyCutSets.h"

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"
#include "VHbbAnalysis/VHbbDataFormats/src/HbbCandidateFinderAlgo.cc" // Not entirely sure why this is included and not linked to
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


#include <TFile.h>
#include <TH1.h>
#include <TH2.h>


const bool trkupgradeanalysis::LightweightAnalyser::verbose_=false;
const double trkupgradeanalysis::LightweightAnalyser::jetPtThresholdZ_=20;
const double trkupgradeanalysis::LightweightAnalyser::jetPtThresholdW_=20;
const bool trkupgradeanalysis::LightweightAnalyser::useHighestPtHiggsZ_=false;
const bool trkupgradeanalysis::LightweightAnalyser::useHighestPtHiggsW_=true;

namespace // Use the unnamed namespace
{
	TDirectory* createDirectory( const std::string& fullPath, TDirectory* pParent )
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
}

trkupgradeanalysis::LightweightAnalyser::LightweightAnalyser( const std::string& outputFilename )
	: zFinder_( verbose_, jetPtThresholdZ_, useHighestPtHiggsZ_ ),
	  wFinder_( verbose_, jetPtThresholdW_, useHighestPtHiggsW_ ),
	  numberOfEvents_(0),
	  numberOfEventsWithAtLeastOneZOrWCandidate_(0),
	  pPlotsForEverything_(NULL)
{
	// All of these are empty smart pointers, so I need to call reset on them
	pOutputFile_.reset( new TFile( outputFilename.c_str(), "RECREATE" ) );


	using namespace trkupgradeanalysis; // Don't really need this but I like to be explicit (ooh err..)
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZee(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionWen(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionWmun(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumuWithoutAdditionalJetsCut(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(110)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(115)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(125)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(130)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(135)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(110-20,135+10)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumuWithCSVCutsSwitched(110-20,135+10)) );

	//
	// These are only temporary while I'm studying the number of additional jets
	//
	using namespace trkupgradeanalysis::additionaljetstudy;
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide2() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide3Plot1() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide3Plot2() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide3Plot3() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide4Plot2() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide5Plot1() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide5Plot2() ) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>( new WilkenSlide5Plot2AssumingTypo() ) );

	//
	// Don't need the following because I'm not studying the background estimation regions
	//

	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VlightRegionHWmun) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VlightRegionHWen) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VlightRegionHZmumu) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VlightRegionHZee) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new TTbarRegionHWmun) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new TTbarRegionHWen) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new TTbarRegionHZmumu) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new TTbarRegionHZee) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VbbRegionHWmun) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VbbRegionHWen) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VbbRegionHZmumu) );
	//cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new VbbRegionHZee) );
}

trkupgradeanalysis::LightweightAnalyser::~LightweightAnalyser()
{
	save();
	pOutputFile_->Close();
}

void trkupgradeanalysis::LightweightAnalyser::changeDirectory( const std::string& directory )
{
	std::map<std::string,PlotsSplitByMCType>::iterator iPlots=plotsForEventTypes_.find(directory);

	if( iPlots==plotsForEventTypes_.end() )
	{
		pPlotsForEverything_=&plotsForEventTypes_[directory]; // This will create a new entry in the map

		// Only book the histograms for all MC event types now. I don't know what MC event types there
		// until I look at the events so I need to wait until then
		bookHistograms( pPlotsForEverything_->allEventTypes_, directory+"/allEvents" );
	}
	else pPlotsForEverything_=&iPlots->second; // This directory has already been created and the histograms booked, so reuse those

	currentDirectory_=directory;
}

void trkupgradeanalysis::LightweightAnalyser::save()
{
	pOutputFile_->Write(0,TObject::kOverwrite);
}

void trkupgradeanalysis::LightweightAnalyser::bookHistograms( trkupgradeanalysis::LightweightAnalyser::AllPlots& plots, const std::string& directoryName )
{
	TDirectory* pRootDirectory=pOutputFile_->GetDirectory("/");
	TDirectory* pDirectory=createDirectory( directoryName, pRootDirectory );
//	TDirectory* pDirectory=pRootDirectory->GetDirectory( directoryName.c_str() );
//	if( pDirectory==NULL ) pDirectory=pRootDirectory->mkdir( directoryName.c_str() );

	TDirectory* pSubDirectory=pDirectory->mkdir( "VHbbCandidates" );

	plots.pNumberOfCandidates_=new TH2F( "numberOfCandidates","Number of candidates",5,-0.5,4.5,5,-0.5,4.5);
	plots.pNumberOfCandidates_->SetDirectory(pSubDirectory);
	plots.candidateHistogramsForAllEvents_.book( pSubDirectory );
	plots.pNumberOfCandidates_->GetXaxis()->SetTitle( "Z candidates" );
	plots.pNumberOfCandidates_->GetYaxis()->SetTitle( "W candidates" );

	pSubDirectory=pDirectory->mkdir( "MuonInfos" );
	plots.allMuons_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "ElectronInfos" );
	plots.allElectrons_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "MonteCarloTruth" );
	plots.monteCarloInfo_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "Cuts" );

	for( std::vector< boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> >::const_iterator iCutSet=cutsToApply_.begin();
			iCutSet!=cutsToApply_.end(); ++iCutSet )
	{
		TDirectory* pSubSubDirectory=pSubDirectory->mkdir( (*iCutSet)->name().c_str() );

		CutSetPlotSet newPlotSet( *iCutSet );
		newPlotSet.book( pSubSubDirectory );
		plots.cutCollectionPlotSets_.push_back( newPlotSet );
	}
}

void trkupgradeanalysis::LightweightAnalyser::fillAllPlotsStructure( AllPlots* pPlotsToFill, const VHbbEvent& vhbbEvent, const std::vector<VHbbCandidate>& zCandidates, const std::vector<VHbbCandidate>& wCandidates )
{
	pPlotsToFill->allMuons_.fill( vhbbEvent.muInfo );
	pPlotsToFill->allElectrons_.fill( vhbbEvent.eleInfo );

	for( std::vector<VHbbCandidate>::const_iterator iCandidate=zCandidates.begin(); iCandidate!=zCandidates.end(); ++iCandidate )
	{
		pPlotsToFill->candidateHistogramsForAllEvents_.fill( *iCandidate );

		// Loop over all of the cuts. If this candidate passes the cuts then fill the plots
		for( std::vector<CutSetPlotSet>::iterator iCutPlotSet=pPlotsToFill->cutCollectionPlotSets_.begin();
				iCutPlotSet!=pPlotsToFill->cutCollectionPlotSets_.end(); ++iCutPlotSet )
		{
			iCutPlotSet->fill( *iCandidate );
		}
	}

	pPlotsToFill->pNumberOfCandidates_->Fill( zCandidates.size(), wCandidates.size() );
}

void trkupgradeanalysis::LightweightAnalyser::processEvent( const fwlite::Event& event )
{
	std::vector<VHbbCandidate> zCandidates;
	std::vector<VHbbCandidate> wCandidates;

	fwlite::Handle<VHbbEvent> vhbbHandle;
	vhbbHandle.getByLabel( event, "HbbAnalyzerNew" );
	const VHbbEvent* pVHbbEvent=vhbbHandle.product();
	if( pVHbbEvent == 0 )
	{
		std::cerr << "Couldn't get the VHbbEvent" << std::endl;
		return;
	}

	AllPlots* pCurrentPlotsForThisEventType=NULL;
	fwlite::Handle<VHbbEventAuxInfo> vhbbAuxInfoHandle;
	vhbbAuxInfoHandle.getByLabel( event, "HbbAnalyzerNew" );
	const VHbbEventAuxInfo* pVHbbEventAuxInfo=vhbbAuxInfoHandle.product();
	if( pVHbbEventAuxInfo == 0 ) std::cerr << "Couldn't get the VHbbEventAuxInfo" << std::endl;
	else
	{
		pPlotsForEverything_->allEventTypes_.monteCarloInfo_.fill( *pVHbbEventAuxInfo );
		std::string eventType=eventTypeFromMC( *pVHbbEventAuxInfo );

		std::map<std::string,AllPlots>::iterator iPlotsForThisEventType=pPlotsForEverything_->mcEventTypePlots_.find(eventType);

		if( iPlotsForThisEventType==pPlotsForEverything_->mcEventTypePlots_.end() )
		{
			// This event type hasn't been encountered before so I need to book the histograms
			AllPlots& plotsForThisEventType=pPlotsForEverything_->mcEventTypePlots_[eventType];
			bookHistograms( plotsForThisEventType, currentDirectory_+"/"+eventType );
			pCurrentPlotsForThisEventType=&plotsForThisEventType;
		}
		else pCurrentPlotsForThisEventType=&(iPlotsForThisEventType->second);
	}

	++numberOfEvents_;

	zFinder_.run( pVHbbEvent, zCandidates );
	wFinder_.run( pVHbbEvent, wCandidates );

	fillAllPlotsStructure( &pPlotsForEverything_->allEventTypes_, *pVHbbEvent, zCandidates, wCandidates );
	if( pCurrentPlotsForThisEventType!=NULL ) fillAllPlotsStructure( pCurrentPlotsForThisEventType, *pVHbbEvent, zCandidates, wCandidates );


	if( !zCandidates.empty() || !wCandidates.empty() ) ++numberOfEventsWithAtLeastOneZOrWCandidate_;

}

void trkupgradeanalysis::LightweightAnalyser::processFile( TFile* pInputFile )
{
	if( pPlotsForEverything_==NULL ) changeDirectory("allEvents");

	fwlite::Event event( pInputFile );
	for( event.toBegin(); !event.atEnd(); ++event )
	{
		processEvent( event );
	}

	std::cout << "There were " << numberOfEventsWithAtLeastOneZOrWCandidate_ << " events with a Z or W candidate out of " << numberOfEvents_ << " events." << std::endl;
}

std::string trkupgradeanalysis::LightweightAnalyser::eventTypeFromMC( const VHbbEventAuxInfo& eventAuxInfo )
{
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

	if( eventStringStream.str().empty() ) return "otherBackground";
	else return eventStringStream.str();
}

