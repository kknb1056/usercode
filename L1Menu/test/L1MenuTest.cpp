#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "l1menu/MenuSample.h"
#include "l1menu/TriggerMenu.h"
#include "l1menu/TriggerTable.h"
#include "l1menu/ITrigger.h"
#include "l1menu/MenuRatePlots.h"
#include "l1menu/ReducedMenuSample.h"
#include "l1menu/ReducedEvent.h"
#include <iostream>
#include <iomanip>
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

//	try
//	{
		l1menu::TriggerTable& triggerTable=l1menu::TriggerTable::instance();

		std::cout << "------ Available triggers ------" << std::endl;
		std::cout << std::left << std::setw(25) << "Name" << "Version" << "\n";
		std::cout << "--------------------------------" << std::endl;
		std::vector<l1menu::TriggerTable::TriggerDetails> listOfTriggers=triggerTable.listTriggers();
		for( std::vector<l1menu::TriggerTable::TriggerDetails>::const_iterator iTriggerEntry=listOfTriggers.begin(); iTriggerEntry!=listOfTriggers.end(); ++iTriggerEntry )
		{
			std::cout << std::left << std::setw(25) << iTriggerEntry->name << iTriggerEntry->version << "\n";
		}
		std::cout << "------- End of triggers -------" << std::endl;

		// Open a file to save the histograms that will automatically
		// save if it goes out of scope (i.e. if an exception is thrown).
		std::unique_ptr<TFile,void(*)(TFile*)> pMyRootFile( new TFile( "rateHistograms.root", "RECREATE" ), [](TFile*p){p->Write();p->Close();delete p;} );

		std::unique_ptr<l1menu::ITrigger> pMyTrigger=triggerTable.getTrigger("L1_DoubleJet");
		if( pMyTrigger.get()==NULL ) throw std::runtime_error( "The trigger was not in the trigger table" );

		std::cout << "Loading menu from file " << menuFilename << std::endl;
		l1menu::TriggerMenu myMenu;
		myMenu.loadMenuFromFile( menuFilename );

		std::cout << "Loading ntuple from file " << ntupleFilename << std::endl;
		l1menu::MenuSample mySample;
		mySample.loadFile(ntupleFilename);

		l1menu::ReducedMenuSample myReducedSample( mySample, myMenu );
		myReducedSample.saveToFile( "reducedSample.proto" );

		size_t eventNumber;

		pMyTrigger->initiateForReducedSample( myReducedSample );

		l1menu::MenuRatePlots reducedRateVersusThresholdPlots( myMenu );
		TDirectory* pSubDirectory=pMyRootFile->mkdir("reduced");
		reducedRateVersusThresholdPlots.setDirectory(pSubDirectory);
		reducedRateVersusThresholdPlots.relinquishOwnershipOfPlots();

		reducedRateVersusThresholdPlots.initiateForReducedSample(myReducedSample);
//		for( eventNumber=0; eventNumber<myReducedSample.numberOfEvents(); ++eventNumber )
//		{
//			//if( eventNumber>5 ) break;
//			const l1menu::ReducedEvent& event=myReducedSample.getEvent( eventNumber );
//			reducedRateVersusThresholdPlots.addEvent( event );
//			//std::cout << eventNumber << " trigger=" << pMyTrigger->apply(event) << " Parameters: " << event.parameterValue(0) << "," << event.parameterValue(0)  << std::endl;
//		}
		std::cout << "Finished reduced test" << std::endl;

		l1menu::MenuRatePlots rateVersusThresholdPlots( myMenu );
		rateVersusThresholdPlots.setDirectory( pMyRootFile.get() );
		rateVersusThresholdPlots.relinquishOwnershipOfPlots(); // If I don't do this the plots will be deleted twice.

		std::cout << "There are " << mySample.numberOfEvents() << " events." << std::endl;
		for( eventNumber=0; eventNumber<mySample.numberOfEvents(); ++eventNumber )
		{
			//if( eventNumber>5 ) break;
			const l1menu::L1TriggerDPGEvent& event=mySample.getEvent( eventNumber );

			rateVersusThresholdPlots.addEvent( event );
		}
		std::cout << "Finished processing " << eventNumber << " events." << std::endl;



		const auto& fullPlots=rateVersusThresholdPlots.getPlots();
		const auto& reducedPlots=reducedRateVersusThresholdPlots.getPlots();

		pSubDirectory=pMyRootFile->mkdir("differences");

		for( auto iFullPlot=fullPlots.begin(), iReducedPlot=reducedPlots.begin();
				iFullPlot!=fullPlots.end() && iReducedPlot!=reducedPlots.end();
				++iFullPlot, ++iReducedPlot )
		{
			TAxis* pAxis=(*iFullPlot)->GetXaxis();
			TH1* pNewPlot=new TH1F( (std::string( (*iFullPlot)->GetName() )+"_difference").c_str(), (*iFullPlot)->GetTitle(), pAxis->GetNbins(), pAxis->GetXmin(), pAxis->GetXmax() );
			pNewPlot->SetDirectory( pSubDirectory );

			for( int binNumber=1; binNumber<=pAxis->GetNbins(); ++binNumber )
			{
				pNewPlot->SetBinContent( binNumber, (*iFullPlot)->GetBinContent(binNumber)-(*iReducedPlot)->GetBinContent(binNumber) );
			}
		}


//	}
//	catch( std::exception& error )
//	{
//		std::cerr << "Exception caught: " << error.what() << std::endl;
//	}
}
