#include "TrkUpgradeAnalysis/VHbb/interface/LightweightAnalyser.h"

#include <sstream>

#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"
#include "TrkUpgradeAnalysis/VHbb/interface/AdditionalJetStudyCutSets.h"
#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for trkupgradeanalysis::tools::createDirectory
#include "FWCore/Utilities/interface/EDMException.h"

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"
#include "VHbbAnalysis/VHbbDataFormats/src/HbbCandidateFinderAlgo.cc" // Not entirely sure why this is included and not linked to
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"


#include <TFile.h>
#include <TH1.h>
#include <TH2.h>


const bool trkupgradeanalysis::LightweightAnalyser::verbose_=false;
const double trkupgradeanalysis::LightweightAnalyser::jetPtThresholdZ_=30;
const double trkupgradeanalysis::LightweightAnalyser::jetPtThresholdW_=30;
const bool trkupgradeanalysis::LightweightAnalyser::useHighestPtHiggsZ_=false;
const bool trkupgradeanalysis::LightweightAnalyser::useHighestPtHiggsW_=true;


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
//	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZee(120)) );
//	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionZmumu(120)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new AllVariables) );
//	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionStdGeom50PUZmumuWithoutAdditionalJetsCut(125)) );
//	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new SignalSelectionPhase150PUZmumuWithoutAdditionalJetsCut(125)) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new InitialSignalSelectionZee) );
	cutsToApply_.push_back( boost::shared_ptr<VHbbCandidateCutSet>(new InitialSignalSelectionZmumu) );
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
	TDirectory* pDirectory=trkupgradeanalysis::tools::createDirectory( directoryName, pRootDirectory );
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

	pSubDirectory=pDirectory->mkdir( "HiggsCandidateJets" );
	plots.higgsCandidateJets_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "AdditionalJets" );
	plots.additionalJets_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "SimpleJets2" );
	plots.simpleJets2_.book( pSubDirectory );

	pSubDirectory=pDirectory->mkdir( "IsolationStudy" );
	plots.isolationStudy_.book( pSubDirectory );

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

void trkupgradeanalysis::LightweightAnalyser::fillAllPlotsStructure( AllPlots* pPlotsToFill, const VHbbEvent& vhbbEvent, const VHbbEventAuxInfo* pVHbbEventAuxInfo, const std::vector<VHbbCandidate>& zCandidates, const std::vector<VHbbCandidate>& wCandidates )
{
	pPlotsToFill->allMuons_.fill( vhbbEvent.muInfo, pVHbbEventAuxInfo );
	pPlotsToFill->allElectrons_.fill( vhbbEvent.eleInfo );
	if( pVHbbEventAuxInfo ) pPlotsToFill->monteCarloInfo_.fill( *pVHbbEventAuxInfo );
	pPlotsToFill->isolationStudy_.fill( vhbbEvent, pVHbbEventAuxInfo ); // This class checks if is null itself
	pPlotsToFill->simpleJets2_.fill( vhbbEvent.simpleJets2, pVHbbEventAuxInfo );

	for( std::vector<VHbbCandidate>::const_iterator iCandidate=zCandidates.begin(); iCandidate!=zCandidates.end(); ++iCandidate )
	{
		pPlotsToFill->candidateHistogramsForAllEvents_.fill( *iCandidate );
		pPlotsToFill->higgsCandidateJets_.fill( iCandidate->H.jets, pVHbbEventAuxInfo );
		pPlotsToFill->additionalJets_.fill( iCandidate->additionalJets, pVHbbEventAuxInfo );

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

	bool applyMuonIsolation=false;
	// Apply isolation to the muons. Need to modify so use a very naughty const cast
	if( applyMuonIsolation )
	{
		std::vector<VHbbEvent::MuonInfo>& muonInfos=const_cast< std::vector<VHbbEvent::MuonInfo>& >( pVHbbEvent->muInfo );
		bool allMuonsPassIsolationCut=false;
		while( !allMuonsPassIsolationCut )
		{
			// I'm worried about looping using invalidated iterators, so whenever I erase an element I'll
			// start looping from the start. Not the most efficient way but these vectors typically have
			// at most 3 entries.
			std::vector<VHbbEvent::MuonInfo>::iterator iMuon;
			for( iMuon=muonInfos.begin(); iMuon!=muonInfos.end(); ++iMuon )
			{
				float isolation=MuonInfoPlotSet::thirdRho25CorrectedIsolation( *iMuon, pVHbbEventAuxInfo->puInfo.rho25 );
				if( isolation>=0.15 ) break;
			}
			if( iMuon!=muonInfos.end() ) muonInfos.erase( iMuon );
			else allMuonsPassIsolationCut=true;
		}
	}

	zFinder_.run( pVHbbEvent, zCandidates );
	wFinder_.run( pVHbbEvent, wCandidates );

	fillAllPlotsStructure( &pPlotsForEverything_->allEventTypes_, *pVHbbEvent, pVHbbEventAuxInfo, zCandidates, wCandidates );
	if( pCurrentPlotsForThisEventType!=NULL ) fillAllPlotsStructure( pCurrentPlotsForThisEventType, *pVHbbEvent, pVHbbEventAuxInfo, zCandidates, wCandidates );


	if( !zCandidates.empty() || !wCandidates.empty() ) ++numberOfEventsWithAtLeastOneZOrWCandidate_;

}

void trkupgradeanalysis::LightweightAnalyser::processFile( TFile* pInputFile, size_t numberOfEvents )
{
	if( pPlotsForEverything_==NULL ) changeDirectory("allEvents");

	fwlite::Event event( pInputFile );

	size_t eventCount=0;
	for( event.toBegin(); !event.atEnd(); ++event )
	{
		try
		{
			processEvent( event );
			++eventCount;
		}
		catch( edm::Exception& error )
		{
			std::cerr << error.what() << std::endl;
		}

		if( numberOfEvents!=0 && eventCount>=numberOfEvents )
		{
			std::cout << "Breaking after " << eventCount << " events" << std::endl;
			break;
		}
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

