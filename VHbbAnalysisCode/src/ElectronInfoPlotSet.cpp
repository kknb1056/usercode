#include "TrkUpgradeAnalysis/VHbb/interface/ElectronInfoPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::ElectronInfoPlotSet::ElectronInfoPlotSet()
	: histogramHaveBeenBooked_(false), leptonPlotSet_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::ElectronInfoPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::ElectronInfoPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//
	leptonPlotSet_.book(pDirectory);

	pID95_=new TH1F( "id95", "id95", 9, -0.5, 8.5 );
	pID95_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::ElectronInfoPlotSet::fill( const VHbbEvent::ElectronInfo& electron, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::ElectronInfoPlotSet::book() - histograms have not been booked" );

	leptonPlotSet_.fill( electron, pAuxInfo );

	pID95_->Fill( electron.id95 );

}

