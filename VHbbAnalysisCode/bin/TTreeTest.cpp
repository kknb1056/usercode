#include <iostream>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for TFile_auto_ptr
#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidateCutSets.h"
#include "TrkUpgradeAnalysis/VHbb/interface/CutSetPlotSet.h"

void printUsage( const std::string& name );
void processFile( const trkupgradeanalysis::tools::TFile_auto_ptr& pInputFile );
void processCutDirectory( const std::string& cutDirectory, const trkupgradeanalysis::tools::TFile_auto_ptr& pInputFile, boost::scoped_ptr<TFile>& pOutputFile );

int main( int argc, char* argv[] )
{
	std::vector<std::string> filenames;

	try
	{
		for( int a=1; a<argc; ++a )
		{
			if( std::string(argv[a])=="-h" )
			{
				printUsage( argv[0] );
				return 0;
			}

			// If the argument doesn't match anything tried above then treat it as an input filename
			filenames.push_back( argv[a] );
		}

		if( filenames.empty() ) throw std::runtime_error("No input files have been specified.");
	}
	catch( std::exception& error )
	{
		std::cerr << "Error parsing the command line arguments: " << error.what() << std::endl;
		printUsage( argv[0] );
		return -1;
	}

	std::cout << "Starting" << std::endl;
	for( std::vector<std::string>::const_iterator iFilename=filenames.begin(); iFilename!=filenames.end(); ++iFilename )
	{
		try // Use another try block so that processing can continue on the other files if one fails
		{
			trkupgradeanalysis::tools::TFile_auto_ptr pInputFile( *iFilename );
			if( pInputFile==NULL ) throw std::runtime_error("Couldn't open the input file");

			processFile( pInputFile );
		}
		catch( std::exception& exception )
		{
			std::cerr << "Exception while processing file " << *iFilename << ": " << exception.what() << std::endl;
		}
	}

	std::cout << "Finished" << std::endl;
	return 1;
}

void printUsage( const std::string& name )
{
	std::cout << "Usage:" << std::endl;
}

void processFile( const trkupgradeanalysis::tools::TFile_auto_ptr& pInputFile )
{
	boost::scoped_ptr<TFile> pOutputFile( new TFile( "testFile.root", "RECREATE" ) );

	std::vector<std::string> directoryNames;

	directoryNames.push_back( "/ZH-ZToLL-HToBB/StdGeom-TuneZ2Star/50PU/H->(5,-5) Z->(-13,13) " );
	directoryNames.push_back( "/ZH-ZToLL-HToBB/StdGeom-TuneZ2Star/50PU/H->(5,-5) Z->(-11,11) " );
	directoryNames.push_back( "/DYToEE/StdGeom-TuneZ2Star/50PU/allEvents" );
	directoryNames.push_back( "/DYToMuMu/StdGeom-TuneZ2Star/50PU/allEvents" );
	directoryNames.push_back( "/TTbar/StdGeom-TuneZ2Star/50PU/allEvents" );

	directoryNames.push_back( "/ZH-ZToLL-HToBB/Phase1R30-TuneZ2Star/50PU/H->(5,-5) Z->(-13,13) " );
	directoryNames.push_back( "/ZH-ZToLL-HToBB/Phase1R30-TuneZ2Star/50PU/H->(5,-5) Z->(-11,11) " );
	directoryNames.push_back( "/DYToEE/Phase1R30-TuneZ2Star/50PU/allEvents" );
	directoryNames.push_back( "/DYToMuMu/Phase1R30-TuneZ2Star/50PU/allEvents" );
	directoryNames.push_back( "/TTbar/Phase1R30-TuneZ2Star/50PU/allEvents" );

	for( std::vector<std::string>::const_iterator iDirectoryName=directoryNames.begin(); iDirectoryName!=directoryNames.end(); ++iDirectoryName )
	{
		try{ processCutDirectory( *iDirectoryName, pInputFile, pOutputFile ); }
		catch( std::exception& exception )
		{
			std::cerr << "Exception while processing directory " << *iDirectoryName << ": " << exception.what() << std::endl;
		}
	}

	pOutputFile->Write();
	pOutputFile->Close();
}

