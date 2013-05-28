#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "l1menu/MenuSample.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/TriggerTable.h"
#include "l1menu/ITrigger.h"
#include "l1menu/MenuRatePlots.h"
#include <iostream>
#include <string>
#include <stdexcept>

#include <TFile.h>
#include <TH1.h>

int main( int argc, char* argv[] )
{
	gSystem->Load("libFWCoreFWLite.so");
	AutoLibraryLoader::enable();

	if( argc!=3 )
	{
		std::string executableName=argv[0];
		size_t lastSlashPosition=executableName.find_last_of('/');
		if( lastSlashPosition!=std::string::npos ) executableName=executableName.substr( lastSlashPosition+1, std::string::npos );
		std::cerr << "   Usage: " << executableName << " <ntuple filename> <menu filename>" << std::endl;
		return -1;
	}

	std::string ntupleFilename=argv[1];
	std::string menuFilename =argv[2];

	try
	{
		l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();

		std::cout << "------ Available triggers ------" << std::endl;
		std::vector<l1menu::TriggerTable::TriggerDetails> listOfTriggers=triggerTable.listTriggers();
		for( std::vector<l1menu::TriggerTable::TriggerDetails>::const_iterator iTriggerEntry=listOfTriggers.begin(); iTriggerEntry!=listOfTriggers.end(); ++iTriggerEntry )
		{
			std::cout << "Name: \"" << iTriggerEntry->name << "\" version " << iTriggerEntry->version << std::endl;
		}
		std::cout << "------- End of triggers -------" << std::endl;

		std::unique_ptr<l1menu::ITrigger> pMyTrigger=triggerTable.getTrigger("L1_DoubleJet");
		if( pMyTrigger.get()==NULL ) throw std::runtime_error( "The trigger was not in the trigger table" );

		std::cout << "Loading menu from file " << menuFilename << std::endl;
		l1menu::TriggerMenu myMenu;
		myMenu.loadMenuFromFile( menuFilename );

		std::cout << "Loading ntuple from file " << ntupleFilename << std::endl;
		l1menu::MenuSample mySample;
		mySample.loadFile(ntupleFilename);

		l1menu::MenuRatePlots rateVersusThresholdPlots( myMenu );
//		l1menu::TriggerRatePlot doubleJetRate( *pMyTrigger );
//
		std::cout << "There are " << mySample.numberOfEvents() << " events." << std::endl;
		size_t eventNumber;
		for( eventNumber=0; eventNumber<mySample.numberOfEvents(); ++eventNumber )
		{
			//if( eventNumber>5 ) break;
			const l1menu::IEvent& event=mySample.getEvent( eventNumber );

			rateVersusThresholdPlots.addEvent( event );
		}
		std::cout << "Finished processing " << eventNumber << " events." << std::endl;

		// Save the histogram
		std::unique_ptr<TFile> pMyRootFile( new TFile( "rateHistograms.root", "RECREATE" ) );
		rateVersusThresholdPlots.setDirectory( pMyRootFile.get() );
		// If I don't do this the plots will be deleted twice - once by the MenuRatePlots and once by the TTree.
		rateVersusThresholdPlots.relinquishOwnershipOfPlots();
		pMyRootFile->Write();
		pMyRootFile->Close();

	}
	catch( std::exception& error )
	{
		std::cerr << "Exception caught: " << error.what() << std::endl;
	}
}
