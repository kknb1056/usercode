#include "MarksAnalysers/CBCAnalyser/interface/AnalyseCBCOutput.h"

#include <iostream>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <EventFilter/SiStripRawToDigi/interface/SiStripFEDBuffer.h>


namespace cbcanalyser
{
	DEFINE_FWK_MODULE(AnalyseCBCOutput);
}

cbcanalyser::AnalyseCBCOutput::AnalyseCBCOutput( const edm::ParameterSet& config )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::AnalyseCBCOutput()" << std::endl;
}

cbcanalyser::AnalyseCBCOutput::~AnalyseCBCOutput()
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::~AnalyseCBCOutput()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::fillDescriptions()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::beginJob()
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::beginJob()" << std::endl;
}

class CBCChannelUnpacker
{
public:
	CBCChannelUnpacker( const sistrip::FEDChannel& fedChannel );
	const std::vector<bool>& hits() const;
private:
	std::vector<bool> hits_;
};

CBCChannelUnpacker::CBCChannelUnpacker( const sistrip::FEDChannel& fedChannel )
	: hits_(128,false)
{
	sistrip::FEDZSChannelUnpacker unpacker=sistrip::FEDZSChannelUnpacker::zeroSuppressedModeUnpacker(fedChannel);

	while( unpacker.hasData() )
	{
		if( unpacker.adc()>0 ) // A "1" seems to be encoded with "243", and "0" either absent or with "0"
		{
			hits_[unpacker.sampleNumber()]=true;
		}
		unpacker++;
	}
}

const std::vector<bool>& CBCChannelUnpacker::hits() const
{
	return hits_;
}

void cbcanalyser::AnalyseCBCOutput::analyze( const edm::Event& event, const edm::EventSetup& setup )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::analyze()" << std::endl;

	edm::Handle<FEDRawDataCollection> hRawData;
	event.getByLabel( "rawDataCollector", hRawData );

	size_t fedIndex;
	for( fedIndex=0; fedIndex<sistrip::CMS_FED_ID_MAX; ++fedIndex )
	{
		const FEDRawData& fedData=hRawData->FEDData(fedIndex);

		if( fedData.size()!=0 )
		{
			// Check to see if this FED is one of the ones allocated to the strip tracker
			if( fedIndex < sistrip::FED_ID_MIN || fedIndex > sistrip::FED_ID_MAX )
			{
				//std::cout << "Skipping FEDRawData at fedIndex " << std::dec << fedIndex << " has size " << fedData.size() << std::endl;
				continue;
			}

			//std::cout << "FEDRawData at fedIndex " << std::dec << fedIndex << " has size " << fedData.size() << std::endl;
			try
			{
				sistrip::FEDBuffer myBuffer(fedData.data(),fedData.size());
				//myBuffer.print( std::cout );

				for ( uint16_t feIndex = 0; feIndex<sistrip::FEUNITS_PER_FED; ++feIndex )
				{
					if( !myBuffer.fePresent(feIndex) ) continue;

					for ( uint16_t channelInFe = 0; channelInFe < sistrip::FEDCH_PER_FEUNIT*sistrip::FEUNITS_PER_FED; ++channelInFe )
					{
						const uint16_t channelIndex=feIndex*channelInFe;
						const sistrip::FEDChannel& channel=myBuffer.channel(channelIndex);

						CBCChannelUnpacker unpacker(channel);
						const std::vector<bool>& hits=unpacker.hits();
						for( std::vector<bool>::const_iterator iHit=hits.begin(); iHit!=hits.end(); ++iHit )
						{
							if( *iHit==true ) std::cout << "1";
							else std::cout << " ";
						}
						std::cout << std::endl;
					}
				}
			}
			catch( std::exception& error )
			{
				std::cout << "Exception: "<< error.what() << std::endl;
			}

		}
	}
//	std::cout << fedIndex << " FEDs checked." << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::endJob()
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::endJob()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::beginRun( const edm::Run& run, const edm::EventSetup& setup )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::beginRun()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::endRun( const edm::Run& run, const edm::EventSetup& setup )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::endRun()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::beginLuminosityBlock( const edm::LuminosityBlock& lumiBlock, const edm::EventSetup& setup )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::beginLuminosityBlock()" << std::endl;
}

void cbcanalyser::AnalyseCBCOutput::endLuminosityBlock( const edm::LuminosityBlock& lumiBlock, const edm::EventSetup& setup )
{
//	std::cout << "cbcanalyser::AnalyseCBCOutput::endLuminosityBlock()" << std::endl;
}
