#include <stdexcept>
#include <iostream>

#include <TSystem.h>
#include <FWCore/FWLite/interface/AutoLibraryLoader.h>
#include <TFile.h>
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/MenuRatePlots.h"


int main( int argc, char* argv[] )
{
	gSystem->Load("libFWCoreFWLite.so");
	AutoLibraryLoader::enable();

	if( argc!=2 )
	{
		std::string executableName=argv[0];
		size_t lastSlashPosition=executableName.find_last_of('/');
		if( lastSlashPosition!=std::string::npos ) executableName=executableName.substr( lastSlashPosition+1, std::string::npos );
		std::cerr << "   Usage: " << executableName << " <reduced sample filename>" << std::endl;
		return -1;
	}

	std::string sampleFilename=argv[1];

//	try
//	{
		l1menu::ReducedMenuSample mySample( sampleFilename );

		l1menu::MenuRatePlots rateVersusThresholdPlots( mySample.getTriggerMenu() );
		rateVersusThresholdPlots.initiateForReducedSample( mySample );

		for( size_t eventNumber=0; eventNumber<mySample.numberOfEvents(); ++eventNumber )
		{
			const l1menu::IReducedEvent& event=mySample.getEvent( eventNumber );
			rateVersusThresholdPlots.addEvent( event );
		}

		std::unique_ptr<TFile,void(*)(TFile*)> pMyRootFile( new TFile( "reducedRateHistograms.root", "RECREATE" ), [](TFile*p){p->Write();p->Close();delete p;} );
		rateVersusThresholdPlots.setDirectory( pMyRootFile.get() );
		rateVersusThresholdPlots.relinquishOwnershipOfPlots();
//	}
//	catch( std::exception& error )
//	{
//		std::cerr << "Exception caught: " << error.what() << std::endl;
//		return -1;
//	}

	return 0;
}
