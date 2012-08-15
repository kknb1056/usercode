#include "TrkUpgradeAnalysis/VHbb/interface/CutSetPlotSet.h"

#include <stdexcept>
#include <sstream>

#include <TDirectory.h>
#include <TH1F.h>
#include <TTree.h>

#include "TrkUpgradeAnalysis/VHbb/interface/IBasicCut.h"
#include "TrkUpgradeAnalysis/VHbb/interface/IHistogramVariable.h"
#include "TrkUpgradeAnalysis/VHbb/interface/tools.h" // required for NTupleRow

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbCandidate.h"

#include <iostream>

trkupgradeanalysis::CutSetPlotSet::CutSetPlotSet( boost::shared_ptr<trkupgradeanalysis::VHbbCandidateCutSet> pCutSet, bool createNTupleOfAdditionalVariables )
	: histogramHaveBeenBooked_(false), pCutSet_(pCutSet), createNTupleOfAdditionalVariables_(createNTupleOfAdditionalVariables), pAdditionalVariableTree_(NULL)
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
	pCutsPassed_->GetXaxis()->SetBinLabel( 1, "Input VHbbCandidates" );

	pCutFlow_=new TH1F( "cutFlow","Events passing all cuts",numberOfCuts+1,-0.5,numberOfCuts+0.5);
	pCutFlow_->SetDirectory(pDirectory);
	pCutFlow_->GetXaxis()->SetBinLabel( 1, "Input VHbbCandidates" );

	// Create a directory for the variables for all candidates
	TDirectory* pAllCandidatesDirectory=pDirectory->GetDirectory( "AllCandidates" );
	if( pAllCandidatesDirectory==NULL ) pAllCandidatesDirectory=pDirectory->mkdir( "AllCandidates" );

	// Create a directory for the variables for all candidates that pass the cuts
	TDirectory* pPassedCandidatesDirectory=pDirectory->GetDirectory( "PassedCandidates" );
	if( pPassedCandidatesDirectory==NULL ) pPassedCandidatesDirectory=pDirectory->mkdir( "PassedCandidates" );

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

		// Book the histograms for all candidates
		pNewPlot=new TH1F( cutName.c_str(), cutName.c_str(), cutVariable.suggestedNumberOfBins(), cutVariable.suggestedLowerEdge(), cutVariable.suggestedUpperEdge() );
		pNewPlot->SetDirectory(pAllCandidatesDirectory);
		allCandidatesPlots_.push_back( pNewPlot );

		// Book the histograms for candidates that pass all cuts
		pNewPlot=new TH1F( cutName.c_str(), cutName.c_str(), cutVariable.suggestedNumberOfBins(), cutVariable.suggestedLowerEdge(), cutVariable.suggestedUpperEdge() );
		pNewPlot->SetDirectory(pPassedCandidatesDirectory);
		passedCandidatesPlots_.push_back( pNewPlot );
	}

	//
	// Now see if the cut set has defined any additional variables, i.e. variables that would
	// interesting to look at but shouldn't be cut on.
	//
	size_t numberOfAdditionalVariables=pCutSet_->numberOfAdditionalVariables();
	if( numberOfAdditionalVariables>0 )
	{
		if( createNTupleOfAdditionalVariables_ )
		{
			pAdditionalVariableTree_=new TTree("additionalVariables","Additional variables that haven't been cut on");
			pAdditionalVariableTree_->SetDirectory( pDirectory );
		}

		for( size_t a=0; a<numberOfAdditionalVariables; ++a )
		{
			const trkupgradeanalysis::IHistogramVariable& variable=pCutSet_->additionalVariableAt(a);
			TH1F* pNewPlot=new TH1F( variable.variableName().c_str(), variable.variableName().c_str(), variable.suggestedNumberOfBins(), variable.suggestedLowerEdge(), variable.suggestedUpperEdge() );
			pNewPlot->SetDirectory(pDirectory);
			additionalVariablePlots_.push_back(pNewPlot);

			// Set the tree branch to point to the correct point in the values vector. The values
			// vector will not change size so this *should* be safe.
			if( pAdditionalVariableTree_ )
			{
				// Note that the branch address is initially set to NULL because the address is set before each fill.
				// This is because if this class is copied the member addresses change, so I can't set the branch
				// address to any members.
				TBranch* pNewBranch=pAdditionalVariableTree_->Branch( variable.variableName().c_str(), NULL, (variable.variableName()+"/D").c_str() );
				additionalVariableBranches_.push_back( pNewBranch ); // record the branch so that I can set the address individually
			}
		}
	}

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::CutSetPlotSet::fill( const VHbbCandidate& vhbbCandidate )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::CutSetPlotSet::book() - histograms have not been booked" );

	bool candidatePassesAllCuts=false;
	if( pCutSet_->applyCuts(vhbbCandidate) )
	{
		passedCandidateHistograms_.fill( vhbbCandidate );
		candidatePassesAllCuts=true;

		//
		// Now fill the plots and tree for the additional variables if there are any.
		//

		// Need to keep track of the variables for the TTree to fill with (if there is one)
		std::vector<double> additionalVariableValues( pCutSet_->numberOfAdditionalVariables() );
		// Fill the histograms for the additional variables, and the ntuple if it has been requested
		for( size_t a=0; a<pCutSet_->numberOfAdditionalVariables(); ++a )
		{
			// All these vectors should be set the same size, so I won't check again
			Double_t value=pCutSet_->additionalVariableAt(a).histogrammableValue();
			additionalVariableValues[a]=value; // store this, so that the TTree can access it
			additionalVariablePlots_[a]->Fill(value);

			// An ntuple might not have been requested, in which case don't try setting empty vectors
			if( pAdditionalVariableTree_ ) additionalVariableBranches_[a]->SetAddress( &additionalVariableValues[a] );
		}
		if( pAdditionalVariableTree_ ) pAdditionalVariableTree_->Fill();
	}

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
		// Only fill the passed candidates plots if all cuts were passed
		if( candidatePassesAllCuts ) passedCandidatesPlots_[a]->Fill( pCutSet_->cutAt(a).cutVariable().histogrammableValue() );
	}

}

