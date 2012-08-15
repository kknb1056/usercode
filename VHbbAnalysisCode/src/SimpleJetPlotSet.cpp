#include "TrkUpgradeAnalysis/VHbb/interface/SimpleJetPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"

trkupgradeanalysis::SimpleJetPlotSet::SimpleJetPlotSet()
	: histogramHaveBeenBooked_(false)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::SimpleJetPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::SimpleJetPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	transverseMomentum_=new TH1F( "transverseMomentum","Transverse momentum", 60, 0, 100 );
	transverseMomentum_->SetDirectory(pDirectory);

	numberOfTracks_=new TH1F( "numberOfTracks","Number of tracks", 61, -0.5, 60.5 );
	numberOfTracks_->SetDirectory(pDirectory);

	numberOfConstituents_=new TH1F( "numberOfConstituents","Number of constituents", 81, -0.5, 80.5 );
	numberOfConstituents_->SetDirectory(pDirectory);

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::SimpleJetPlotSet::fill( const VHbbEvent::SimpleJet& jet, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::SimpleJetPlotSet::book() - histograms have not been booked" );

	transverseMomentum_->Fill( jet.p4.Pt() );
	numberOfTracks_->Fill( jet.ntracks );
	numberOfConstituents_->Fill( jet.nConstituents );
}
