#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <sstream>

#include <TFile.h>
#include <TSystem.h>
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "TrkUpgradeAnalysis/VHbb/interface/LightweightAnalyser.h"

void printUsage( const std::string& programName )
{
	std::cout << programName << " [-o outputFilename] [-d directoryInRootFile] [-n numberOfEvents] inputFile1 [inputFile2 ....] [[-d directory2InRootFile] inputFile3 [inputFile4 ....]]" << "\n"
			<< "       e.g. \"" << programName << " -o output.root -d signal/ZH PAT.edm.01.root -d background/ZZ PAT.edm.02.root PAT.edm.03.root\"" << "\n"
			<< "            will create a file called output.root, with the data from PAT.edm.01.root in the directory signal/ZH and" << "\n"
			<< "            the data from PAT.edm.02.root and PAT.edm.03.root in the directory background/ZZ." << "\n"
			<< "\n"
			<< "            \"" << programName << " PAT.edm.01.root PAT.edm.02.root PAT.edm.03.root\"" << "\n"
			<< "            will create a file called TrkUpgradeAnalyserOutput.root (default), with all data from PAT.edm.01.root," << "\n"
			<< "            PAT.edm.02.root and PAT.edm.03.root in the root of the file." << std::endl;
}

int main( int argc, char* argv[] )
{
	std::cout << "Starting" << std::endl;

	// Do the weird root stuff
	gSystem->Load( "libFWCoreFWLite" );
	gSystem->Load( "libDataFormatsFWLite" );
	AutoLibraryLoader::enable();

	// These variables keep track of the settings passed on the command line
	std::map<std::string,std::vector<std::string> > directoriesAndFilenames;
	std::string currentDirectory="";
	std::string outputFilename;

	// Optionally take the number of events to run over from the command line
	size_t numberOfEvents;

	// Run over the command line arguments and figure out what the user wants to do
	try
	{
		std::string numberOfEventsAsString;

		for( int a=1; a<argc; ++a )
		{
			if( std::string(argv[a])=="-h" ) printUsage( argv[0] );
			else if( std::string(argv[a])=="-d" )
			{
				if( a+1>=argc ) throw std::runtime_error("-d requires an additional argument");

				++a;
				currentDirectory=argv[a];
				continue;
			}
			else if( std::string(argv[a])=="-o" )
			{
				if( !outputFilename.empty() ) throw std::runtime_error("only one output filename can be specified with the -o option");
				if( a+1>=argc ) throw std::runtime_error("-o requires an additional argument");

				++a;
				outputFilename=argv[a];
				continue;
			}
			else if( std::string(argv[a])=="-n" )
			{
				if( !numberOfEventsAsString.empty() ) throw std::runtime_error("number of events can only be specified once with the -n option");
				if( a+1>=argc ) throw std::runtime_error("-n requires an additional argument");

				++a;
				numberOfEventsAsString=argv[a];
				continue;
			}

			// If the argument doesn't match anything tried above then treat it as an input filename
			directoriesAndFilenames[currentDirectory].push_back( argv[a] );
		}

		if( directoriesAndFilenames.empty() ) throw std::runtime_error("No input files have been specified.");

		if( !numberOfEventsAsString.empty() )
		{
			// Try and convert the efficiency to a float
			std::stringstream stringConverter;
			stringConverter.str(numberOfEventsAsString);
			stringConverter >> numberOfEvents;

			// Make sure all of the string converted properly to the float
			if( stringConverter.fail() || !stringConverter.eof() ) throw std::runtime_error("Number of events must be set to a valid positive integer.");
		}
		else numberOfEvents=0; // default to 0, which is interpreted as running over all the events

	}
	catch( std::exception& error )
	{
		std::cerr << "Error parsing the command line arguments: " << error.what() << std::endl;
		printUsage( argv[0] );
		return -1;
	}

	// Create the analyser class with the output filename supplied
	if( outputFilename.empty() ) outputFilename="TrkUpgradeAnalyserOutput.root";
	trkupgradeanalysis::LightweightAnalyser analyser( outputFilename );

	// Now that I have a map of all the input filenames and all the directories the user wants
	// the histograms stored in, I can loop over them.
	for( std::map<std::string,std::vector<std::string> >::const_iterator iDirectoryFilenamePair=directoriesAndFilenames.begin();
			iDirectoryFilenamePair!=directoriesAndFilenames.end(); ++iDirectoryFilenamePair )
	{
		// Declare some references to make the code more readable
		const std::string& directory=iDirectoryFilenamePair->first;
		const std::vector<std::string>& filenames=iDirectoryFilenamePair->second;

		// This is talking about the directory in the root file where the histograms will be saved
		analyser.changeDirectory(directory);

		for( std::vector<std::string>::const_iterator iFilename=filenames.begin(); iFilename!=filenames.end(); ++iFilename )
		{
			std::cout << "Processing " << *iFilename << std::endl;
			TFile* pInputFile=TFile::Open( iFilename->c_str() );
			if( pInputFile == 0 )
			{
				std::cerr << "Couldn't open the input file" << std::endl;
				continue;
			}

			analyser.processFile( pInputFile, numberOfEvents );

			// Save after each file so if the program is interrupted at least I'll get
			// some output.
			analyser.save();
			pInputFile->Close(); // Close the input file before moving on to the next
			delete pInputFile;
		}
	}

	std::cout << "Finished" << std::endl;
	return 0;
}
