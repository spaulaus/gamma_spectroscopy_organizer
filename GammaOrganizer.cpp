/*************************************************
 ** GammaOranizer.cpp
 ** Class to hold and report info about gammas
 ** Written: S.V. Paulauskas - 17 Aug 2011
 ************************************************/
#include <algorithm>
#include <fstream>
#include <iostream>

#include <cmath>
#include <cstdlib>

#include "GammaOrganizer.h"

using namespace std;

GammaOrganizer::GenInfoMap GammaOrganizer::genInfo;
GammaOrganizer::CoinInfoMap GammaOrganizer::coinInfo;
GammaOrganizer::FitInfoMap GammaOrganizer::fitInfo;

//********** GammaOrganizer() **********
GammaOrganizer::GammaOrganizer(const int &numArgs)
{
   if(numArgs == 1) {
      OutputHelpInfo();
      exit(1);
   }
}


//********** ~GammaOrganizer() **********
GammaOrganizer::~GammaOrganizer()
{
}


//********** OutputCoinInfo **********
void GammaOrganizer::OutputCoinInfo(const int &searchEnergy)
{
   CoinInfoMap::iterator itCoin = coinInfo.find(searchEnergy);
   
   if(itCoin == coinInfo.end()) {
      cout << endl << "Coincidence Information" << endl 
	   << "--------------------" << endl
	   << "There is no coincidence information for this gamma." 
	   << endl << endl;
   }else {
      cout << endl << "Coincidence Information" << endl 
	   << "--------------------" << endl
	   << "Spectrum used: " << (*itCoin).second.spectrum << endl 
	   << "Gamma: (" << (*itCoin).second.gammaLow << "," 
	   << (*itCoin).second.gammaHigh << ")" << endl
	   << "Background: (" << (*itCoin).second.bkgLowA << "," 
	   << (*itCoin).second.bkgHighA << ")";
      
      if((*itCoin).second.bkgLowB > 0) { 
	 cout << "and (" << (*itCoin).second.bkgLowB
	      << "," << (*itCoin).second.bkgHighB << ")" << endl;
      }else {
	 cout << endl;
      }
      
      cout << "The following gammas are in coincidence: ";
      for(vector<unsigned int>::iterator itTemp = 
	     (*itCoin).second.coincidences.begin();
	  itTemp != (*itCoin).second.coincidences.end(); itTemp++) {
	 cout << *itTemp << " ";
      }
      cout << endl << "Comments: " << (*itCoin).second.comment << endl << endl;
   }
}


//********** OutputFitInfo *********
void GammaOrganizer::OutputFitInfo(const int &searchEnergy)
{
   FitInfoMap::iterator itFit = fitInfo.find(searchEnergy);

   if(itFit == fitInfo.end()) {
      cout << endl << "Fit Information" << endl
	   << "-----------------------" << endl
	   << "There is no fit information for this gamma." << endl << endl; 
   } else {   
      cout << endl << "Fit Information" << endl
	   << "-------------------" << endl
	   << "Fit Spectrum: " << (*itFit).second.spectrum << endl
	   << "Fit Range: (" << (*itFit).second.low << "," 
	   << (*itFit).second.high << ")" << endl
	   << "Centroid: " << (*itFit).second.centroid << " keV" << endl
	   << "Area: " << (*itFit).second.area
	   << " +- " << ((*itFit).second.percentError/100)*(*itFit).second.area 
	   << endl << "FWHM: " << (*itFit).second.fwhm << " keV" << endl
	   << "Low/High Method?: ";
      
      if((*itFit).second.lowHighMethod) {
	 cout << "Yes" << endl;
      }else
	 cout << "No" << endl;
   }
}


//********** OutputGammaInformation **********
void GammaOrganizer::OutputGammaInformation(const int &searchEnergy, 
					    const string &verbosity)
{
   ReadGeneralInformation();
   OutputGenInfo(searchEnergy);
   
   if(verbosity == "-c") {
      ReadCoincidenceInformation();
      OutputCoinInfo(searchEnergy);
   }else if(verbosity == "-f") {
      ReadFitInformation();
      OutputFitInfo(searchEnergy);
   }else if(verbosity == "-v") {
      ReadCoincidenceInformation();
      ReadFitInformation();
      OutputFitInfo(searchEnergy);
      OutputCoinInfo(searchEnergy);
   }
}


