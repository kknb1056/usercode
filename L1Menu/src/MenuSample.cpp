#include "l1menu/MenuSample.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

#include <TSystem.h>
#include "UserCode/L1TriggerUpgrade/macros/L1UpgradeNtuple.h"
#include "UserCode/L1TriggerUpgrade/interface/L1AnalysisDataFormat.h"

namespace l1menu
{
	class MenuSamplePrivateMembers
	{
	private:
		static const size_t PHIBINS;
		static const double PHIBIN[];
		static const size_t ETABINS;
		static const double ETABIN[];

		double degree( double radian );
		int phiINjetCoord( double phi );
		int etaINjetCoord( double eta );
		double calculateHTT( const L1Analysis::L1AnalysisDataFormat& event );
		double calculateHTM( const L1Analysis::L1AnalysisDataFormat& event );
	public:
		void fillDataStructure( int selectDataInput );
		L1UpgradeNtuple inputNtuple;
		L1Analysis::L1AnalysisDataFormat currentEvent;
	};
}

const size_t l1menu::MenuSamplePrivateMembers::PHIBINS=18;
const double l1menu::MenuSamplePrivateMembers::PHIBIN[]={10,30,50,70,90,110,130,150,170,190,210,230,250,270,290,310,330,350};
const size_t l1menu::MenuSamplePrivateMembers::ETABINS=23;
const double l1menu::MenuSamplePrivateMembers::ETABIN[]={-5.,-4.5,-4.,-3.5,-3.,-2.172,-1.74,-1.392,-1.044,-0.696,-0.348,0,0.348,0.696,1.044,1.392,1.74,2.172,3.,3.5,4.,4.5,5.};


double l1menu::MenuSamplePrivateMembers::degree( double radian )
{
	if( radian<0 ) return 360.+(radian/M_PI*180.);
	else return radian/M_PI*180.;
}

int l1menu::MenuSamplePrivateMembers::phiINjetCoord( double phi )
{
	size_t phiIdx=0;
	double phidegree=degree( phi );
	for( size_t idx=0; idx<PHIBINS; idx++ )
	{
		if( phidegree>=PHIBIN[idx] and phidegree<PHIBIN[idx+1] ) phiIdx=idx;
		else if( phidegree>=PHIBIN[PHIBINS-1] || phidegree<=PHIBIN[0] ) phiIdx=idx;
	}
	phiIdx=phiIdx+1;
	if( phiIdx==18 ) phiIdx=0;
	return int( phiIdx );
}

int l1menu::MenuSamplePrivateMembers::etaINjetCoord( double eta )
{
	size_t etaIdx=0.;
	for( size_t idx=0; idx<ETABINS; idx++ )
	{
		if( eta>=ETABIN[idx] and eta<ETABIN[idx+1] ) etaIdx=idx;
	}
	return int( etaIdx );
}

double l1menu::MenuSamplePrivateMembers::calculateHTT( const L1Analysis::L1AnalysisDataFormat& event )
{
	double httValue=0.;

	// Calculate our own HT and HTM from the jets that survive the double jet removal.
	for( int i=0; i<event.Njet; i++ )
	{
		if( event.Bxjet.at( i )==0 )
		{
			if( event.Etajet.at( i )>4 and event.Etajet.at( i )<17 )
			{
				httValue+=event.Etjet.at( i );
			} //in proper eta range
		} //correct beam crossing
	} //loop over cleaned jets

	return httValue;
}

double l1menu::MenuSamplePrivateMembers::calculateHTM( const L1Analysis::L1AnalysisDataFormat& event )
{
	double htmValue=0.;
	double htmValueX=0.;
	double htmValueY=0.;

	// Calculate our own HT and HTM from the jets that survive the double jet removal.
	for( int i=0; i<event.Njet; i++ )
	{
		if( event.Bxjet.at( i )==0 )
		{
			if( event.Etajet.at( i )>4 and event.Etajet.at( i )<17 )
			{

				//  Get the phi angle  towers are 0-17 (this is probably not real mapping but OK for just magnitude of HTM
				float phi=2*M_PI*(event.Phijet.at( i )/18.);
				htmValueX+=cos( phi )*event.Etjet.at( i );
				htmValueY+=sin( phi )*event.Etjet.at( i );

			} //in proper eta range
		} //correct beam crossing
	} //loop over cleaned jets

	htmValue=sqrt( htmValueX*htmValueX+htmValueY*htmValueY );

	return htmValue;
}

