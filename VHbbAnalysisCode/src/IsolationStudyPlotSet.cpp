#include "TrkUpgradeAnalysis/VHbb/interface/IsolationStudyPlotSet.h"

#include <stdexcept>

#include <TDirectory.h>
#include <TH1F.h>

#include "VHbbAnalysis/VHbbDataFormats/interface/VHbbEventAuxInfo.h"

namespace // Use the unnamed namespace for things only used in this file
{
	/** @brief A functor to sort muon collections by pT in the STL sort algorithm.
	 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
	 * @date 11/Apr/2012
	 */
	template <typename T>
	class SortByTransverseMomentum
	{
	public:
		// Return true if the first argument should come before
		// the second, as per normal STL sort comparitors
		bool operator()( const T& first, const T& second )
		{
			return first.p4.Pt() > second.p4.Pt();
		}
	};

	/** @brief Class to pass as exception when something in the analysis breaks down.
	 *
	 * Intended to be thrown when the event/particle/whatever cannot be processed further
	 * but the analysis as a whole should continue running. A std::runtime_error will be
	 * thrown when everything should stop.
	 */
	class AnalysisFailureException : public std::runtime_error
	{
	public:
		AnalysisFailureException( const std::string& what ) : std::runtime_error(what) {}
	};
}

trkupgradeanalysis::IsolationStudyPlotSet::IsolationStudyPlotSet()
	: histogramHaveBeenBooked_(false), leastIsolatedDiMuon_(true), leastDeltaBetaCorrectedIsolatedDiMuon_(true)
{
	// No operation besides the initialiser list.
}

