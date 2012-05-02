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
			<< "\t" << "Examines btag performance files and finds the b, c and light tagging efficiencies for a" << "\n"
			<< "\t" << "given CSV discriminator" << "\n"
			<< "\t" << "Finds the closest histogram bins to the point required and then uses linear interpolation" << "\n"
			<< "\t" << "to find the value between those two bins." << "\n"
			<< "Usage:" << "\n"
			<< "\t" << programName << " -d <discriminator> <btag performance filename> [<btag performance filename2> [...] ]" << "\n"
			<< "\n"
			<< "\t" << "-d  --discriminator           The value of the CSV discriminator to calculate" << "\n"
			<< "\t" << "                              efficiencies for" << "\n"
			<< "\t" << "<btag performance filename>   One or more root files with these histograms of events" << "\n"
			<< "\t" << "                              versus discriminator:" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALB" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALC" << "\n"
			<< "\t" << "    /DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALDUSG" << "\n"
			<< std::endl;
}


int main( int argc, char* argv[] )
{
	std::vector<std::string> inputFilenames; // The list of files to examine. Set from the command line.
	float discriminator; // The discriminator to find the efficiencies for. Set from the command line.

	//
	// Run over the command line arguments and figure out what the user wants to do
	//
	try
	{
		std::string discriminatorAsString;

		for( int a=1; a<argc; ++a )
		{
			if( std::string(argv[a])=="-h" || std::string(argv[a])=="--help" ) printUsage( argv[0] );
			else if( std::string(argv[a])=="-d" || std::string(argv[a])=="--discriminator" )
			{
				if( !discriminatorAsString.empty() ) throw std::runtime_error("discriminator can only be specified once with the -d option");
				if( a+1>=argc ) throw std::runtime_error("-d requires an additional argument");

				++a;
				discriminatorAsString=argv[a];
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

		// Make sure the discriminator has been set to a valid float
		if( !discriminatorAsString.empty() )
		{
			// Try and convert the discriminator to a float
			std::stringstream stringConverter;
			stringConverter.str(discriminatorAsString);
			stringConverter >> discriminator;

			// Make sure all of the string converted properly to the float
			if( stringConverter.fail() || !stringConverter.eof() ) throw std::runtime_error("Discriminator must be set to a valid floating point number.");

			// Make sure the float is a valid value
			// EDIT - I'll allow any value as input for now
			//if( !(efficiency>0.0 && efficiency<1.0) ) throw std::runtime_error("Efficiency must be set to a value between 0 and 1 exclusive.");
		}
		else throw std::runtime_error("The discriminator has not been specified. Specify a discriminator with the -d option.");

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

				// Try and retrieve the required histograms I need from the file
				TH1F* pBJetHistogram=dynamic_cast<TH1F*>( pInputFile->Get( "/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALB" ) );
				TH1F* pCJetHistogram=dynamic_cast<TH1F*>( pInputFile->Get( "/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALC" ) );
				TH1F* pLightJetHistogram=dynamic_cast<TH1F*>( pInputFile->Get( "/DQMData/Run 1/Btag/Run summary/CSVMVA_GLOBAL/discr_CSVMVA_GLOBALDUSG" ) );
				if( pBJetHistogram==NULL || pCJetHistogram==NULL || pLightJetHistogram==NULL ) throw std::runtime_error("Couldn't get one of the input histograms.");

				float bJetEfficiency=trkupgradeanalysis::tools::findEfficiency( pBJetHistogram, discriminator, false );
				float cJetEfficiency=trkupgradeanalysis::tools::findEfficiency( pCJetHistogram, discriminator, false );
				float lightJetEfficiency=trkupgradeanalysis::tools::findEfficiency( pLightJetHistogram, discriminator, false );

				std::cout << "Discriminator= " << discriminator
						<< " btagEfficiency= " << bJetEfficiency
						<< " ctagEfficiency= " << cJetEfficiency
						<< " lighttagEfficiency= " << lightJetEfficiency
						<< " input= " << *iFilename << std::endl;
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