void trkupgradeanalysis::CutSetPlotSet::fill( const trkupgradeanalysis::tools::NTupleRow& ntupleRow )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::CutSetPlotSet::book() - histograms have not been booked" );

	bool candidatePassesAllCuts=false;
	if( pCutSet_->applyCuts(ntupleRow) )
	{
		// TODO - implement this
//		passedCandidateHistograms_.fill( ntupleRow );
		candidatePassesAllCuts=true;

		//
		// Now fill the plots and tree for the additional variables if there are any.
		//

		// Need to keep track of the variables for the TTree to fill with (if there is one)
		std::vector<double> additionalVariableValues( pCutSet_->numberOfAdditionalVariables() );
		// Fill the histograms for the additional variables, and the ntuple if it has been requested
		for( size_t a=0; a<pCutSet_->numberOfAdditionalVariables(); ++a )
		{
			// All these vectors should be set the same size, so I won't check again
			Double_t value=pCutSet_->additionalVariableAt(a).histogrammableValue();
			additionalVariableValues[a]=value; // store this, so that the TTree can access it
			additionalVariablePlots_[a]->Fill(value);

			// An ntuple might not have been requested, in which case don't try setting empty vectors
			if( pAdditionalVariableTree_ ) additionalVariableBranches_[a]->SetAddress( &additionalVariableValues[a] );
		}
		if( pAdditionalVariableTree_ ) pAdditionalVariableTree_->Fill();
	}

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
		// Only fill the passed candidates plots if all cuts were passed
		if( candidatePassesAllCuts ) passedCandidatesPlots_[a]->Fill( pCutSet_->cutAt(a).cutVariable().histogrammableValue() );
	}

}
