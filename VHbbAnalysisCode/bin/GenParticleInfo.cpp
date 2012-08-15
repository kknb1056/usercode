#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <TSystem.h>
#include <FWCore/FWLite/interface/AutoLibraryLoader.h>
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for TFile_auto_ptr

#include <DataFormats/HepMCCandidate/interface/GenParticle.h>

void processFile( const trkupgradeanalysis::tools::TFile_auto_ptr& pFile );
void processEvent( const fwlite::Event& event );

int main( int argc, char* argv[] )
{
	std::cout << "Starting" << std::endl;

	// Do the weird root stuff
	gSystem->Load( "libFWCoreFWLite" );
	gSystem->Load( "libDataFormatsFWLite" );
	AutoLibraryLoader::enable();

	std::vector<std::string> inputFilenames;
//	inputFilenames.push_back("/users/phmag/CMSSW/CMSSW_4_2_8_SLHCtk3/src/runOverCrab/testMultitrackValidator/GEN-SIM/REDIGI_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO.root");
	inputFilenames.push_back("/gpfs_phys/storm/cms/mc/CMSSW_4_2_3_SLHC2/RelValZMM/AODSIM/Upgrade_50PU-v2/0000/0E3EBF20-843A-E111-B957-003048679168.root");

	for( std::vector<std::string>::const_iterator iFilename=inputFilenames.begin(); iFilename!=inputFilenames.end(); ++iFilename )
	{
		std::cout << "Processing " << *iFilename << std::endl;
		try // Use another try block so that processing can continue on the other files if one fails
		{
			trkupgradeanalysis::tools::TFile_auto_ptr pInputFile( *iFilename );
			if( pInputFile==NULL ) throw std::runtime_error("Couldn't open the input file");

			processFile( pInputFile );
		}
		catch( std::exception& exception )
		{
			std::cerr << "Exception while processing file " << *iFilename << ": " << exception.what() << std::endl;
			return -1;
		}

	} // End of loop over input filenames

	return 0;
}

void processFile( const trkupgradeanalysis::tools::TFile_auto_ptr& pInputFile )
{
	fwlite::Event event( pInputFile.get() );

	for( event.toBegin(); !event.atEnd(); ++event )
	{
		processEvent( event );
	}

}

void processEvent( const fwlite::Event& event )
{
	fwlite::Handle< std::vector<reco::GenParticle> > genParticleHandle;
	genParticleHandle.getByLabel( event, "genParticles" );
	if( genParticleHandle.product() == 0 ) throw std::runtime_error( "Couldn't get the genParticle collection");

	const std::vector<reco::GenParticle>& genParticles=*genParticleHandle.product();

	std::cout << "There are " << genParticles.size() << " GenParticles." << std::endl;

	for( std::vector<reco::GenParticle>::const_iterator iParticle=genParticles.begin(); iParticle!=genParticles.end(); ++iParticle )
	{
		std::cout << "particle with PDG id=" << iParticle->pdgId() << ", nMothers=" << iParticle->numberOfMothers() << ", nDaughters=" << iParticle->numberOfDaughters() << " and collisionId=" << iParticle->collisionId() << std::endl;
	}

}