void processCutDirectory( const std::string& cutDirectory, const trkupgradeanalysis::tools::TFile_auto_ptr& pInputFile, boost::scoped_ptr<TFile>& pOutputFile )
{
	TTree* pTree=(TTree*) pInputFile->Get( (cutDirectory+"/Cuts/AllVariables/additionalVariables").c_str() );
	if( !pTree ) throw std::runtime_error("Unable to get the TTree \""+cutDirectory+"/Cuts/AllVariables/additionalVariables\".");

	{ // Block to show these variables are only used once
		// Copy over these histograms as well because they're used to see how many events have been analysed
		TH1F* pSourceHistogram=(TH1F*)pInputFile->Get( (cutDirectory+"/MuonInfos/numberOfMuons").c_str() );
		TDirectory* pNewDirectory=trkupgradeanalysis::tools::createDirectory( cutDirectory+"/MuonInfos", pOutputFile.get() );
		TH1F* pHistogramCopy=new TH1F( *pSourceHistogram );
		pHistogramCopy->SetDirectory( pNewDirectory );
	}

	trkupgradeanalysis::tools::NTupleRow ntupleRow( pTree );

	// Decide which btag cuts to use depending on whether there is "Phase1" or "StdGeom" in the directory name
	double highBTagCut=-1;
	double lowBTagCut=-1;
	if( cutDirectory.find("StdGeom")!=std::string::npos )
	{
//		highBTagCut=0.719612; // old tracking DUS
//		lowBTagCut=0.460623; // old tracking DUS
//		highBTagCut=0.717621; // old tracking DUSG
//		lowBTagCut=0.395486; // old tracking DUSG
		highBTagCut=0.741415; // new tracking DUS
		lowBTagCut=0.446474; // new tracking DUS
//		highBTagCut=0.709339; // new tracking DUSG
//		lowBTagCut=0.388739; // new tracking DUSG
	}
	else if( cutDirectory.find("Phase1")!=std::string::npos )
	{
//		highBTagCut=0.778472; // old tracking DUS
//		lowBTagCut=0.519269; // old tracking DUS
//		highBTagCut=0.776769; // old tracking DUSG
//		lowBTagCut=0.536104; // old tracking DUSG
		highBTagCut=0.787815; // new tracking DUS
		lowBTagCut=0.517352; // new tracking DUS
//		highBTagCut=0.757648; // new tracking DUSG
//		lowBTagCut=0.532425; // new tracking DUSG
	}
	else throw std::runtime_error("Unable to decide whether to use StdGeom btag cuts or Phase1 btag cuts");

	TDirectory* pCutsDirectory;

	trkupgradeanalysis::CutSetPlotSet FullSelectionZmumuNoTrigger( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet>(new trkupgradeanalysis::ParameterisedZmumu(highBTagCut,lowBTagCut,75,105,140,170)), false );
	pCutsDirectory=trkupgradeanalysis::tools::createDirectory( cutDirectory+"/Cuts/FullSelectionZmumuNoTrigger", pOutputFile.get() );
	FullSelectionZmumuNoTrigger.book( pCutsDirectory );

	trkupgradeanalysis::CutSetPlotSet FullSelectionZmumuSingleTrigger( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet>(new trkupgradeanalysis::ParameterisedZmumu(highBTagCut,lowBTagCut,75,105,140,170,true)), false );
	pCutsDirectory=trkupgradeanalysis::tools::createDirectory( cutDirectory+"/Cuts/FullSelectionZmumuSingleTrigger", pOutputFile.get() );
	FullSelectionZmumuSingleTrigger.book( pCutsDirectory );

	trkupgradeanalysis::CutSetPlotSet FullSelectionZmumuDoubleTrigger( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet>(new trkupgradeanalysis::ParameterisedZmumu(highBTagCut,lowBTagCut,75,105,140,170,true,true)), false );
	pCutsDirectory=trkupgradeanalysis::tools::createDirectory( cutDirectory+"/Cuts/FullSelectionZmumuDoubleTrigger", pOutputFile.get() );
	FullSelectionZmumuDoubleTrigger.book( pCutsDirectory );

	trkupgradeanalysis::CutSetPlotSet FullSelectionZee( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet>(new trkupgradeanalysis::ParameterisedZee(highBTagCut,lowBTagCut,75,105,140,170)), false );
	pCutsDirectory=trkupgradeanalysis::tools::createDirectory( cutDirectory+"/Cuts/FullSelectionZee", pOutputFile.get() );
	FullSelectionZee.book( pCutsDirectory );

	while( ntupleRow.nextRow() )
	{
		FullSelectionZmumuNoTrigger.fill(ntupleRow);
		FullSelectionZmumuSingleTrigger.fill(ntupleRow);
		FullSelectionZmumuDoubleTrigger.fill(ntupleRow);
		FullSelectionZee.fill(ntupleRow);
	}
}
