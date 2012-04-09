#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>

#include "TrkUpgradeAnalysis/VHbb/interface/NTupleCandidateProxy.h"

void printUsage( const std::string& programName )
{
	std::cout << programName << " inputFile1 [inputFile2 ....]" << std::endl;
}

//void processTree( TTree* pTree )
//{
//	int naJets;
//	bool aJet_id[30];
//	float aJet_eta[30];
//	float aJet_pt[30];
//	pTree->SetBranchAddress("naJets",&naJets);
//	pTree->SetBranchAddress("aJet_id",&aJet_id);
//	pTree->SetBranchAddress("aJet_eta",&aJet_eta);
//	pTree->SetBranchAddress("aJet_pt",&aJet_pt);
//
//	for( int a=0; a<pTree->GetEntries(); ++a )
//	{
//		pTree->GetEntry(a);
//		int Naj=0;
//		for( int b=0; b<naJets && b<30; ++b )
//		{
////			std::cout << "\t" << "pT=" << aJet_pt[b] << " eta=" << aJet_eta[b] << " id=" << aJet_id[b] << std::endl;
//			if( aJet_id[b] == 1 && std::fabs(aJet_eta[b]) < 2.5 && aJet_pt[b] > 20) Naj++;
//		}
//		std::cout << a << ": " << naJets << " jets - " << Naj << std::endl;
//	}
//}

void processFile( TFile* pInputFile )
{
	TTree* pTree=(TTree*) pInputFile->Get( "tree" );
	if( !pTree ) throw std::runtime_error("Unable to get the TTree.");

//	processTree( pTree );
	trkupgradeanalysis::NTupleCandidateProxy myCandidateProxy( pTree );

	TFile* pOutputFile=TFile::Open( "ntupleAnalyserOutput.root", "RECREATE" );
	TH1* pNumberOfRawAdditionalJets=new TH1F( "numberOfRawAdditionalJets", "numberOfRawAdditionalJets", 41, -0.5, 40.5 );
	TH1* pNumberOfAdditionalJets=new TH1F( "numberOfAdditionalJets", "numberOfAdditionalJets", 41, -0.5, 40.5 );
	TH1* pDifferenceBetweenRawAndCleanedAdditionalJets=new TH1F( "differenceBetweenRawAndCleanedAdditionalJets", "differenceBetweenRawAndCleanedAdditionalJets", 41, -0.5, 40.5 );

	TH1* pWilkenSlide4Plot1=new TH1F( "WilkenSlide4Plot1", "WilkenSlide4Plot1", 41, -0.5, 40.5 );
	TH1* pWilkenSlide4Plot2=new TH1F( "WilkenSlide4Plot2", "WilkenSlide4Plot2", 41, -0.5, 40.5 );
	TH1* pWilkenSlide5Plot1=new TH1F( "WilkenSlide5Plot1", "WilkenSlide5Plot1", 41, -0.5, 40.5 );
//	TH1* pWilkenSlide4Plot1=new TH1F( "WilkenSlide4Plot1", "WilkenSlide4Plot1", 41, -0.5, 40.5 );

//	pNumberOfRawAdditionalJets->SetDirectory(pOutputFile);
//	pNumberOfAdditionalJets->SetDirectory(pOutputFile);
//	pDifferenceBetweenRawAndCleanedAdditionalJets->SetDirectory(pOutputFile);

	size_t count=0;
	while( myCandidateProxy.nextCandidate() /*&& count<1000*/ )
	{
		++count;
		int numberOfRawAdditionalJets=myCandidateProxy.numberOfRawAdditionalJets();
		int numberOfAdditionalJets=myCandidateProxy.numberOfAdditionalJets();
		pNumberOfRawAdditionalJets->Fill(numberOfRawAdditionalJets);
		pNumberOfAdditionalJets->Fill(numberOfAdditionalJets);
		pDifferenceBetweenRawAndCleanedAdditionalJets->Fill(numberOfRawAdditionalJets-numberOfAdditionalJets);

		if( myCandidateProxy.candidateType()==0 )
		{
			float zMass=myCandidateProxy.zMass();
			pWilkenSlide4Plot1->Fill(numberOfRawAdditionalJets);
			pWilkenSlide5Plot1->Fill(numberOfAdditionalJets);

			if( myCandidateProxy.numberOfHiggsCandidateJets()>=2 )
			{
				if( myCandidateProxy.csvOfHiggsCandidateJet(0)>-1 && myCandidateProxy.csvOfHiggsCandidateJet(1)>-1 && 70<zMass && zMass<110 ) pWilkenSlide4Plot2->Fill(numberOfRawAdditionalJets);
//				if( myCandidateProxy.csvOfHiggsCandidateJet(0)>0.244 && myCandidateProxy.csvOfHiggsCandidateJet(1)>0.244
//						&& 75<zMass && zMass<105
//						&& ) pWilkenSlide4Plot2->Fill(numberOfRawAdditionalJets);
			}
		}
	}

	pOutputFile->Write();
	pOutputFile->Close();
}

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
		try
		{
			TFile* pInputFile=TFile::Open( iFilename->c_str() );
			if( pInputFile == 0 )
			{
				std::cerr << "Couldn't open the input file" << std::endl;
				continue;
			}

			processFile( pInputFile );
		}
		catch( std::exception& error )
		{
			std::cerr << "Error analysing file \"" << *iFilename << "\": " << error.what() << std::endl;
		}
	}

	std::cout << "Finished" << std::endl;
	return 1;
}
