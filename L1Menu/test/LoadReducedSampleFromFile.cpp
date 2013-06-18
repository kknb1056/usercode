#include <stdexcept>
#include <iostream>

#include <TSystem.h>
#include <FWCore/FWLite/interface/AutoLibraryLoader.h>
#include <TFile.h>
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/MenuRatePlots.h"
#include "l1menu/TriggerRates.h"

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

		std::cout << __LINE__ << std::endl;
		l1menu::MenuRatePlots rateVersusThresholdPlots( mySample.getTriggerMenu() );
		std::cout << __LINE__ << std::endl;
		rateVersusThresholdPlots.initiateForReducedSample( mySample );

		std::cout << "Calculating rate plots..." << std::endl;
		for( size_t eventNumber=0; eventNumber<mySample.numberOfEvents(); ++eventNumber )
		{
			const l1menu::IReducedEvent& event=mySample.getEvent( eventNumber );
			rateVersusThresholdPlots.addEvent( event );
		}

		std::unique_ptr<TFile,void(*)(TFile*)> pMyRootFile( new TFile( "reducedRateHistograms.root", "RECREATE" ), [](TFile*p){p->Write();p->Close();delete p;} );
		rateVersusThresholdPlots.setDirectory( pMyRootFile.get() );
		rateVersusThresholdPlots.relinquishOwnershipOfPlots();

		std::cout << "Calculating fractions..." << std::endl;
		const l1menu::TriggerRates rates=mySample.rate();

		std::cout << "Total fraction is " << rates.totalFraction() << std::endl;
		for( const auto& fraction : rates.fractions() )
		{
			std::cout << "Trigger has fraction " << fraction << std::endl;
		}
//	}
//	catch( std::exception& error )
//	{
//		std::cerr << "Exception caught: " << error.what() << std::endl;
//		return -1;
//	}

	return 0;
}
