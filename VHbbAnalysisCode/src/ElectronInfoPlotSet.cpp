#include "TrkUpgradeAnalysis/VHbb/interface/ElectronInfoPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>


trkupgradeanalysis::ElectronInfoPlotSet::ElectronInfoPlotSet()
	: histogramHaveBeenBooked_(false)
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


	pEta_=new TH1F( "eta", "Eta", 60, -3, 3 );
	pEta_->SetDirectory(pDirectory);

	pPt_=new TH1F( "pT", "pT", 60, 0, 250 );
	pPt_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::ElectronInfoPlotSet::fill( const VHbbEvent::ElectronInfo& electron )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::ElectronInfoPlotSet::book() - histograms have not been booked" );

	pEta_->Fill( electron.p4.Eta() );
	pPt_->Fill( electron.p4.Pt() );
}

