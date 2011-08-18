/*************************************************
 ** main.cpp 
 ** Main program to execute the Gamma class
 ** Written S.V. Paulauskas - 17 Aug 2011
 ************************************************/
#include <iostream>
#include <string>

#include <cstdlib>

#include "GammaOrganizer.h"

int main(int argc, char* argv[])
{
   int searchEnergy;
   std::string verbosity;

   GammaOrganizer gammaInfo(argc);

   if(atoi(argv[1]) == 0 && argc < 3) {
      gammaInfo.OutputGammaInformation(0, "--h");
      exit(1);
   }else if(atoi(argv[1]) != 0) {
      searchEnergy = atoi(argv[1]);
      verbosity = "--g";
   }else {
      searchEnergy = atoi(argv[2]);
      verbosity = argv[1];
   }
   
   gammaInfo.OutputGammaInformation(searchEnergy, verbosity);
}