//********** OutputGammaRange **********
void GammaOrganizer::OutputGammaRange(const int &searchEnergyA, 
				      const int &searchEnergyB, 
				      const string &verbosity)
{
   int low, high;
   vector<int> gammas;
   
   if(searchEnergyA > searchEnergyB) {
      low  = searchEnergyB;
      high = searchEnergyA;
   }else {
      low  = searchEnergyA;
      high = searchEnergyB;
   }

   ReadGeneralInformation();
   
   for(int i = low; i <= high; i++) {
      GenInfoMap::iterator it = genInfo.find(i);
      if(it != genInfo.end()) {
	 gammas.push_back((*it).first);
      }
   }
   
   if(verbosity == "-l") {
      cout << endl << "The following gammas were found in this range: ";
      for(vector<int>::iterator it = gammas.begin(); it != gammas.end(); it++) {
	 cout << *it << " ";
      }
      cout << endl << endl;
   }else if(verbosity == "-g") {
      for(vector<int>::iterator it = gammas.begin(); it != gammas.end(); it++)
	 OutputGenInfo(*it);
   }else {
      OutputHelpInfo();
   }
}


//********** OutputGenInfo **********
void GammaOrganizer::OutputGenInfo(const int &searchEnergy)
{
   GenInfoMap::iterator itOrig = genInfo.find(searchEnergy);
   GenInfoMap::iterator itMOne = genInfo.find(searchEnergy - 1);
   GenInfoMap::iterator itPOne = genInfo.find(searchEnergy + 1);
   
   bool orig = itOrig != genInfo.end();
   bool mOne = itMOne != genInfo.end();
   bool pOne = itPOne != genInfo.end();
   
   if(orig) {
      cout << endl << "General Information" << endl 
	   << "-------------------------" << endl
	   << "Energy: " << (*itOrig).first << " keV" << endl
	   << "Found in spectrum(a): " << (*itOrig).second.spectrum  << endl 
    	   << "Nucleus: " << (*itOrig).second.nucleus << endl 
	   << "Half-Life: " << (*itOrig).second.halfLife << " ms" << endl
    	   << "Comments: " << (*itOrig).second.comment << endl;
   }else if(mOne) {
      cout << endl
	   << "There is no gamma recoreded with an energy of " 
	   << searchEnergy << " keV." << endl 
	   << "I did find one with an energy of " << searchEnergy - 1 << " keV." 
	   << endl << endl;
      exit(1);
   }else if(pOne) {
      cout << endl
	   << "There is no gamma recoreded with an energy of " 
	   << searchEnergy << " keV." << endl 
	   << "I did find one with an energy of " << searchEnergy + 1 << " keV." 
	   << endl << endl;
      exit(1);
   }else {
      cout << endl 
	   << "The specified gamma could not be found. " 
	   << "Are you sure you typed it right?" 
	   << endl << endl;
      exit(1);
   }
}


//********** OutputHelpInfo **********
void GammaOrganizer::OutputHelpInfo(void)
{
   cout << endl << "A program to help organize gammas." << endl
	<< "Example: ./gammaSearch 511" << endl
	<< "Optional arguments (place before the energy): " << endl
	<< "-h -> displays this message" << endl
	<< "-g -> outputs general information (default)" << endl
	<< "-c -> outputs general + coincidence information" << endl
	<< "-f -> outputs general + fit information" << endl
	<< "-v -> outputs all information" << endl 
	<< "To search in an energy range simply enter the " 
	<< "low and high bound." << endl
	<< "General output can be generated for a range by passing" 
	<< "the \"-g\" flag." << endl
	<< "Problems? Contact stanpaulauskas@gmail.com" << endl << endl;
}


//********** ParseLineDouble **********
double GammaOrganizer::ParseLineDouble(string &line)
{
   double temp = atof(line.substr(0, line.find(" ")).c_str());
   line.erase(0, line.find(" ") + 1);
   return(temp);
}


//********** ParseLineInt *********
int GammaOrganizer::ParseLineInt(string &line)
{
   int temp = atoi(line.substr(0, line.find(" ")).c_str());
   line.erase(0, line.find(" ") + 1);
   return(temp);
}


//********** ParseLineString **********
string GammaOrganizer::ParseLineString(string &line)
{
   string temp = line.substr(0, line.find(" ")).c_str();
   line.erase(0, line.find(" ") +1);
   return (temp);
}


//********** QuoteError **********
void GammaOrganizer::QuoteError(const string &fileName, const int &lineNo)
{
   cout << endl << "Cannot find comment/coincidence quotes in \"" 
	<< fileName << "\".  At line: " << lineNo << "." << endl
	<< "This is a fatal error." << endl << endl;

   exit(1);
}