void trkupgradeanalysis::IsolationStudyPlotSet::book( TDirectory* pDirectory )
{
	if( histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::IsolationStudyPlotSet::book() - histograms have already been booked" );

	//
	// Note that the root file which TDirectory is part of takes ownership of all
	// of these objects, so I don't need to (and shouldn't) delete them when I'm
	// finished.
	//


//	pGlobalChi2_=new TH1F( "globalChi2","Global chi2", 60, 0, 20 );
//	pGlobalChi2_->SetDirectory(pDirectory);

//	pLeastIsolatedDiMuonTree_=new TTree("leastIsolatedDiMuonTree","Data about the least isolated muon in the di-muon pair");
//	pLeastIsolatedDiMuonTree_->Branch("numberOfPrimaryVertices",&numberOfPrimaryVertices_branch_,"numberOfPrimaryVertices/I");
//	pLeastIsolatedDiMuonTree_->Branch("chargedIsolation",&chargedIsolation_branch_,"chargedIsolation/F");
//	pLeastIsolatedDiMuonTree_->Branch("photonIsolation",&photonIsolation_branch_,"photonIsolation/F");
//	pLeastIsolatedDiMuonTree_->Branch("neutralIsolation",&neutralIsolation_branch_,"neutralIsolation/F");
//	pLeastIsolatedDiMuonTree_->Branch("pileupIsolation",&pileupIsolation_branch_,"pileupIsolation/F");
//	pLeastIsolatedDiMuonTree_->Branch("pT",&pT_branch_,"pT/F");
//	pLeastIsolatedDiMuonTree_->SetDirectory( pDirectory );

	TDirectory* pNewDirectory;
	pNewDirectory=pDirectory->mkdir("cleanedMuons");
	cleanedMuons_.book( pNewDirectory );

	pNewDirectory=pDirectory->mkdir("highestPtDiMuon");
	highestPtDiMuon_.book( pNewDirectory );

	pNewDirectory=pDirectory->mkdir("lowestPtDiMuon");
	lowestPtDiMuon_.book( pNewDirectory );

	pNewDirectory=pDirectory->mkdir("mostIsolatedDiMuon");
	mostIsolatedDiMuon_.book( pNewDirectory );

	pNewDirectory=pDirectory->mkdir("leastIsolatedDiMuon");
	leastIsolatedDiMuon_.book( pNewDirectory );

	pNewDirectory=pDirectory->mkdir("leastDeltaBetaCorrectedIsolatedDiMuon");
	leastDeltaBetaCorrectedIsolatedDiMuon_.book( pNewDirectory );

	histogramHaveBeenBooked_=true;
}

void trkupgradeanalysis::IsolationStudyPlotSet::fill( const VHbbEvent& event, const VHbbEventAuxInfo* pAuxInfo )
{
	if( !histogramHaveBeenBooked_ ) throw std::runtime_error( "trkupgradeanalysis::IsolationStudyPlotSet::book() - histograms have not been booked" );

	try
	{
		// Clean the muons with all the criteria of the VHbb package (apart from isolation)
		std::vector<VHbbEvent::MuonInfo> cleanedMuons=cleanMuons( event.muInfo );
		// Then make sure they're sorted high to low by pT
		std::sort( cleanedMuons.begin(), cleanedMuons.end(), ::SortByTransverseMomentum<VHbbEvent::MuonInfo>() );
		// Then find two with opposite charge
		std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> diMuons=findOppositelyChargedPair(cleanedMuons);

		int numberOfPrimaryVertices;
		if( pAuxInfo ) numberOfPrimaryVertices=pAuxInfo->pvInfo.nVertices;
		else numberOfPrimaryVertices=-1;

		cleanedMuons_.fill( cleanedMuons, pAuxInfo ); // Fill some plots about the cleaned muons
		highestPtDiMuon_.fill( diMuons.first, pAuxInfo ); // I know the collection is already sorted by pT, so the first has the highest.
		lowestPtDiMuon_.fill( diMuons.second, pAuxInfo );

		if( firstMuonIsMoreIsolated(diMuons) )
		{
			mostIsolatedDiMuon_.fill( diMuons.first, pAuxInfo );
			leastIsolatedDiMuon_.fill( diMuons.second, pAuxInfo );
		}
		else
		{
			mostIsolatedDiMuon_.fill( diMuons.second, pAuxInfo );
			leastIsolatedDiMuon_.fill( diMuons.first, pAuxInfo );
		}

		if( firstMuonIsMoreDeltaBetaCorrectedIsolated(diMuons) ) leastDeltaBetaCorrectedIsolatedDiMuon_.fill( diMuons.second, pAuxInfo );
		else leastDeltaBetaCorrectedIsolatedDiMuon_.fill( diMuons.first, pAuxInfo );
	}
	catch( ::AnalysisFailureException& exception )
	{
		// Um, not sure what to do. Nothing and carry on I suppose. I'm expecting a fair few cases where two oppositely charged muons
		// can't be found so I won't print a warning.
	}

//	pGlobalChi2_->Fill( muon.globChi2 );
}

std::vector<VHbbEvent::MuonInfo> trkupgradeanalysis::IsolationStudyPlotSet::cleanMuons( const std::vector<VHbbEvent::MuonInfo>& muons )
{
	std::vector<VHbbEvent::MuonInfo> returnValue;

	for( std::vector<VHbbEvent::MuonInfo>::const_iterator iMuon=muons.begin(); iMuon!=muons.end(); ++iMuon )
	{
		if( iMuon->globChi2<10 &&
		    iMuon->nPixelHits>= 1 &&
		    iMuon->globNHits != 0 &&
		    iMuon->nHits > 10 &&
		    (iMuon->cat & 0x1) && (iMuon->cat & 0x2) &&
		    iMuon->nMatches >=2 &&
		    iMuon->ipDb<.2 &&
		    fabs(iMuon->p4.Eta())<2.4 &&
		    iMuon->p4.Pt()>20 ) returnValue.push_back(*iMuon);
	}

	return returnValue;
}

std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> trkupgradeanalysis::IsolationStudyPlotSet::findOppositelyChargedPair( const std::vector<VHbbEvent::MuonInfo>& muons )
{
	// No need to check if there are two or more muons in the collection, because if not the loop will not be
	// entered and the other exception will be triggered.

	size_t otherMuonIndex=0; // Use a default of zero as an invalid value
	for( size_t a=1; a<muons.size(); ++a ) // Run over all muons except the first
	{
		if( muons[0].charge*muons[a].charge < 0 ) // Check to see if this muon has opposite charge to the first
		{
			otherMuonIndex=a;
			break;
		}
	}

	if( otherMuonIndex==0 ) throw ::AnalysisFailureException("IsolationStudyPlotSet::findOppositelyChargedPair - Unable to find two oppositely charged muons");

	return std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo>( muons[0], muons[otherMuonIndex] );
}

bool trkupgradeanalysis::IsolationStudyPlotSet::firstMuonIsMoreIsolated( std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> diMuons )
{
	float firstIsolation=MuonInfoPlotSet::combinedRelativeIsolation(diMuons.first);
	float secondIsolation=MuonInfoPlotSet::combinedRelativeIsolation(diMuons.second);
	// More isolated if the sum of energy deposits around it is less.
	return firstIsolation<secondIsolation;
}

bool trkupgradeanalysis::IsolationStudyPlotSet::firstMuonIsMoreDeltaBetaCorrectedIsolated( std::pair<VHbbEvent::MuonInfo,VHbbEvent::MuonInfo> diMuons )
{
	float firstIsolation=MuonInfoPlotSet::deltaBetaCorrectedIsolation(diMuons.first);
	float secondIsolation=MuonInfoPlotSet::deltaBetaCorrectedIsolation(diMuons.second);
	// More isolated if the sum of energy deposits around it is less.
	return firstIsolation<secondIsolation;
}