void l1menu::MenuSamplePrivateMembers::fillDataStructure( int selectDataInput )
{
	currentEvent.Reset();

	// Grab standard event information
	currentEvent.Run=inputNtuple.event_->run;
	currentEvent.LS=inputNtuple.event_->lumi;
	currentEvent.Event=inputNtuple.event_->event;

	/* =======================================================================================================
	 /    Select the input source information
	 / ---------------------------------------------------------------------------------------------------------
	 /    case  0: Use Original L1ExtraTree that came with the event
	 /    case  1: Use reEmulated L1ExtraTree that was produced (note this may not be present or may be identical to the Original tree)
	 /    case  2: Use Original L1ExtraTree that came with the event except for muons which get from GMT. (For old Ntuple with no quality flag in L1Extra)
	 /
	 /	  case 10: Use Original L1Tree (GMT/GT) that came with the event
	 /    case 11: Use reEmulated L1Tree (GMT/GT) that was produced (note this may not be present or may be identical to the Original tree)
	 /    case 12: Use Original L1Tree (GMT/GCT) that was produced
	 /    case 13: Use reEmulated L1Tree (GMT/GCT) that was produced (note this may not be present or may be identical to the Original tree)
	 /
	 /    case 21: Use the L1ExtraUpgradeTree (Assuming Stage 1 Quantities filled in tree)
	 /    case 22: Use the L1ExtraUpgradeTree (Assuming Stage 2 Quantities filled in tree)
	 / ======================================================================================================= */
	switch( selectDataInput )
	{
		case 22:  //Select from L1ExtraUpgradeTree (Stage 2)

			// NOTES:  Stage 1 has EG Relaxed and EG Isolated.  The isolated EG are a subset of the Relaxed.
			//         so sort through the relaxed list and flag those that also appear in the isolated list.
			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nEG; i++ )
			{

				currentEvent.Bxel.push_back( inputNtuple.l1upgrade_->egBx.at( i ) );
				currentEvent.Etel.push_back( inputNtuple.l1upgrade_->egEt.at( i ) );
				currentEvent.Phiel.push_back( phiINjetCoord( inputNtuple.l1upgrade_->egPhi.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with phiINjetCoord
				currentEvent.Etael.push_back( etaINjetCoord( inputNtuple.l1upgrade_->egEta.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with etaINjetCoord

				// Check whether this EG is located in the isolation list
				bool isolated=false;
				bool fnd=false;
				unsigned int isoEG=0;
				while( !fnd && isoEG < inputNtuple.l1upgrade_->nIsoEG )
				{
					if( inputNtuple.l1upgrade_->isoEGPhi.at( isoEG )==inputNtuple.l1upgrade_->egPhi.at( i )
							&& inputNtuple.l1upgrade_->isoEGEta.at( isoEG )==inputNtuple.l1upgrade_->egEta.at( i ) )
					{
						isolated=true;
						fnd=true;
					}
					isoEG++;
				}
				currentEvent.Isoel.push_back( isolated );
				currentEvent.Nele++;
			}

			// Note:  Taus are in the jet list.  Decide what to do with them. For now
			//  leave them the there as jets (not even flagged..)
			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nJets; i++ )
			{

				// For each jet look for a possible duplicate if so remove it.
				bool duplicate=false;
				for( unsigned int j=0; j<i; j++ )
				{
					if( inputNtuple.l1upgrade_->jetBx.at( i )==inputNtuple.l1upgrade_->jetBx.at( j )
							&& inputNtuple.l1upgrade_->jetEt.at( i )==inputNtuple.l1upgrade_->jetEt.at( j )
							&& inputNtuple.l1upgrade_->jetEta.at( i )==inputNtuple.l1upgrade_->jetEta.at( j )
							&& inputNtuple.l1upgrade_->jetPhi.at( i )==inputNtuple.l1upgrade_->jetPhi.at( j ) )
					{
						duplicate=true;
						//printf("Duplicate jet found and removed \n");
					}
				}

				if( !duplicate )
				{
					currentEvent.Bxjet.push_back( inputNtuple.l1upgrade_->jetBx.at( i ) );
					currentEvent.Etjet.push_back( inputNtuple.l1upgrade_->jetEt.at( i ) );
					currentEvent.Phijet.push_back( phiINjetCoord( inputNtuple.l1upgrade_->jetPhi.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with phiINjetCoord
					currentEvent.Etajet.push_back( etaINjetCoord( inputNtuple.l1upgrade_->jetEta.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with etaINjetCoord
					currentEvent.Taujet.push_back( false );
					currentEvent.isoTaujet.push_back( false );
					//currentEvent.Fwdjet.push_back(false); //COMMENT OUT IF JET ETA FIX

					//if(fabs(inputNtuple.l1upgrade_->jetEta.at(i))>=3.0) printf("Et %f  Eta  %f  iEta  %f Phi %f  iPhi  %f \n",currentEvent.Etjet.at(currentEvent.Njet),inputNtuple.l1upgrade_->jetEta.at(i),currentEvent.Etajet.at(currentEvent.Njet),inputNtuple.l1upgrade_->jetPhi.at(i),currentEvent.Phijet.at(currentEvent.Njet));
					//  Eta Jet Fix.  Some Jets with eta>3 has appeared in central jet list.  Move them by hand
					//  This is a problem in Stage 2 Jet code.
					(fabs( inputNtuple.l1upgrade_->jetEta.at( i ) )>=3.0) ? currentEvent.Fwdjet.push_back( true ) : currentEvent.Fwdjet.push_back( false );

					currentEvent.Njet++;
				}
			}

			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nFwdJets; i++ )
			{

				currentEvent.Bxjet.push_back( inputNtuple.l1upgrade_->fwdJetBx.at( i ) );
				currentEvent.Etjet.push_back( inputNtuple.l1upgrade_->fwdJetEt.at( i ) );
				currentEvent.Phijet.push_back( phiINjetCoord( inputNtuple.l1upgrade_->fwdJetPhi.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with phiINjetCoord
				currentEvent.Etajet.push_back( etaINjetCoord( inputNtuple.l1upgrade_->fwdJetEta.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with etaINjetCoord
				currentEvent.Taujet.push_back( false );
				currentEvent.isoTaujet.push_back( false );
				currentEvent.Fwdjet.push_back( true );

				currentEvent.Njet++;
			}

			// NOTES:  Stage 1 has Tau Relaxed and TauIsolated.  The isolated Tau are a subset of the Relaxed.
			//         so sort through the relaxed list and flag those that also appear in the isolated list.

			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nTau; i++ )
			{

				// remove duplicates
				bool duplicate=false;
				for( unsigned int j=0; j<i; j++ )
				{
					if( inputNtuple.l1upgrade_->tauBx.at( i )==inputNtuple.l1upgrade_->tauBx.at( j )
							&& inputNtuple.l1upgrade_->tauEt.at( i )==inputNtuple.l1upgrade_->tauEt.at( j )
							&& inputNtuple.l1upgrade_->tauEta.at( i )==inputNtuple.l1upgrade_->tauEta.at( j )
							&& inputNtuple.l1upgrade_->tauPhi.at( i )==inputNtuple.l1upgrade_->tauPhi.at( j ) )
					{
						duplicate=true;
						//printf("Duplicate jet found and removed \n");
					}
				}

				if( !duplicate )
				{
					currentEvent.Bxjet.push_back( inputNtuple.l1upgrade_->tauBx.at( i ) );
					currentEvent.Etjet.push_back( inputNtuple.l1upgrade_->tauEt.at( i ) );
					currentEvent.Phijet.push_back( phiINjetCoord( inputNtuple.l1upgrade_->tauPhi.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with phiINjetCoord
					currentEvent.Etajet.push_back( etaINjetCoord( inputNtuple.l1upgrade_->tauEta.at( i ) ) ); //PROBLEM: real value, trigger wants bin convert with etaINjetCoord
					currentEvent.Taujet.push_back( true );
					currentEvent.Fwdjet.push_back( false );

					bool isolated=false;
					bool fnd=false;
					unsigned int isoTau=0;
					while( !fnd && isoTau < inputNtuple.l1upgrade_->nIsoTau )
					{
						if( inputNtuple.l1upgrade_->isoTauPhi.at( isoTau )==inputNtuple.l1upgrade_->tauPhi.at( i )
								&& inputNtuple.l1upgrade_->isoTauEta.at( isoTau )==inputNtuple.l1upgrade_->tauEta.at( i ) )
						{
							isolated=true;
							fnd=true;
						}
						isoTau++;
					}
					currentEvent.isoTaujet.push_back( isolated );

					currentEvent.Njet++;
				} // duplicate check
			}

			// Fill energy sums  (Are overflow flags accessible in l1extra?)
			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nMet; i++ )
			{
				//if(inputNtuple.l1upgrade_->metBx.at(i)==0) {
				currentEvent.ETT=inputNtuple.l1upgrade_->et.at( i );
				currentEvent.ETM=inputNtuple.l1upgrade_->met.at( i );
				currentEvent.PhiETM=inputNtuple.l1upgrade_->metPhi.at( i );
			}
			currentEvent.OvETT=0; //not available in l1extra
			currentEvent.OvETM=0; //not available in l1extra

			for( unsigned int i=0; i<inputNtuple.l1upgrade_->nMht; i++ )
			{
				if( inputNtuple.l1upgrade_->mhtBx.at( i )==0 )
				{
					currentEvent.HTT=calculateHTT( currentEvent ); //inputNtuple.l1upgrade_->ht.at(i) ;
					currentEvent.HTM=calculateHTM( currentEvent ); //inputNtuple.l1upgrade_->mht.at(i) ;
					currentEvent.PhiHTM=0.; //inputNtuple.l1upgrade_->mhtPhi.at(i) ;
				}
			}
			currentEvent.OvHTM=0; //not available in l1extra
			currentEvent.OvHTT=0; //not available in l1extra

			// Get the muon information  from reEmul GMT
			for( int i=0; i<inputNtuple.gmtEmu_->N; i++ )
			{

				currentEvent.Bxmu.push_back( inputNtuple.gmtEmu_->CandBx[i] );
				currentEvent.Ptmu.push_back( inputNtuple.gmtEmu_->Pt[i] );
				currentEvent.Phimu.push_back( inputNtuple.gmtEmu_->Phi[i] );
				currentEvent.Etamu.push_back( inputNtuple.gmtEmu_->Eta[i] );
				currentEvent.Qualmu.push_back( inputNtuple.gmtEmu_->Qual[i] );
				currentEvent.Isomu.push_back( false );
			}

		break;

		default:
			std::cout << "---Not a valid input source FULL STOP! " << std::endl;

		break;
	}

	return;
}

l1menu::MenuSample::MenuSample()
	: pImple_( new MenuSamplePrivateMembers )
{
	// No operation besides the initialiser list
}

l1menu::MenuSample::~MenuSample()
{
	delete pImple_;
}

l1menu::MenuSample::MenuSample( const l1menu::MenuSample& otherMenuSample )
	: pImple_( new MenuSamplePrivateMembers(*otherMenuSample.pImple_) )
{
	// No operation besides the initialiser list
	gSystem->Load("libFWCoreFWLite.so");
}

l1menu::MenuSample& l1menu::MenuSample::operator=( const l1menu::MenuSample& otherMenuSample )
{
	*pImple_=*otherMenuSample.pImple_;
	return *this;
}

void l1menu::MenuSample::loadFile( const std::string& filename )
{
	pImple_->inputNtuple.Open( filename );
}

size_t l1menu::MenuSample::numberOfEvents() const
{
	return static_cast<size_t>( pImple_->inputNtuple.GetEntries() );
}

const L1Analysis::L1AnalysisDataFormat& l1menu::MenuSample::getEvent( size_t eventNumber ) const
{
	// Make sure the event number requested is valid. Use static_cast to get rid
	// of the "comparison between signed and unsigned" compiler warning.
	if( eventNumber>static_cast<size_t>(pImple_->inputNtuple.GetEntries()) ) throw std::runtime_error( "Requested event number is out of range" );

	pImple_->inputNtuple.LoadTree(eventNumber);
	pImple_->inputNtuple.GetEntry(eventNumber);
	// This next call fills pImple_->currentEvent with the information in pImple_->inputNtuple
	pImple_->fillDataStructure( 22 );

	return pImple_->currentEvent;
}