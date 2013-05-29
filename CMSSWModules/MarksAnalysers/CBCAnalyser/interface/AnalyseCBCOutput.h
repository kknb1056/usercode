#ifndef MarksAnalysers_CBCAnalyser_AnalyseCBCOutput_h
#define MarksAnalysers_CBCAnalyser_AnalyseCBCOutput_h

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>

namespace cbcanalyser
{
	/** @brief Analyser to look over the CBC output written by the GlibStreamer XDAQ plugin.
	 *
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 08/May2013
	 */
	class AnalyseCBCOutput : public edm::EDAnalyzer
	{
	public:
		explicit AnalyseCBCOutput( const edm::ParameterSet& config );
		~AnalyseCBCOutput();

		static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );
	private:
		virtual void beginJob();
		virtual void analyze( const edm::Event& event, const edm::EventSetup& setup );
		virtual void endJob();

		virtual void beginRun( const edm::Run& run, const edm::EventSetup& setup );
		virtual void endRun( const edm::Run& run, const edm::EventSetup& setup );
		virtual void beginLuminosityBlock( const edm::LuminosityBlock& lumiBlock, const edm::EventSetup& setup );
		virtual void endLuminosityBlock( const edm::LuminosityBlock& lumiBlock, const edm::EventSetup& setup );
	};

} // end of namespace cbcanalyser

#endif
