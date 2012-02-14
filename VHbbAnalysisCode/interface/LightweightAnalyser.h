#ifndef trkupgradeanalysis_LightweightAnalyser_h
#define trkupgradeanalysis_LightweightAnalyser_h

#include <string>
#include <vector>
#include <map>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "VHbbAnalysis/VHbbDataFormats/interface/HbbCandidateFinderAlgo.h"

#include "TrkUpgradeAnalysis/VHbb/interface/VHbbCandidatePlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/MuonInfoCollectionPlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/ElectronInfoCollectionPlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/MCInfoPlotSet.h"
#include "TrkUpgradeAnalysis/VHbb/interface/CutSetPlotSet.h"

// Forward declarations
class TH1F;
class TH2F;
class TFile;
class TDirectory;
namespace fwlite
{
	class Event;
}
namespace trkupgradeanalysis
{
	class VHbbCandidateCutSet;
}


namespace trkupgradeanalysis
{

	class LightweightAnalyser
	{
	public:
		LightweightAnalyser( const std::string& outputFilename );
		~LightweightAnalyser();
		void processFile( TFile* pInputFile );
		void changeDirectory( const std::string& directory );
		void save();
	protected:
		void processEvent( const fwlite::Event& event );
		HbbCandidateFinderAlgo zFinder_;
		HbbCandidateFinderAlgo wFinder_;

		boost::scoped_ptr<TFile> pOutputFile_;

		size_t numberOfEvents_;
		size_t numberOfEventsWithAtLeastOneZOrWCandidate_;
		static const bool verbose_;
		static const double jetPtThresholdZ_;
		static const double jetPtThresholdW_;
		static const bool useHighestPtHiggsZ_;
		static const bool useHighestPtHiggsW_;

		struct AllPlots
		{
			TH2F* pNumberOfCandidates_;
			trkupgradeanalysis::VHbbCandidatePlotSet candidateHistogramsForAllEvents_;
			trkupgradeanalysis::MuonInfoCollectionPlotSet allMuons_;
			trkupgradeanalysis::ElectronInfoCollectionPlotSet allElectrons_;
			trkupgradeanalysis::MCInfoPlotSet monteCarloInfo_;
			std::vector<CutSetPlotSet> cutCollectionPlotSets_;
		};

//		std::map<std::string,AllPlots> allPlots_;
//		AllPlots* pCurrentPlots_; ///< @basic The entry in allPlots_ that is currently being acted on.

		struct PlotsSplitByMCType
		{
			AllPlots allEventTypes_;
			std::map<std::string,AllPlots> mcEventTypePlots_;
		};


		std::map<std::string,PlotsSplitByMCType> plotsForEventTypes_;
		PlotsSplitByMCType* pPlotsForEverything_; ///< @brief The entry in plotsForEventTypes_ currently being acted on.
//		AllPlots* pPlotsForAllEventTypes_;
//		AllPlots* pPlotsForCurrentEventType_;
		std::string currentDirectory_;

		std::vector< boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> > cutsToApply_;

		void bookHistograms( AllPlots& plots, const std::string& directoryName );
		std::string eventTypeFromMC( const VHbbEventAuxInfo& eventAuxInfo );
		void fillAllPlotsStructure( AllPlots* pPlotsToFill, const VHbbEvent& vhbbEvent, const std::vector<VHbbCandidate>& zCandidates, const std::vector<VHbbCandidate>& wCandidates );
	};

} // end of namespace trkupgradeanalysis


#endif // end of "#ifndef trkupgradeanalysis_LightweightAnalyser_h"
