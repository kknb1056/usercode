#include "TrkUpgradeAnalysis/VHbb/interface/CutSetPlotSet.h"

#include <stdexcept>
#include <sstream>

#include <TDirectory.h>
#include <TH1F.h>

#include "TrkUpgradeAnalysis/VHbb/interface/IBasicCut.h"
#include "TrkUpgradeAnalysis/VHbb/interface/IHistogramVariable.h"

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

#include <iostream>

trkupgradeanalysis::CutSetPlotSet::CutSetPlotSet( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet )
	: histogramHaveBeenBooked_(false), pCutSet_(pCutSet)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::CutSetPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::CutSetPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//

	passedCandidateHistograms_.book( pDirectory );

	size_t numberOfCuts=pCutSet_->numberOfCuts();

	pCutsPassed_=new TH1F( "cutsPassed","Events passing each cut",numberOfCuts+1,-0.5,numberOfCuts+0.5);
	pCutsPassed_->SetDirectory(pDirectory);
	pCutsPassed_->GetXaxis()->SetBinLabel( 1, "Input events" );

	pCutFlow_=new TH1F( "cutFlow","Events passing all cuts",numberOfCuts+1,-0.5,numberOfCuts+0.5);
	pCutFlow_->SetDirectory(pDirectory);
	pCutFlow_->GetXaxis()->SetBinLabel( 1, "Input events" );

	// Create a directory for the N-1 plots
	TDirectory* pAllCandidatesDirectory=pDirectory->GetDirectory( "AllVariables" );
	if( pAllCandidatesDirectory==NULL ) pAllCandidatesDirectory=pDirectory->mkdir( "AllVariables" );

	// Create a directory for the N-1 plots
	TDirectory* pNMinus1Directory=pDirectory->GetDirectory( "N-1_Plots" );
	if( pNMinus1Directory==NULL ) pNMinus1Directory=pDirectory->mkdir( "N-1_Plots" );

	std::stringstream nameStream;
	for( size_t a=0; a<numberOfCuts; ++a )
	{
		const trkupgradeanalysis::IBasicCut& cut=pCutSet_->cutAt(a);
		const trkupgradeanalysis::IHistogramVariable& cutVariable=cut.cutVariable();

		std::string cutName=cut.name();
		// The bin numbering is "a+2" here because root bin numbering starts at 1, and
		// I'm using the first bin to record the number of input events.
		pCutsPassed_->GetXaxis()->SetBinLabel( a+2, cutName.c_str() );
		pCutFlow_->GetXaxis()->SetBinLabel( a+2, cutName.c_str() );

		nameStream.str("");
		nameStream << "N-1_" << cutName;
		TH1F* pNewPlot=new TH1F( nameStream.str().c_str(), nameStream.str().c_str(), cutVariable.suggestedNumberOfBins(), cutVariable.suggestedLowerEdge(), cutVariable.suggestedUpperEdge() );
		pNewPlot->SetDirectory(pNMinus1Directory);
		nMinus1Plots_.push_back( pNewPlot );

		pNewPlot=new TH1F( cutName.c_str(), cutName.c_str(), cutVariable.suggestedNumberOfBins(), cutVariable.suggestedLowerEdge(), cutVariable.suggestedUpperEdge() );
		pNewPlot->SetDirectory(pAllCandidatesDirectory);
		allCandidatesPlots_.push_back( pNewPlot );
	}


	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::CutSetPlotSet::fill( const VHbbCandidate& vhbbCandidate )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::CutSetPlotSet::book() - histograms have not been booked" );

	if( pCutSet_->applyCuts(vhbbCandidate) ) passedCandidateHistograms_.fill( vhbbCandidate );

	// Always fill the first bin so that I know how many candidates have been tested
	pCutFlow_->Fill(0);
	pCutsPassed_->Fill(0);

	// Now see which individual cuts have been passed and fill the histogram
	bool keepFillingCutFlowHistogram=true;
	for( size_t a=0; a<pCutSet_->numberOfCuts(); ++a )
	{
		if( pCutSet_->cutNPassed(a) ) pCutsPassed_->Fill(a+1); // "a+1" because the first bin is how many candidates there are at the start
		else keepFillingCutFlowHistogram=false;

		if( keepFillingCutFlowHistogram ) pCutFlow_->Fill(a+1);

		// Only fill the N-1 plots if all other cuts were passed
		if( pCutSet_->everythingOtherThanCutNPassed(a) ) nMinus1Plots_[a]->Fill( pCutSet_->cutAt(a).cutVariable().histogrammableValue() );
		// Always fill these plots for all of the candidates
		allCandidatesPlots_[a]->Fill( pCutSet_->cutAt(a).cutVariable().histogrammableValue() );
	}

}

