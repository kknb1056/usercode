#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <sstream>

#include <boost/algorithm/string.hpp>

#include <TFile.h>
#include <TH1.h>

#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for findDiscriminator() and TFile_auto_ptr


void printUsage( const std::string& programName )
{
	std::cout << "\n"
			<< "Program description:" << "\n"
			<< "\t" << "Examines btag performance files and finds the CSV discriminator required to get a given" << "\n"
			<< "\t" << "operating point. The operating point requested can be set with the -e and -f options. If" << "\n"
			<< "\t" << "these aren't specified defaults to the CSVT operating point of 0.1% light mistag rate" << "\n"
			<< "\t" << "(equivalent to specifying \"-e 0.001 -f light\")." << "\n"
			<< "\t" << "Finds the closest histogram bins to the point required and then uses linear interpolation" << "\n"
			<< "\t" << "to find the value between those two bins." << "\n"
			<< "Usage:" << "\n"
			<< "\t" << programName << " [-e <efficiency>] [-f <flavour=\"b\"|\"c\"|\"light\">] <btag performance filename> [<btag performance filename2> [...] ]" << "\n"
			<< "\n"
			<< "\t" << "-e  --efficiency              The efficiency that you would like achieved" << "\n"
			<< "\t" << "-f  --flavour                 The flavour that the efficiency is for. Must be either" << "\n"
			<< "\t" << "                             \"b\", \"c\" or \"light\"." << "\n"
			<< "\t" << "<btag performance filename>   One or more root files with these histograms of events" << "\n"
			<< "\t" << "                              versus discriminator:" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALB      (for \"-f b\")" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALC      (for \"-f c\")" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALDUSG   (for \"-f light\")" << "\n"
			<< std::endl;
}


int main( int argc, char* argv[] )
{
	std::vector<std::string> inputFilenames;
	float efficiency; // The efficency to find an operating point for. Defaults later to 0.001 for the Tight operating point of 0.1% light mistag efficiency.
	std::string flavour; // The flavour that the efficiency is for. Defaults later to "light" for the Tight operating point of 0.1% light mistag efficiency.

	//
	// Run over the command line arguments and figure out what the user wants to do
	//
	try
	{
		std::string efficiencyAsString;

		for( int a=1; a<argc; ++a )
		{
			if( std::string(argv[a])=="-h" || std::string(argv[a])=="--help" ) printUsage( argv[0] );
			else if( std::string(argv[a])=="-e" || std::string(argv[a])=="--efficiency" )
			{
				if( !efficiencyAsString.empty() ) throw std::runtime_error("efficiency can only be specified once with the -e option");
				if( a+1>=argc ) throw std::runtime_error("-e requires an additional argument");

				++a;
				efficiencyAsString=argv[a];
			}
			else if( std::string(argv[a])=="-f" || std::string(argv[a])=="--flavour" )
			{
				if( !flavour.empty() ) throw std::runtime_error("flavour can only be specified once with the -f option");
				if( a+1>=argc ) throw std::runtime_error("-f requires an additional argument");

				++a;
				flavour=argv[a];
			}
			else
			{
				// Assume anything else is an input filename
				inputFilenames.push_back( argv[a] );
			}
		}

		//
		// Do some error checking on the input
		//
		if( inputFilenames.empty() ) throw std::runtime_error("No input files have been specified.");

		// Make sure efficency and flavour have been set to valid values
		if( !flavour.empty() )
		{
			boost::algorithm::to_lower(flavour); // Convert everything to lower case before the comparison
			if( flavour!="b" && flavour!="c" && flavour!="light" ) throw std::runtime_error("Flavour must be set to one of \"b\", \"c\", or \"light\".");
		}
		else flavour="light"; // default to Tight operating point of 0.1% light mistag efficiency

		if( !efficiencyAsString.empty() )
		{
			// Try and convert the efficiency to a float
			std::stringstream stringConverter;
			stringConverter.str(efficiencyAsString);
			stringConverter >> efficiency;

			// Make sure all of the string converted properly to the float
			if( stringConverter.fail() || !stringConverter.eof() ) throw std::runtime_error("Efficiency must be set to a valid floating point number.");
			// Make sure the float is a valid value
			if( !(efficiency>0.0 && efficiency<1.0) ) throw std::runtime_error("Efficiency must be set to a value between 0 and 1 exclusive.");
		}
		else efficiency=0.001; // default to Tight operating point of 0.1% light mistag efficiency

	} // End of try block around command line parsing
	catch( std::exception& error )
	{
		std::cerr << "Error parsing the command line arguments: " << error.what() << std::endl;
		printUsage( argv[0] );
		return -1;
	}

	//
	// Now loop over all of the given input files
	//
	try
	{
		for( std::vector<std::string>::const_iterator iFilename=inputFilenames.begin(); iFilename!=inputFilenames.end(); ++iFilename )
		{
			try // Use another try block so that processing can continue on the other files if one fails
			{
				trkupgradeanalysis::tools::TFile_auto_ptr pInputFile( *iFilename );
				if( pInputFile==NULL ) throw std::runtime_error("Couldn't open the input file");

				// Try and retrieve the required histogram I need from the file
				std::string histogramPath;
				if( flavour=="b" ) histogramPath="/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALB";
				else if( flavour=="c" ) histogramPath="/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALC";
				else if( flavour=="light" ) histogramPath="/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALDUSG";

				TH1F* pPerformanceHistogram=dynamic_cast<TH1F*>( pInputFile->Get( histogramPath.c_str() ) );
				if( pPerformanceHistogram==NULL ) throw std::runtime_error("Couldn't get the input histogram \""+histogramPath+"\"");

				float discriminator=trkupgradeanalysis::tools::findDiscriminator( pPerformanceHistogram, efficiency, false );
//				std::cout << "An efficiency of " << efficiency
//						<< " can be acheived with a discriminator of " << discriminator
//						<< " for histogram " << *iFilename << ":" << histogramPath << std::endl;
				std::cout << flavour << "Efficiency= " << efficiency << " discriminator= " << discriminator << " input= " << *iFilename << std::endl;
			}
			catch( std::exception& exception )
			{
				std::cerr << "Exception while processing file " << *iFilename << ": " << exception.what() << std::endl;
				return -1;
			}

		} // End of loop over input filenames

	} // End of try block
	catch( std::exception& exception )
	{
		std::cerr << "General exception during processing: " << exception.what() << std::endl;
		return -1;
	}

	return 0;
}
