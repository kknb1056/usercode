#include "TSystem.h"
#include "TROOT.h"
#include "l1menu/MenuSample.h"

#include <iostream>
#include <string>

int main( int argc, char* argv[] )
{
	if( argc!=2 )
	{
		std::string executableName=argv[0];
		size_t lastSlashPosition=executableName.find_last_of('/');
		if( lastSlashPosition!=std::string::npos ) executableName=executableName.substr( lastSlashPosition+1, std::string::npos );
		std::cerr << "   Usage: " << executableName << " <ntuple filename>" << std::endl;
		return -1;
	}

	gSystem->Load("libFWCoreFWLite.so");
	std::string filename=argv[1];

	l1menu::MenuSample mySample;
	mySample.loadFile(filename);
}
