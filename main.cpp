/****************************************************
 ** main.cpp 
 ** Main program to execute the GammaOrganizer class
 ** Written S.V. Paulauskas - 17 Aug 2011
 ***************************************************/
#include <iostream>
#include <string>

#include <cstdlib>

#include "GammaOrganizer.h"

int main(int argc, char* argv[])
{
   int searchEnergy;
   std::string verbosity;
   
   GammaOrganizer gammaInfo(argc);
   
   if(argc == 2) {
      if(atoi(argv[1]) == 0) {
	 gammaInfo.OutputHelpInfo();
	 exit(1);
      } else {
	 searchEnergy = atoi(argv[1]);
	 verbosity = "--g";
      }
   }else if(argc == 3) {
      if(atoi(argv[1]) == 0 && atoi(argv[2]) != 0) {
	 searchEnergy = atoi(argv[2]);
	 verbosity = argv[1];
      }else if(atoi(argv[1]) != 0 && atoi(argv[2]) != 0) {
	 gammaInfo.OutputGammaRange(atoi(argv[1]), atoi(argv[2]), "--l");
	 exit(1);
      }else {
	 gammaInfo.OutputHelpInfo();
	 exit(1);
      }
   }else if(argc == 4) {
      if(atoi(argv[1]) == 0 && atoi(argv[2]) != 0 && atoi(argv[3]) != 0) {
	 gammaInfo.OutputGammaRange(atoi(argv[2]), atoi(argv[3]), argv[1]);
	 exit(1);
      }else {
	 gammaInfo.OutputHelpInfo();
	 exit(1);
      }
   }else {
      gammaInfo.OutputHelpInfo();
      exit(1);
   }
      
   gammaInfo.OutputGammaInformation(searchEnergy, verbosity);
}
