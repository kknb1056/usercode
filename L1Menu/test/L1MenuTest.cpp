#include "TSystem.h"
#include "TROOT.h"
#include "menugeneration/MenuSample.h"

int main( int argc, char* argv[] )
{
	gSystem->Load("libFWCoreFWLite.so");
//	AutoLibraryLoader::enable();
	gROOT->ProcessLine(".L $CMSSW_BASE/src/UserCode/L1TriggerUpgrade/macros/L1UpgradeNtuple.C+");
	gROOT->ProcessLine(".L $CMSSW_BASE/src/UserCode/L1TriggerDPG/macros/L1Ntuple.C+");
	gROOT->ProcessLine(".L $CMSSW_BASE/src/UserCode/L1TriggerDPG/macros/L1Rates/toolbox/L1GtNtuple.C+");
	gROOT->ProcessLine(".L $CMSSW_BASE/src/UserCode/L1TriggerUpgrade/macros/L1Menu2015.C+");

	menugeneration::MenuSample mySample;
	mySample.loadFile("/gpfs_phys/storm/cms/user/jbrooke/L1UpgradeNtuples_v16/Fallback_NeutrinoGun_14TeV_PU100/L1Tree_1_1_3px.root");
}
