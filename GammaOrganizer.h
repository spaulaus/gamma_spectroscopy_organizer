/** \file GammaOrganizer.h
 * Class for Organizing Gammas
 * Written: S.V. Paulauskas - 17 Aug 2011
 */
#ifndef __GAMMAORGANIZER_H_
#define __GAMMAORGANIZER_H_

#include <map>
#include <string>
#include <vector>

class GammaOrganizer
{
 public:
   GammaOrganizer(const int &numArgs);
   ~GammaOrganizer();

   static void OutputGammaInformation(const int &searchEnergy, 
				      const std::string &verbosity);
   static void ReadGeneralInformation(void);
   static void ReadCoincidenceInformation(void);
   static void ReadFitInformation(void);

 protected: 
   //EMPTY

 private:
   struct GeneralInformation {
      //General information
      double halfLife;
      std::string nucleus;
      std::string comment;
      int spectrum;
   };
   struct CoincidenceInformation {
      //Coincidence Information
      unsigned int gammaHigh;
      unsigned int gammaLow;
      unsigned int bkgHighA;
      unsigned int bkgHighB;
      unsigned int bkgLowA;
      unsigned int bkgLowB;
      unsigned int spectrum;
      std::string comment;
      std::vector<unsigned int> coincidences;
   };
   struct FitInformation {
      //Fit Information
      bool lowHighMethod;
      double centroid;
      double fwhm;
      double percentError;
      unsigned int area;
      unsigned int high;
      unsigned int low;
      unsigned int spectrum;
   };

   static void OutputCoinInfo(const int &searchEnergy);
   static void OutputGenInfo(const int &searchEnergy);
   static void OutputFitInfo(const int &searchEnergy);
   static void OutputHelpInfo(void);

   static int ParseLineInt(std::string &line);
   static double ParseLineDouble(std::string &line);
   static std::string ParseLineString(std::string &line);

   typedef std::map<unsigned int, struct GeneralInformation> GenInfoMap;
   typedef std::map<unsigned int, struct CoincidenceInformation> CoinInfoMap;
   typedef std::map<unsigned int, struct FitInformation> FitInfoMap;
   
   static GenInfoMap genInfo;
   static CoinInfoMap coinInfo;
   static FitInfoMap fitInfo;
};
#endif //__GAMMAS_H_