//********** ReadCoincidenceInformation ***********
void GammaOrganizer::ReadCoincidenceInformation(void)
{
   int lineNo = 0;
   ifstream coinInfoFile("coincidenceInfo.dat");
   if(coinInfoFile.is_open()) {
      while(coinInfoFile.good()) {
	 lineNo++;

	 CoincidenceInformation data;
	 string line;

	 getline(coinInfoFile, line);
	 if(line.find("#") != string::npos || line == "")
	    continue;

	 //Get the Coincidence gammas
	 size_t foundVecStart = line.find("\"");
	 size_t foundVecEnd   = line.find("\"", int(foundVecStart)+1);
	 int coinStringLen = foundVecEnd - foundVecStart;

	 if(foundVecStart == string::npos || foundVecEnd == string::npos)
	    QuoteError("coincidenceInfo.dat", lineNo);

	 string coinGammas = 
	    line.substr(int(foundVecStart) + 1, coinStringLen - 1);
	 line.erase(foundVecStart, coinStringLen + 1);

	 while(!coinGammas.empty()) {
	    size_t found = coinGammas.find(" ");
	    int energy = atoi(coinGammas.substr(0, found).c_str());
	    data.coincidences.push_back(energy);
	    coinGammas.erase(0,found+1);
	    if(found == string::npos)
	       coinGammas.clear();
	 }

	 sort(data.coincidences.begin(), data.coincidences.end());

	 //Get the Comment
	 size_t foundComStart = line.find("\"");
	 size_t foundComEnd   = line.find("\"", int(foundComStart)+1);
	 int commentSize = foundComEnd - foundComStart;

	 if(foundComStart == string::npos || foundComEnd == string::npos)
	    QuoteError("coincidenceInfo.dat", lineNo);

	 data.comment = line.substr(int(foundComStart) + 1, commentSize - 1);
	 line.erase(foundComStart, commentSize + 1);

	 int gammaEnergy = ParseLineInt(line);
	 data.spectrum   = ParseLineInt(line);
	 data.gammaLow   = ParseLineInt(line);
	 data.gammaHigh  = ParseLineInt(line);
	 data.bkgLowA    = ParseLineInt(line);
	 data.bkgHighA   = ParseLineInt(line);
	 data.bkgLowB    = ParseLineInt(line);
	 data.bkgHighB   = ParseLineInt(line);

	 coinInfo.insert(make_pair(gammaEnergy, data));
      } // while(coinInfoFile.good())
   }else {
      cout << "Couldn't open \"coincidenceInfo.dat\"" 
	   << "  This is fatal!" << endl << "Exiting" << endl;
      exit(2);
   } // if(coinInfoFile.is_open())
}//void GammaOrganizer::ReadCoincidenceInformation


//********** ReadGeneralInformation **********
void GammaOrganizer::ReadGeneralInformation(void) 
{
   int lineNo = 0;

   ifstream genInfoFile("generalInfo.dat");
   if(genInfoFile.is_open()) {
      while(genInfoFile.good()) {
	 lineNo++;

	 GeneralInformation data;
	 string line;
	 
	 getline(genInfoFile, line);

	 if(line.find("#") != string::npos || line == "")
	    continue;

	 size_t foundQuoteBegin = line.find("\"");
	 size_t foundQuoteEnd   = line.find("\"", int(foundQuoteBegin)+1);
	 int commentSize = foundQuoteEnd - foundQuoteBegin;

	 if(foundQuoteBegin == string::npos || foundQuoteEnd == string::npos)
	    QuoteError("generalInfo.dat", lineNo);

	 data.comment = line.substr(int(foundQuoteBegin) + 1, commentSize -1);
	 line.erase(foundQuoteBegin);

	 int gammaEnergy  = ParseLineInt(line);
	 data.spectrum = ParseLineInt(line);
	 data.nucleus  = ParseLineString(line);
	 data.halfLife = ParseLineDouble(line);

	 genInfo.insert(make_pair(gammaEnergy, data));
      } // while(genInfoFile.good())
   }else {
      cout << "Couldn't open \"generalInfo.dat\"" 
	   << "  This is fatal!" << endl << "Exiting" << endl;
      exit(2);
   } // if(genInfoFile.is_open())
}


//********** ReadFitInformation(void) **********
void GammaOrganizer::ReadFitInformation(void) 
{
   ifstream fitInfoFile("fitInfo.dat");
   if(fitInfoFile.is_open()) {
      while(fitInfoFile.good()) {
	 FitInformation data;
	 string line;
	 
	 getline(fitInfoFile, line);
	 if(line.find("#") != string::npos || line == "")
	    continue;

	 int gammaEnergy    = ParseLineInt(line);
	 data.spectrum      = ParseLineInt(line);
	 data.low           = ParseLineInt(line);
	 data.high          = ParseLineInt(line);
	 data.centroid      = ParseLineDouble(line);
	 data.area          = ParseLineInt(line);
	 data.percentError  = ParseLineDouble(line);
	 data.fwhm          = ParseLineDouble(line);
	 data.lowHighMethod = ParseLineInt(line) == 1;

	 fitInfo.insert(make_pair(gammaEnergy, data));
      } // while(fitInfoFile.good())
   }else {
      cout << "Couldn't open \"fitInfo.dat\"" 
	   << "  This is fatal!" << endl << "Exiting" << endl;
      exit(2);
   } // if(fitInfoFile.is_open())
}



