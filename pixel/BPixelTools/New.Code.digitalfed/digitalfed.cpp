#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iomanip>
#include <math.h>
#include <ctime>
#include <cstring>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <bitset>
#include <vector>

#include <iterator>
#include <algorithm>
#include <string>

#include "uhal/uhal.hpp"
#include "/home/cmspixel/FEDtool/Utils/Utilities.h"
#include "/home/cmspixel/FEDtool/HWDescription/PixFED.h"
#include "/home/cmspixel/FEDtool/HWInterface/PixFEDInterface.h"
#include "/home/cmspixel/FEDtool/System/SystemController.h"
#include "/home/cmspixel/FEDtool/AMC13/Amc13Controller.h"
#include "/home/cmspixel/FEDtool/Utils/Data.h"
#include "/home/cmspixel/FEDtool/AMC13/Amc13Description.h"
#include "/home/cmspixel/FEDtool/HWInterface/PixFEDFWInterface.h"

#include "CAENVMElib.h"  // CAEN library prototypes
#include "VMELock.h"
using namespace std;

// HAL includes
// #include "VMEDevice.hh"
// #include "VMEAddressTable.hh"
// #include "VMEAddressTableASCIIReader.hh"
// #include "CAENLinuxBusAdapter.hh"

// Pixel includes
//#include "PixelFEDInterface/include/PixelFEDInterface.h" // PixFED class definitionl
//#include "PixelFEDInterface/include/PixelFEDFifoData.h" // PixFED data decode
//#include "CalibFormats/SiPixelObjects/interface/PixelFEDCard.h" //The FED settings structure

//using namespace pos;

#include "SimpleCommand.h"
#include "MultiplexingServer.h"

#include "TBMdecode.cc"



//-----------------------------------------

//#define FEDBASE  0x3C000000
//#define FEDBASE  0x4C000000
#define FEDBASE  0x5C000000

#define LAD_N  		(FEDBASE)
#define LAD_NC  	(FEDBASE+0x200000)
#define LAD_SC  	(FEDBASE+0x400000)
#define LAD_S  	        (FEDBASE+0x600000)
#define LAD_C   	(FEDBASE+0x800000)
#define BRDCST_FR       (FEDBASE+0xe00000) 
#define READ_MA         (FEDBASE+0xa00000)  
#define READ_GA         (FEDBASE+0xa00004)
#define I2C_RD_STAT     (FEDBASE+0xa00014)
#define I2C_RES  	(FEDBASE+0xa00008)
#define I2C_LOAD  	(FEDBASE+0xa0000c)
#define I2C_ADDR_RW     (FEDBASE+0xa00010)
#define I2C_RD_DATA     (FEDBASE+0xa00010)
#define I2C_RD_STAT     (FEDBASE+0xa00014)

#define WR_VME_TestReg  (FEDBASE+0xa08000) 
#define RD_VME_TestReg  (FEDBASE+0xa0000c) 


#define RES_TTCrx  	(FEDBASE+0xa00038)

#define LRES	  	(FEDBASE+0xa00000)
#define CLRES	  	(FEDBASE+0xa00004)
#define nCONFIG  	(FEDBASE+0xa0003c)

//-----------------------------------------

#define TTCVIBASE       0xFFFF00

#define TTCVICSR1	(TTCVIBASE+0x80)
#define TTCVICSR2	(TTCVIBASE+0x82)
#define TTCVI_sBRD	(TTCVIBASE+0xc4)
#define TTCVI_BG0_m	(TTCVIBASE+0x90)
#define TTCVI_BG0_g	(TTCVIBASE+0x96)
#define TTCVI_BG0_d	(TTCVIBASE+0xb0)
#define TTCVI_LF_h	(TTCVIBASE+0xc0)
#define TTCVI_LF_l	(TTCVIBASE+0xc2)
#define TTCVI_IN_dl	(TTCVIBASE+0x92)
#define TTCVI_IN_du	(TTCVIBASE+0x94)


 //------------------------------------------
 
#define CHa_CHb_mux	0x80000000  	//  CHa_CHb_mux=0x80000000 for use with new TBM  
                                        //  CHa_CHb_mux=0x00000000 for use with old TBM  

//------------------------------------------
  
#define write_FITELmemaddr  		0x3100
#define write_FITELdata     		0x3200 
#define FITELreadI2C        		0x2000 
#define FITELwriteI2C       		0x3000 
#define write_selectLEDs    		0x3300 
#define enable_trp_select   		0x2300 
#define write_resetFITEL	  		0x3400
#define write_select_fibers_1to6  0x3700
//------------------------------------------
 
  
/*
CVBoardTypes  VMEBoard = cvV2718;  // define interface type
short         Link = 0;  // define device & link
short         Device = 0;
//    long          BHandle; // pointer to the device
int32_t          BHandle; // pointer to the device
CVErrorCodes  ret; // return code

bool ttcconfigured = true; // JMT must be set up using ttcci_standalone
bool firstenable = true;

unsigned long int CHIP[4],CH_SubAddr[9],CH_SubAddr_Sim[9],CH_Color[9],CLOCKsel[4],FIFO_I_ReadAddr;
int TestData[256],chip,channel,roc,gl_ch_ped[37];
unsigned int ttcaddcsr0=0x180000+0x60;//Base is slot 2 shifted over, 0x60 is BGO register

unsigned long buffer1[128],buffer2[128];
int decodePTrans(unsigned long * data1, unsigned long * data2, const int length);
int decodePTrans2(unsigned long * data1, unsigned long * data2, const int length);
void decode(unsigned long * data1, unsigned long * data2, const int length);
void decode_withROCs(unsigned long * data1, unsigned long * data2, const int length);
int readlastTTS();

int a[4]={0,0,0,0};

int a2[4]={0,0,0,0};
*/

int fedSlot=0;
int fiber=11;
int fedchannel=3;
int select_fibers = 1;
bool piggyN=true;
bool isFitel=true;
bool TBMA=true;


//----------------------------------------------------------
//----------------------------------------------------------
// instantiate System Controller
SystemController cSystemController;
Amc13Controller  cAmc13Controller;
PixFEDFWInterface* fFEDFW;

ofstream file("output.txt");




void exec(SimpleCommand* c);
void StartContTrig ();
void StopContTrig ();
void SendTrigBurst();
void SendNTrigBurst();
void ReadSpyFifo();
void ReadFifo1();
void Test(int q);
void Testprogram(int n); 
void Testprogram(int n, int m);
void ReadErrorFifo();
void ReadData();
void SendEC0();
void SendBGO();
void SendNBGO(int N);
void EnableBGO();
void EnableBGOi(int i);
void DisableBGO();
void DisableBGOi(int j);
void SetBGO(int bgoform, int repeat, int prescale, int BX);
void getTrigger ();
void getBGO ();
void FIFOreset();


int mode, burst, rate ;
int sleeptime;


int main(int argc, char *argv[]) {

  //unsigned long int d;
  

  cout << "Start main " << endl;


  //----------------------------------------------------------
  //initialize FED + AMC13 (see FEDTool/src/fedconfig.cpp)
  //----------------------------------------------------------

  //Lea und Benno temporary hack  //const char* cHWFile = "settings/HWDescription.xml";
  const char* cHWFile = argv[1];

    std::cout << "HW Description File: " << cHWFile << std::endl;

    uhal::setLogLevelTo (uhal::Debug() );

    

    // initialize map of settings so I can know the proper number of acquisitions and TBMs
    cSystemController.InitializeSettings (cHWFile, std::cout);

    // initialize HWdescription from XML, beware, settings have to be read first
    cAmc13Controller.InitializeAmc13 ( cHWFile, std::cout );
    cSystemController.InitializeHw (cHWFile, std::cout);

    auto cSetting = cSystemController.fSettingsMap.find ("NAcq");
    int cNAcq = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 10;
    cSetting = cSystemController.fSettingsMap.find ("BlockSize");
    int cBlockSize = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 2;

    cSetting = cSystemController.fSettingsMap.find ("ChannelOfInterest");
    int cChannelOfInterest = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 0; 

    cSetting = cSystemController.fSettingsMap.find ("ROCOfInterest");
    int cROCOfInterest = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 0;
    
    
    // configure the AMC13
    cAmc13Controller.ConfigureAmc13 ( std::cout );

    //initial values for mode, rate, burst (fix me, should be read from HWDescrition.xml
    mode = 1;
    rate= 1000;
    burst= 100;

    // set sleep time
    sleeptime = 100000/25000*rate;
    
    //configure FED & FITELS & SFP+
    cSystemController.ConfigureHw (std::cout );

    // get the board info of all boards and start the acquistion
    for (auto& cFED : cSystemController.fPixFEDVector)
    {
        //for (auto& cFitel : cFED->fFitelVector)
        //{
        //cSystemController.fFEDInterface->ReadADC(cFitel, cChannelOfInterest, true);
        //}
        //cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases (cFED, cChannelOfInterest);
        cSystemController.fFEDInterface->Start (cFED);
    }
    
    std::cout << "FED Configured, SLink Enabled" << std::endl;

  //-------------------------------------------------------------------------


  
 
  // parse command line arguments
  int port=0; 
  for(int i=2; i<argc; i++){
    if (strcmp(argv[i],"-port")==0){
      i++;
      try{
        port=atoi(argv[i]);
      }catch(...){
        cerr << "bad port number " << argv[i] << endl;
        exit(1);
      }
    }else{
      cerr << "usage: fed [-port <port>] " << endl;
      exit(1);
    }
  }

  cout << "using port " << port << endl;
  
  
  VMELock lock(1);
  lock.acquire();


  //ADD SOCKETS
  SimpleCommand* cmd=new SimpleCommand(); 
  lock.release();
  MultiplexingServer serv(cmd, false);
  if(port){ serv.open(port); }
  cout << "done initializing server " << endl;
  DisableBGO();

  // go
  while(serv.eventloop() ){
    lock.acquire();
    exec(cmd);
    lock.release();   
  }

}

void setAMC13Mode(int mode_) {
  mode = mode_;
}
void setAMC13Rate(int rate_) {
  rate = rate_;
}
void setAMC13Burst(int burst_) {
  burst = burst_;
}


void FindPhases () {
 for (auto& cFED : cSystemController.fPixFEDVector){
   auto cSetting = cSystemController.fSettingsMap.find ("ChannelOfInterest");
   int cChannelOfInterest = (cSetting != std::end (cSystemController.fSettingsMap) ) ? cSetting->second : 0; 
   cSystemController.fFEDInterface->findPhases (cFED, cChannelOfInterest);}
}

void ConfigureAMC13(){
  cAmc13Controller.fAmc13Interface->ConfigureAmc13WithInput(mode, rate, burst);
  sleeptime = 100000/25000*rate; 
}

void StartContTrig () {
  cout << "start continuous triggers " << endl;
  cAmc13Controller.fAmc13Interface->StartL1A();     //start to send unlimited triggers
}

void StopContTrig (){
  cout << "stop continuous triggers " << endl;
  cAmc13Controller.fAmc13Interface->StopL1A();     //stop to send unlimited triggers
}

void SendTrigBurst(){
  cAmc13Controller.fAmc13Interface->BurstL1A();
  cout << "Trigger sent" << endl;
}
   
void SendNTrigBurst(int n){
  for (int i=1; i<=n; i++){
    SendTrigBurst();
      }
  cout << n << " triggers are sent"<< endl;
}

void ReadSpyFifo() {
  for (auto& cFED : cSystemController.fPixFEDVector)
    cSystemController.fFEDInterface->readSpyFIFO (cFED);
}

void ReadFifo1() {
  for (auto& cFED : cSystemController.fPixFEDVector) {     // loops over a range of values contained in cSystemController.fPixFEDVector
    std::vector<uint32_t> cFifo1A;
    std::vector<uint32_t> cFifo1B;
    std::vector<uint32_t> cMarkerA;
    std::vector<uint32_t> cMarkerB;

    std::vector<uint32_t> cFifo1A_temp;
    std::vector<uint32_t> cFifo1B_temp;
    std::vector<uint32_t> cMarkerA_temp;
    std::vector<uint32_t> cMarkerB_temp; 

    

    for (int t= 0; t < 2; t++){
      cSystemController.fFEDInterface->readFIFO1 (cFED,cFifo1A_temp,cFifo1B_temp,cMarkerA_temp, cMarkerB_temp);  
    
 
      for(int i = 0; i < cFifo1A_temp.size(); i++)
	{
	  if (cFifo1A_temp[i] != 0){
	    cFifo1A.push_back(cFifo1A_temp[i]);
	    cFifo1B.push_back(cFifo1B_temp[i]);
	    cMarkerA.push_back(cMarkerA_temp[i]);
	    cMarkerB.push_back(cMarkerB_temp[i]);
	  }
	}
    }
    

    for (int i=0; i < cFifo1A.size(); i++)                         // print out 
      cout  << cMarkerA.at(i)<< "      "  <<  cMarkerB.at(i) << "   line: " <<  i+1  << endl;
  }
}




void Title (int q){
  cout << "#Sent triggers per frequency: " << q*burst << endl;
  cout << "#Frequency [Hz]" << "\tcorrect transmitted H+T" << "\t\terror rate [%]" << "\tuncertainty [%]" << "\tbad or missed events" << "\terror count" << "\tmissing headers" << "\tmissing trailers" << "\tflipped H/T"
       << "\tevent nr errors" << "\ttbm missed trigger" << "\tunknown errors" << "\tevent nr resets" << endl;
}

void TitleGainBias (int q){
  cout << "#Sent triggers per gain and bias: " << q*burst << "\tand with a frequency [Hz] of: " << ((float)4/(float)rate*10000000) << endl;
  cout << "#Gain" << "\tbias" << "\tFrequency" << "\tcorrect transmitted H+T" << "\t\terror rate [%]" << "\tuncertainty [%]" << "\tbad or missed events" 
       << "\terror count" << "\tmissing headers" << "\tmissing trailers" << "\tflipped H/T"<< "\tevent nr errors" << "\ttbm missed trigger" 
       << "\tunknown errors" << "\tevent nr resets" << endl;
}

void TitlePLL (int q){
  cout << "#Sent triggers per delay setting: " << q*burst << "\tand with a frequency [Hz] of: " << ((float)4/(float)rate*10000000) << endl;
  cout << "#160MHz delay setting" << "\t400MHz delay setting" << "\tFrequency" << "\tcorrect transmitted H+T" << "\t\terror rate [%]" << "\tuncertainty [%]" << "\tbad or missed events" 
       << "\terror count" << "\tmissing headers" << "\tmissing trailers" << "\tflipped H/T"<< "\tevent nr errors" << "\ttbm missed trigger" 
       << "\tunknown errors" << "\tevent nr resets" << endl;
}

void TitleROCAB (int q) {
  cout << "#Sent triggers per delay setting: " << q*burst << "\tand with a frequency [Hz] of: " << ((float)4/(float)rate*10000000) << endl;
  cout << "#Delay token Ins by 6.25ns" << "\tdelay Head/Trail by 6.25ns" << "\tdelay port 1" << "\tdelay port 0" << "\tFrequency" << "\tgood words" << "\ttot error" 
       << "\terror in %" << "\tbad or missed events (tbm header/trailer not correct)"  << endl;
}


void Round (float f) {
  f =  (float) round(f*100)/100;
  cout << f  << endl;
}



void TestTBMA(int q) {               // testprogram with trigger bursts and "tbm disable triggers" so just the header and trailer of the tbm.
                                    // but fed fills in fifo1 between tbm header and trailer some tokken data.
  FIFOreset ();


int n = 0;
int j = 0;
int l = 0;
int totevent = 0;
int totwordcounter = 0;
int errorburstcounter = 0;
 int totmissingheader = 0;
 int totmissingtrailer = 0;
 int totunknownerror = 0;
 int toterror = 0;
 int totheaderbit = 0;
 int tottrailerbit = 0;
 int totgoodword = 0;
 int totheadertrailermissorder = 0;
 int expeventID = -999;
 int totmissedtrigger = 0;
 int toteventnrerror = 0;
 int toteventnrreset = 0;
 bool isEmpty = false;
 bool canreadout = false;
 int spam = 0;

 for (int p = 0; p < q; p++){                                   // will go through the data for each burst in the total number of events q
   cAmc13Controller.fAmc13Interface->BurstL1A();                   //sends burst
      usleep(sleeptime);
   for (auto& cFED : cSystemController.fPixFEDVector) {     // loops over a range of values contained in cSystemController.fPixFEDVector

     // read out spyfifo and write header/trailer and event number each in a vector
     vector<uint32_t> cSpyA;
     vector<uint32_t> cSpyB;
     vector<uint32_t> cSpyA_Type;
     vector<uint32_t> cSpyA_ID;
     vector<uint32_t> cSpyB_Type;
     vector<uint32_t> cSpyB_ID;
     bool isEmpty = false;
     int cnt = 0;



     //cout << "this is cSpyA: " << endl;
     //  for (int content = 0; content < cSpyA.size(); content++)
     //     cout << cSpyA_Type[content] << " ";
     //  cout << endl;



     while(!isEmpty && !(cnt > 6)){
     cnt ++;
     cSystemController.fFEDInterface->readSpyFIFO (cFED, cSpyA, cSpyB);
     uint32_t cMask = 0xf0;
     isEmpty = true;
     /*
    cout << "this is cSpyA: " << endl;
     for (int content = 0; content < cSpyA.size(); content++)
       cout << cSpyA_Type[content] << " ";
     cout << endl;
     */
     for (int i=0; i < cSpyA.size(); i++){       // if anything in cSpyA is not zero, we set isEmpty = false and read out again the spyFIFO 
         if (cSpyA[i] != 0)
	   isEmpty = false;
     }      


     //     if (cnt < 6)
     // isEmpty = false;


     int checkHeader = 0;
     int checkTrailer = 0;
 

     int counter = 0;
     int type = 0;
     int id = 0;

     for (int a=0; a < cSpyA.size(); a++)
       if (cSpyA[a]!=0)
	 canreadout = true;

     if (canreadout = true){
       // cout << "we are reading out" << endl;
       //    cout << cSpyA.size() << endl;
     for (int i=0; i < cSpyA.size(); i++){
       // cout << cSpyA[i];
       if (cSpyA[i] != 0)
	  {	    
	    // Header
	    type = 0;
	    id = 0;
	    int ctr = i;
	    checkHeader = 0;
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 8){
	      type += ((cSpyA[i] & 0xf0) << 8);
	      id += ((cSpyA[i] & 0x0f) << 12);
	      i++;
	      checkHeader++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 9){
	      type += ((cSpyA[i] & 0xf0) << 4);
	      id += ((cSpyA[i] & 0x0f) << 8);	     
	      i++;
	      checkHeader++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xa){
	      type += ((cSpyA[i] & 0xf0) );	     
	      id += ((cSpyA[i] & 0x0f) << 4);
	      i++;
	      checkHeader++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xb){
	      type += ((cSpyA[i] & 0xf0) >> 4);
	      id += ((cSpyA[i] & 0x0f));
	      i++;
	      checkHeader++;
	    }

	    if (type != 0 && (checkHeader == 4 )){
	      cSpyA_Type.push_back(type);
	      cSpyA_ID.push_back(id);
	    }


	    // Trailer
	    type = 0;
	    id = 0;
	    ctr = i;
	    checkTrailer = 0;
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xc){
	      type += ((cSpyA[i] & 0xf0) << 8);	     
	      id += ((cSpyA[i] & 0x0f) << 12);
	      i++;
	      checkTrailer++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xd){
	      type += ((cSpyA[i] & 0xf0) << 4);	      
	      id += ((cSpyA[i] & 0x0f) << 8);
	      i++;
	      checkTrailer++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xe){
	      type += ((cSpyA[i] & 0xf0) );	      
	      id += ((cSpyA[i] & 0x0f) << 4);
	      i++;
	      checkTrailer++;
	    }
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xf){
	      type += ((cSpyA[i] & 0xf0) >> 4);	      
	      id += ((cSpyA[i] & 0x0f));
	       checkTrailer++;
	    }

	    if (type != 0 && (checkTrailer == 4)){
	      cSpyA_Type.push_back(type);
	      cSpyA_ID.push_back(id);
	    }


      }





   }
     // cout << endl;
     }
     else 
       cout << "cSpyA is empty" << endl;
     }
     
     if (cnt >= 7){
       canreadout = false;
       spam++;
       FIFOreset ();
       usleep(1000);
       p--;
       if (spam > 100) {
	 cout  << dec << ((float)4/(float)rate*10000000) << "\t" << 0 << "\t" << 100 << "\t" 
	      << 100  << "\t"   << q*burst << "\t" << q*burst << "\t" 
	      << q*burst  << "\t" <<  q*burst << "\t"  <<  q*burst << "\t" 
	      <<  q*burst << "\t" <<  q*burst << "\t" <<  q*burst << "\t"<<  (q*burst+1) << endl;
	 return;}

      continue;
     }
       

     //analize data of the vecotrs written above


     int missingheadererror=0;
     int missingtrailererror=0;
     int goodword = 0;
     int unknownerror=0;
     int error = 0;
     int headertrailermissorder = 0;
     int missedtrigger = 0;
     int eventnrerror = 0;
     int resetter = 0;
     int eventnrreset = 0;

     int size = 0;

     size = cSpyA_Type.size();


     // read out channel A
     //------------------------------------------------------------------------------------------------------


    if (canreadout = true){
     for (int i=0; i < size; i+=2){
       // counter++;
       if( ((i+1) < size) && (cSpyA_Type[i] == 0x89ab) && (cSpyA_Type[i+1] == 0xcdef)){
	 goodword++;
	   if (expeventID == -999){
	     eventnrreset++;
	     expeventID = (cSpyA_ID[i] & 0xff00) >> 8;}
	   else {
	     expeventID++; //
	     if ((0xff & expeventID) == ((cSpyA_ID[i] & 0xff00) >> 8))
	       // goodword++;
	       ;
	     else if (((0xff & expeventID)-1) == ((cSpyA_ID[i] & 0xff00) >> 8)){                 // TBM missed a trigger
	       error++;
	       missedtrigger++;
	       goodword--;//
	       //  resetter++;
	       expeventID = -999;
	       //cout << "event number: " << ((cSpyA_ID[i] & 0xff00) >> 8) << ", expected: " << (0xff & expeventID) << endl;
	     }
	     else{                                                                          // 1 data got lost or bit flip
	       error++;
	       eventnrerror++;
	       // resetter++;
	       goodword--;
	       expeventID = -999;
	       //  cout << "event number: " << ((cSpyA_ID[i] & 0xff00) >> 8) << ", expected: " << (0xff & expeventID) << endl;
	     }
	     //  while (!((0xff & expeventID) == ((cSpyA_ID[i] & 0xff00) >> 8))){
	     //	 expeventID++;
	     //	 eventnrerror++;
	     //  cout << "event number error: " << ((cSpyA_ID[i] & 0xff00) >> 8) << endl;
	   }
	   
	 }     

       else if ( ((i+3) < size) && (cSpyA_Type[i] == 0xcdef) && (cSpyA_Type[i+1] == 0x89ab))     {
	 error++;
	 expeventID++;
	 expeventID = -999;
	 if ((cSpyA_Type[i+2] == 0x89ab) && (cSpyA_Type[i+3] == 0xcdef))
	   headertrailermissorder++;
	 if ((cSpyA_Type[i+2] == 0xcdef) && (cSpyA_Type[i+3] == 0x89ab)){
	   i--;
	   missingheadererror++;  
	 }
	}

       else if( ((i+3) < size) && (cSpyA_Type[i] == 0x89ab) && (cSpyA_Type[i+1] == 0x89ab))     {
	 error++;
	 missingtrailererror++;
	 expeventID++;
         expeventID = -999;
	 if ((cSpyA_Type[i+2] == 0xcdef) && (cSpyA_Type[i+3] == 0x89ab))
	   i--;
	}


	 else if( ((i+1) < size) && (cSpyA_Type[i] == 0xcdef) && (cSpyA_Type[i+1] == 0xcdef))     {
	 error++;
	 missingheadererror+=2;
	 expeventID+=2;	       
	 expeventID = -999;
	}

	 else{
	 unknownerror++; 
	 error++;
	 expeventID = -999;
	 }

     }
    }
    else {
       error = burst;
       missingheadererror = 0;
       missingtrailererror = 0;
       unknownerror = burst;
       goodword = 0;
       headertrailermissorder = 0;
       missedtrigger = 0;
       eventnrerror = 0;
       eventnrreset = burst + 1;
	}



       toterror += error;
       totmissingheader += missingheadererror;
       totmissingtrailer += missingtrailererror;
       totunknownerror += unknownerror;
       totgoodword += goodword;
       totheadertrailermissorder += headertrailermissorder;
       totmissedtrigger += missedtrigger;
       toteventnrerror += eventnrerror;
       toteventnrreset += eventnrreset;


     /*
     if (error != 0){      
       cout << dec << "SUMMARY OF  THIS BURST: " << dec << p  << endl;
       cout << "\tNumber of sent triggers: " << burst << endl;
       cout << "\tNumber of correct transmitted Header+Trailer: " << goodword << endl;
       cout << "\tError count: " << error << endl;
       cout << "\tNumber of missing header error: " << missingheadererror << endl;
       cout << "\tNumber of missing trailer error: " << missingtrailererror << endl;    
       cout << "\tNumber of flipped header and trailer: " << headertrailermissorder  << endl;
       cout << "\tNumber of unknown error: " << unknownerror << endl;
       cout << "\tNumber of event number errors (missing header+trailer): " << eventnrerror << endl;
       cout << "\tNumber of missed triggers: " << missedtrigger << endl;
       cout << "\tNumber of event number resets: " << eventnrreset << endl;
       cout << "-----------------------------------------" << endl;
     }

     */

   }

 } 



 float disappearedtriggers = 0;               // are this really the disappeared triggers?
 disappearedtriggers = (q*burst - totgoodword - toterror);

 // cout << Frequency << good words << error in % << uncertainty on error in % << bad or missed events << errors << missing header << missing trailer << trailer/header missorder << eventnr errors << missed triggers << unknown errors << eventnr resets << endl;
 /* if (totgoodword ==0)
   cout << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodword << "\t" << "100" << "\t" <<"0" << "\t"   << (q*burst - totgoodword) << "\t" << toterror << "\t" 
	<< totmissingheader << "\t" << totmissingtrailer << "\t" << totheadertrailermissorder << "\t" << toteventnrerror << "\t" << totmissedtrigger << "\t" 
	<< totunknownerror  << "\t"<< toteventnrreset << endl;
 else {
   cout << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodword << "\t" << (((float)(q*burst - totgoodword)/totgoodword)*100) << "\t" 
      << ((float)(q*burst-totgoodword-toterror)/(totgoodword)*100) << "\t"   << (q*burst - totgoodword) << "\t" << toterror << "\t" << totmissingheader << "\t" << totmissingtrailer << "\t" 
	<< totheadertrailermissorder << "\t" << toteventnrerror << "\t" << totmissedtrigger << "\t" << totunknownerror  << "\t"<< toteventnrreset << endl;}

 cout << disappearedtriggers << endl;
 cout << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodword << "\t" << ((float)(1-totgoodword/(q*burst-disappearedtriggers))*100) << "\t" 
      << ((float)(disappearedtriggers)/(q*burst)*100) << "\t"   << (q*burst - totgoodword) << "\t" << toterror << "\t" << totmissingheader << "\t" << totmissingtrailer << "\t" 
	<< totheadertrailermissorder << "\t" << toteventnrerror << "\t" << totmissedtrigger << "\t" << totunknownerror  << "\t"<< toteventnrreset << endl;
*/

 // if (spam != 0){
 //  cout << dec << "cSpyA was " << spam << " spamed and the readout avoided!" << endl;   //spam printer
 //  }

 cout << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodword << "\t" << ((float)(1-((float)totgoodword/(q*burst)))*100) << "\t" 
      << ((float)(disappearedtriggers)/(q*burst)*100) << "\t"   << (q*burst - totgoodword) << "\t" << toterror << "\t" << totmissingheader << "\t" << totmissingtrailer << "\t" 
	<< totheadertrailermissorder << "\t" << toteventnrerror << "\t" << totmissedtrigger << "\t" << totunknownerror  << "\t"<< toteventnrreset << endl;


 /*
     cout << "\n\n-----------------------------------------" << endl;
     cout << "-----------------------------------------" << endl;
     cout << dec << "FINAL SUMMARY: " << endl;
     cout << "\tNumber of total sent triggers: " << q*burst << endl;
     cout << "\tNumber of correct transmitted Header+Trailer: " << totgoodword << endl;
     cout << "\tError count: " << toterror << endl;
     cout << "\tNumber of missing header error: " << totmissingheader << endl;
     cout << "\tNumber of missing trailer error: " << totmissingtrailer << endl;    
     cout << "\tNumber of flipped header and trailer: " << totheadertrailermissorder  << endl;
     cout << "\tNumber of unknown error: " << totunknownerror << endl;
     cout << "\tNumber of event number errors (missing header+trailer): " << toteventnrerror << endl;
     cout << "\tNumber of missed triggers: " << totmissedtrigger << endl;       
     cout << "\tNumber of event number resets: " << toteventnrreset << endl;
     cout << "-----------------------------------------" << endl;
     cout << "Bad or missed events: " << (q*burst - totgoodword) << endl;
     cout << "Error rate: " <<"("<< (((float)(q*burst - totgoodword)/totgoodword)*100) <<" +- " << ((float)(q*burst-totgoodword-toterror)/(totgoodword)*100)<< ")"<< "%" << endl;
     cout << "-----------------------------------------" << endl;
     cout << "-----------------------------------------" << endl;
 */


}



















void FIFOreset (){
  for (auto& cFED : cSystemController.fPixFEDVector){


     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reset", 1);
     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.idel_ctrl_reset", 1);
     usleep(1000); 

     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.initialize_swap", 0);
     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reset", 0);
     cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 0);
     usleep(1000); 
  }
}



void Start () {
  for (auto& cFED : cSystemController.fPixFEDVector){
    cSystemController.fFEDInterface->Start ( cFED );}
}

void Stop () {
  for (auto& cFED : cSystemController.fPixFEDVector){
    cSystemController.fFEDInterface->Stop ( cFED );}
}

void Pause () {
for (auto& cFED : cSystemController.fPixFEDVector){
  cSystemController.fFEDInterface->Pause ( cFED );}
}

void Resume () {
for (auto& cFED : cSystemController.fPixFEDVector){
  cSystemController.fFEDInterface->Resume (cFED );}
}









void TestTBMAB(int q)                // testprogram with trigger bursts and "tbm disable triggers" so just the header and trailer of the tbm.
{                                    // but fed fills in fifo1 between tbm header and trailer some tokken data.

  // cout << "0.0.0";

  FIFOreset ();
  //usleep(1000000);
 int errorburstcounter = 0;




 // only if you want to read out channel A and B at the same time and compare them ------------------------------------------------------------------------------------------

 int spam = 0;

 bool canreadoutA = false;
 bool canreadoutB = false;
 bool isEmptyA = false;
 bool isEmptyB = false;

 int toteventA = 0;
 int totwordcounterA = 0;
 int errorburstcounterA = 0;
 int totmissingheaderA = 0;
 int totmissingtrailerA = 0;
 int totunknownerrorA = 0;
 int toterrorA = 0;
 int totheaderbitA = 0;
 int tottrailerbitA = 0;
 int totgoodwordA = 0;
 int totheadertrailermissorderA = 0;
 int expeventIDA = -999;
 int totmissedtriggerA = 0;
 int toteventnrerrorA = 0;
 int toteventnrresetA = 0;
 float disappearedtriggersA = 0;  

 int toteventB = 0;
 int totwordcounterB = 0;
 int errorburstcounterB = 0;
 int totmissingheaderB = 0;
 int totmissingtrailerB = 0;
 int totunknownerrorB = 0;
 int toterrorB = 0;
 int totheaderbitB = 0;
 int tottrailerbitB = 0;
 int totgoodwordB = 0;
 int totheadertrailermissorderB = 0;
 int expeventIDB = -999;
 int totmissedtriggerB = 0;
 int toteventnrerrorB = 0;
 int toteventnrresetB = 0;
 float disappearedtriggersB = 0;  

 int totgoodgoodword = 0;
    //  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     vector<uint32_t> cSpyA;
     vector<uint32_t> cSpyB;
     vector<uint32_t> cSpyA_Type;
     vector<uint32_t> cSpyA_ID;
     vector<uint32_t> cSpyB_Type;
     vector<uint32_t> cSpyB_ID;
 // cout << "\t0.0"; 
 for (int p = 0; p < q; p++)        // will go through the data for each burst in the total number of events q
 {                        
   cAmc13Controller.fAmc13Interface->BurstL1A();                   //sends burst
      usleep(sleeptime);
   for (auto& cFED : cSystemController.fPixFEDVector)   // loops over a range of values contained in cSystemController.fPixFEDVector
  {    



     // read out spyfifo and write header/trailer and event number each in a vector
    cSpyA.clear();
    cSpyB.clear();
    cSpyA_Type.clear();
    cSpyA_ID.clear();
    cSpyB_Type.clear();
    cSpyB_ID.clear();

    // cout << "\t0"; 

     //read in spyFIFO CHANNEL A
     //------------------------------------------------------------------------------------------------------------------------

     bool isEmptyA = false;

     int cnt = 0;

     bool isEmptyB = false;
     //   cout << "\t0.25";

     while((!isEmptyA || !isEmptyB) && !(cnt > 6))
    {
     cnt++;
     //  cout << "\t0.5"; 
     cSystemController.fFEDInterface->readSpyFIFO (cFED, cSpyA, cSpyB);
     uint32_t cMask = 0xf0;

     isEmptyA = true;
     isEmptyB = true;
     //   cout << "\t1"; 

     for (int i=0; i < cSpyA.size(); i++){       // if anything in cSpyA is not zero, we set isEmptyA = false and read out again the spyFIFO 
         if (cSpyA[i] != 0)
	   isEmptyA = false;}
     for (int i=0; i < cSpyB.size(); i++){       // if anything in cSpyB is not zero, we set isEmptyB = false and read out again the spyFIFO 
         if (cSpyB[i] != 0)
	   isEmptyB = false;}

     int checkHeaderA = 0;
     int checkTrailerA = 0;
     int counterA = 0;
     int typeA = 0;
     int idA = 0;

     int checkHeaderB = 0;
     int checkTrailerB = 0;
     int counterB = 0;
     int typeB = 0;
     int idB = 0;

     for (int a=0; a < cSpyA.size(); a++){
       if (cSpyA[a]!=0)
	 canreadoutA = true;
     }
     for (int a=0; a < cSpyB.size(); a++){
       if (cSpyB[a]!=0)
	 canreadoutB = true;
     }

     // cout << "\t2" << "\tcanreadoutA = " << canreadoutA;; 

     if (canreadoutA = true)
     {
      for (int i=0; i < cSpyA.size(); i++)
      {
       if (cSpyA[i] != 0){	    
	    
	    // Header
	    typeA = 0;
	    idA = 0;
	    int ctrA = i;
	    checkHeaderA = 0;
	    if (((cSpyA[i] & 0xf0) >> 4) == 8){
	      typeA += ((cSpyA[i] & 0xf0) << 8);
	      idA += ((cSpyA[i] & 0x0f) << 12);
	      i++;
	      checkHeaderA++;}

	    if ( ((cSpyA[i] & 0xf0) >> 4) == 9){
	      typeA += ((cSpyA[i] & 0xf0) << 4);
	      idA += ((cSpyA[i] & 0x0f) << 8);	     
	      i++;
	      checkHeaderA++;}
	    
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xa){
	      typeA += ((cSpyA[i] & 0xf0) );	     
	      idA += ((cSpyA[i] & 0x0f) << 4);
	      i++;
	      checkHeaderA++;}
	    
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xb){
	      typeA += ((cSpyA[i] & 0xf0) >> 4);
	      idA += ((cSpyA[i] & 0x0f));
	      i++;
	      checkHeaderA++;}

	    if (typeA != 0 && (checkHeaderA == 4 )){
	      cSpyA_Type.push_back(typeA);
	      cSpyA_ID.push_back(idA);}

	    //  cout << "\t3";

	    // Trailer
	    typeA = 0;
	    idA = 0;
	    ctrA = i;
	    checkTrailerA = 0;
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xc){
	      typeA += ((cSpyA[i] & 0xf0) << 8);	     
	      idA += ((cSpyA[i] & 0x0f) << 12);
	      i++;
	      checkTrailerA++;}
	    
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xd){
	      typeA += ((cSpyA[i] & 0xf0) << 4);	      
	      idA += ((cSpyA[i] & 0x0f) << 8);
	      i++;
	      checkTrailerA++;}
	    
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xe){
	      typeA += ((cSpyA[i] & 0xf0) );	      
	      idA += ((cSpyA[i] & 0x0f) << 4);
	      i++;
	      checkTrailerA++;}
	    
	    if ( ((cSpyA[i] & 0xf0) >> 4) == 0xf){
	      typeA += ((cSpyA[i] & 0xf0) >> 4);	      
	      idA += ((cSpyA[i] & 0x0f));
	       checkTrailerA++;}

	    if (typeA != 0 && (checkTrailerA == 4)){
	      cSpyA_Type.push_back(typeA);
	      cSpyA_ID.push_back(idA);}}
	    
      }
     }
     else 
       cout << "cSpyA is empty" << endl;

     //   cout << "\t4" << "\tcanreadoutB = " << canreadoutB;

     if (canreadoutB = true)
     {
      for (int i=0; i < cSpyB.size(); i++)
      {
       if (cSpyB[i] != 0){	    
	    
	    // Header
	    typeB = 0;
	    idB = 0;
	    int ctrB = i;
	    checkHeaderB = 0;
	    if (((cSpyB[i] & 0xf0) >> 4) == 8){
	      typeB += ((cSpyB[i] & 0xf0) << 8);
	      idB += ((cSpyB[i] & 0x0f) << 12);
	      i++;
	      checkHeaderB++;}

	    if ( ((cSpyB[i] & 0xf0) >> 4) == 9){
	      typeB += ((cSpyB[i] & 0xf0) << 4);
	      idB += ((cSpyB[i] & 0x0f) << 8);	     
	      i++;
	      checkHeaderB++;}
	    
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xa){
	      typeB += ((cSpyB[i] & 0xf0) );	     
	      idB += ((cSpyB[i] & 0x0f) << 4);
	      i++;
	      checkHeaderB++;}
	    
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xb){
	      typeB += ((cSpyB[i] & 0xf0) >> 4);
	      idB += ((cSpyB[i] & 0x0f));
	      i++;
	      checkHeaderB++;}

	    if (typeB != 0 && (checkHeaderB == 4 )){
	      cSpyB_Type.push_back(typeB);
	      cSpyB_ID.push_back(idB);}

	    //   cout << "\t5";
	    // Trailer
	    typeB = 0;
	    idB = 0;
	    ctrB = i;
	    checkTrailerB = 0;
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xc){
	      typeB += ((cSpyB[i] & 0xf0) << 8);	     
	      idB += ((cSpyB[i] & 0x0f) << 12);
	      i++;
	      checkTrailerB++;}
	    
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xd){
	      typeB += ((cSpyB[i] & 0xf0) << 4);	      
	      idB += ((cSpyB[i] & 0x0f) << 8);
	      i++;
	      checkTrailerB++;}
	    
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xe){
	      typeB += ((cSpyB[i] & 0xf0) );	      
	      idB += ((cSpyB[i] & 0x0f) << 4);
	      i++;
	      checkTrailerB++;}
	    
	    if ( ((cSpyB[i] & 0xf0) >> 4) == 0xf){
	      typeB += ((cSpyB[i] & 0xf0) >> 4);	      
	      idB += ((cSpyB[i] & 0x0f));
	       checkTrailerB++;}

	    if (typeB != 0 && (checkTrailerB == 4)){
	      cSpyB_Type.push_back(typeB);
	      cSpyB_ID.push_back(idB);}}
	    
      }
     }
     else 
       cout << "cSpyB is empty" << endl;


    }
     


     //read in finished 
     //---------------------------------------------------------------------------------------------------------


     if (cnt >= 7){                  //looks, whether there was a infinity loop in read out data
       canreadoutA = false;
       canreadoutB = false;  //
       spam++;
       FIFOreset();
       usleep(1000);       
       p--;
       if (spam > 100) {
	 cout << "G" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << 0 << "\t" << 100 << "\t" 
	      << 100  << "\t"   << q*burst << "\t" << 2*q*burst << "\t" 
	      << 2*q*burst  << "\t" <<  2*q*burst << "\t"  <<  2*q*burst << "\t" 
	      <<  2*q*burst << "\t" <<  2*q*burst << "\t" <<  2*q*burst << "\t"<<  (2*q*burst+2) << endl;

	 cout << "A" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << 0 << "\t" << 100 << "\t" 
	      << 100  << "\t"   << q*burst << "\t" << q*burst << "\t" 
	      << q*burst  << "\t" <<  q*burst << "\t"  <<  q*burst << "\t" 
	      <<  q*burst << "\t" <<  q*burst << "\t" <<  q*burst << "\t"<<  (q*burst+1) << endl;

	 cout << "B" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << 0 << "\t" << 100 << "\t" 
	      << 100  << "\t"   << q*burst << "\t" << q*burst << "\t" 
	      << q*burst  << "\t" <<  q*burst << "\t"  <<  q*burst << "\t" 
	      <<  q*burst << "\t" <<  q*burst << "\t" <<  q*burst << "\t"<<  (q*burst+1) << endl;
	 return;}

       //  cout << "spam: " << spam << endl;
       //break;
       continue;}




     //analize data of the vecotrs written above

     //  cout << "\t6";
     int missingheadererror=0;
     int missingtrailererror=0;
     int goodword = 0;
     int unknownerror=0;
     int error = 0;
     int headertrailermissorder = 0;
     int missedtrigger = 0;
     int eventnrerror = 0;
     int resetter = 0;
     int eventnrreset = 0;
     //   int counter = 0;



     // read out channel A & B
     //------------------------------------------------------------------------------------------------------
     //-------------------------------------------------------------------------------------------------------------   
    
     int size = 0;
     bool Agoodword = false;
     int iA = 0;
     int iB = 0;
     int goodgoodword = 0;

     int missingheadererrorA=0;
     int missingtrailererrorA=0;
     int goodwordA = 0;
     int unknownerrorA=0;
     int errorA = 0;
     int headertrailermissorderA = 0;
     int missedtriggerA = 0;
     int eventnrerrorA = 0;
     int resetterA = 0;
     int eventnrresetA = 0;

     int missingheadererrorB=0;
     int missingtrailererrorB=0;
     int goodwordB = 0;
     int unknownerrorB=0;
     int errorB = 0;
     int headertrailermissorderB = 0;
     int missedtriggerB = 0;
     int eventnrerrorB = 0;
     int resetterB = 0;
     int eventnrresetB = 0;

     int sizeA = 0;
     int sizeB = 0;
     //   cout << "\t6.1";


     sizeA = cSpyA_Type.size();
     sizeB = cSpyB_Type.size();

     if (sizeA > sizeB)
        size = sizeA;
     else
	size = sizeB;

     //  cout << "\t6.2";

     //   cout << "size SpyA_Type: " << cSpyA_Type.size() << "\tsize SpyB_Type: " << cSpyB_Type.size() << endl;

     // for (int i=0; i < cSpyA.size(); i++){
     //  cout << "6.2.5." << endl;
       //cout << cSpyA_Type[i] << "\t";}
                   



     // cout << endl; 
     //cout << " ----------------------------------------------------------------------------------------" << endl;
     //   for (int i=0; i < cSpyB.size(); i++){
	   // cout << cSpyB_Type[i] << "\t"; }
	                    
     //cout << endl; 

     //  cout << "\t6.3";

     //cout << endl;

     //  cout << "\tcanreadoutA = " << canreadoutA << "\tcanreadoutB = " << canreadoutB << endl;
     //   cout << "size SpyA_Type: " << cSpyA_Type.size() << "\tsize SpyB_Type: " << cSpyB_Type.size() << endl;
     while ((iB < size) && (iA < size)){
       Agoodword = false;
       //   cout << "\t7";
      
       if(( ((iA+1) < sizeA) && cSpyA_Type[iA] == 0x89ab) && (cSpyA_Type[iA+1] == 0xcdef) ){
	 goodwordA++;
	 //    cout << "\t7.1";
	   if (expeventIDA == -999){
	     // cout << "\t7.2";
	     eventnrresetA++;
	     Agoodword = true;
	     expeventIDA = (cSpyA_ID[iA] & 0xff00) >> 8;}
	   else {
	     //   cout << "\t7.3";
	     expeventIDA++; 
	     if ((0xff & expeventIDA) == ((cSpyA_ID[iA] & 0xff00) >> 8)){
	       Agoodword = true;
	       // cout << "\t7.3.5";
	     }	      
	     else if (((0xff & expeventIDA)-1) == ((cSpyA_ID[iA] & 0xff00) >> 8)){                 // TBM missed a trigger
	       //  cout << "\t7.4";
	       errorA++;
	       missedtriggerA++;
	       goodwordA--;
	       expeventIDA = -999;}
   
	     else{                                                                          // 1 data got lost or bit flip
	       //  cout << "\t7.5";
	       errorA++;
	       eventnrerrorA++;
	       goodwordA--;
	       expeventIDA = -999;
	     }
	   }	   
	 }     
       else if(( ((iA+3) < sizeA) && cSpyA_Type[iA] == 0xcdef) && (cSpyA_Type[iA+1] == 0x89ab)  )     {
	 //  cout << "\t7.6";
	 errorA++;
	 expeventIDA = -999;
	 if ((cSpyA_Type[iA+2] == 0x89ab) && (cSpyA_Type[iA+3] == 0xcdef))
	   headertrailermissorderA++;
	 if ((cSpyA_Type[iA+2] == 0xcdef) && (cSpyA_Type[iA+3] == 0x89ab)){
	   iA--;
	   missingheadererrorA++;  
	 }
	}
       else if(( ((iA+3) < sizeA) && cSpyA_Type[iA] == 0x89ab) && (cSpyA_Type[iA+1] == 0x89ab) ){
	 //  cout << "\t7.7";
	 errorA++;
	 missingtrailererrorA++;
	 expeventIDA++;
         expeventIDA = -999;
	 if ((cSpyA_Type[iA+2] == 0xcdef) && (cSpyA_Type[iA+3] == 0x89ab))
	   iA--;}
       else if( ((iA + 1) < sizeA) && (cSpyA_Type[iA] == 0xcdef) && (cSpyA_Type[iA+1] == 0xcdef)){
	 //  cout << "\t7.8";
         errorA++;
         missingheadererrorA+=2;	       
	 expeventIDA = -999;}
       else if (iA < sizeA){
	 //  cout << "\t7.9";
	 unknownerrorA++; 
	 errorA++;
	 expeventIDA = -999;}
       //    cout << "\t8";

       if( (iB < sizeB) && (cSpyB_Type[iB] == 0x89ab) && (cSpyB_Type[iB+1] == 0xcdef) ){
	 goodwordB++;
	 if (expeventIDB == -999){
	     eventnrresetB++;
	     expeventIDB = (cSpyB_ID[iB] & 0xff00) >> 8;
	     if (Agoodword == true){
	       goodgoodword++;}}
	 else {
	     expeventIDB++; 
	     if ((0xff & expeventIDB) == ((cSpyB_ID[iB] & 0xff00) >> 8)){
	       if (Agoodword == true){
		 Agoodword = false;
		 // cout << "\tiB: " << iB; 
		 goodgoodword++;}                                                         // falls beide (zur gleichen Zeit! -> muss noch implementiert werden) wahr sind, zaehlt es eins hoch
	     }
	     else if (((0xff & expeventIDB)-1) == ((cSpyB_ID[iB] & 0xff00) >> 8)){                 // TBM missed a trigger
	       errorB++;
	       missedtriggerB++;
	       goodwordB--;
	       expeventIDB = -999;}
	     else{                                                                          // 1 data got lost or bit flip
	       errorB++;
	       eventnrerrorB++;
	       goodwordB--;
	       expeventIDB = -999;}
	   }
       }
  

       else if( ((iB+3) < sizeB) && (cSpyB_Type[iB] == 0xcdef) && (cSpyB_Type[iB+1] == 0x89ab)){
	 errorB++;
	 expeventIDB++;
	 expeventIDB = -999;
	 if ((cSpyB_Type[iB+2] == 0x89ab) && (cSpyB_Type[iB+3] == 0xcdef))
	   headertrailermissorderB++;
	 if ((cSpyB_Type[iB+2] == 0xcdef) && (cSpyB_Type[iB+3] == 0x89ab)){
	   iB--;
	   missingheadererrorB++;}}  

       else if( ((iB+3) < sizeB) && (cSpyB_Type[iB] == 0x89ab) && (cSpyB_Type[iB+1] == 0x89ab)){
	 errorB++;
	 missingtrailererrorB++;
	 expeventIDB++;
         expeventIDB = -999;
	 if ((cSpyB_Type[iB+2] == 0xcdef) && (cSpyB_Type[iB+3] == 0x89ab))
	   iB--;}


       else if( ((iB+1) < sizeB) && (cSpyB_Type[iB] == 0xcdef) && (cSpyB_Type[iB+1] == 0xcdef)){
	 errorB++;
	 missingheadererrorB+=2;
	 expeventIDB+=2;	       
	 expeventIDB = -999;}

	 else if (iB < sizeB){
	 unknownerrorB++; 
	 errorB++;
	 expeventIDB = -999;}
 //cout << "\tID A: " << (expeventIDA & 0xff) << "\tID B: " << (expeventIDB & 0xff);
 iA += 2;
 iB += 2;
 Agoodword = false;
 //  cout << "\t9";

   }



     if (errorA >= 90){
       cout << "FEHLER" << endl;
       for (int i=0; i<cSpyA_Type.size(); i++)
	 cout << cSpyA_Type[i] << "  " ;
       cout << endl;
     }

     //    cout << "burst: " << p << "\tgoodwordA: " << goodwordA << "\tgoodwordB: " << goodwordB << "\tgoodgoodword: " << goodgoodword << endl;
     //cout << "\t9.5";
     totgoodgoodword += goodgoodword;

       toterrorA += errorA;
       totmissingheaderA += missingheadererrorA;
       totmissingtrailerA += missingtrailererrorA;
       totunknownerrorA += unknownerrorA;
       totgoodwordA += goodwordA;
       totheadertrailermissorderA += headertrailermissorderA;
       totmissedtriggerA += missedtriggerA;
       toteventnrerrorA += eventnrerrorA;
       toteventnrresetA += eventnrresetA;             
       disappearedtriggersA = (q*burst - totgoodwordA - toterrorA);       // are this really the disappeared triggers?


       toterrorB += errorB;
       totmissingheaderB += missingheadererrorB;
       totmissingtrailerB += missingtrailererrorB;
       totunknownerrorB += unknownerrorB;
       totgoodwordB += goodwordB;
       totheadertrailermissorderB += headertrailermissorderB;
       totmissedtriggerB += missedtriggerB;
       toteventnrerrorB += eventnrerrorB;
       toteventnrresetB += eventnrresetB;
       disappearedtriggersB = (q*burst - totgoodwordB - toterrorB);   // are this really the disappeared triggers?

  //-----------------------------------------------------------------------------------------------


  }
 } 



 //   cout << "\t10";
 // cout << Frequency << good words << error in % << uncertainty on error in % << bad or missed events << errors << missing header << missing trailer << trailer/header missorder << eventnr errors << missed triggers << unknown errors << eventnr resets << endl;


 //cout << dec << "Number of events, where channel A and B are fine: " << totgoodgoodword << endl;

 cout << "G" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodgoodword << "\t" << ((float)(1-((float)totgoodgoodword/(q*burst)))*100) << "\t" 
      << ((float)(disappearedtriggersA + disappearedtriggersB)/(2*q*burst)*100)  << "\t"   << (q*burst - totgoodgoodword) << "\t" << (toterrorA + toterrorB) << "\t" 
      << (totmissingheaderA + totmissingheaderB) << "\t" << (totmissingtrailerA + totmissingtrailerB) << "\t"  << (totheadertrailermissorderA + totheadertrailermissorderB) << "\t" 
      << (toteventnrerrorA + toteventnrerrorB) << "\t" << (totmissedtriggerA + totmissedtriggerB) << "\t" << (totunknownerrorA + totunknownerrorB) << "\t"<< (toteventnrresetA + toteventnrresetB) << endl;

 cout << "A" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodwordA << "\t" << ((float)(1-((float)totgoodwordA/(q*burst)))*100) << "\t" 
      << ((float)(disappearedtriggersA)/(q*burst)*100) << "\t"   << (q*burst - totgoodwordA) << "\t" << toterrorA << "\t" << totmissingheaderA << "\t" << totmissingtrailerA << "\t" 
      << totheadertrailermissorderA << "\t" << toteventnrerrorA << "\t" << totmissedtriggerA << "\t" << totunknownerrorA  << "\t"<< toteventnrresetA << endl;

 cout << "B" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodwordB << "\t" << ((float)(1-((float)totgoodwordB/(q*burst)))*100) << "\t" 
      << ((float)(disappearedtriggersB)/(q*burst)*100) << "\t"   << (q*burst - totgoodwordB) << "\t" << toterrorB << "\t" << totmissingheaderB << "\t" << totmissingtrailerB << "\t" 
	<< totheadertrailermissorderB << "\t" << toteventnrerrorB << "\t" << totmissedtriggerB << "\t" << totunknownerrorB  << "\t"<< toteventnrresetB << endl;

 if (spam != 0){
   cout << dec << "cSpy was " << spam << " spamed and the readout avoided!" << endl;}

 //    cout << "11" << endl;

}













void TestTBMROCAB(int q)                // testprogram with trigger bursts and "tbm enable triggers"
{                     


  FIFOreset ();
  int errorburstcounter = 0;
  
  int spam = 0;
  
  bool canreadoutA = false;
  bool canreadoutB = false;
  bool isEmptyA = false;
  bool isEmptyB = false;
  
  int totrocerrorA = 0;
  int totgoodwordA = 0;
  
  int totrocerrorB = 0;
  int totgoodwordB = 0;

  int notrailerA = 0;
  int notrailerB = 0;

  vector<uint32_t> cSpyA;
  vector<uint32_t> cSpyB;
  vector<uint32_t> cSpyA_Type;
  vector<uint32_t> cSpyA_ID;
  vector<uint32_t> cSpyB_Type;
  vector<uint32_t> cSpyB_ID;
  
  for (int p = 0; p < q; p++)        // will go through the data for each burst in the total number of events q
    {                        
      cAmc13Controller.fAmc13Interface->BurstL1A();                   //sends burst
      usleep(sleeptime);
      for (auto& cFED : cSystemController.fPixFEDVector)   // loops over a range of values contained in cSystemController.fPixFEDVector
	{    
	  
	  // read out spyfifo
	  cSpyA.clear();
	  cSpyB.clear();
	  cSpyA_Type.clear();
	  cSpyA_ID.clear();
	  cSpyB_Type.clear();
	  cSpyB_ID.clear();
	  
	  
	  
	  //read in spyFIFO CHANNEL A
	  //------------------------------------------------------------------------------------------------------------------------
	  
	  bool isEmptyA = false;
	  int cnt = 0;
	  bool isEmptyB = false;

	  while((!isEmptyA || !isEmptyB) && !(cnt > 6))
	    {
	      cnt++;
	      cSystemController.fFEDInterface->readSpyFIFO (cFED, cSpyA, cSpyB);
	      uint32_t cMask = 0xf0;
	      
	      isEmptyA = true; 
	      isEmptyB = true;
	      canreadoutA = false;
	      canreadoutB = false;

	      // cout << "\nChannel A" << endl;

	      for (int i=0; i < cSpyA.size(); i++){       // if anything in cSpyA is not zero, we set isEmptyA = false and read out again the spyFIFO 
		if (cSpyA[i] != 0)
		  isEmptyA = false;}
	      for (int i=0; i < cSpyB.size(); i++){       // if anything in cSpyB is not zero, we set isEmptyB = false and read out again the spyFIFO 
		if (cSpyB[i] != 0)
		  isEmptyB = false;}
	      
	      for (int a=0; a < cSpyA.size(); a++){
		if (cSpyA[a]!=0)
		  canreadoutA = true;}
	      
	      for (int a=0; a < cSpyB.size(); a++){
		if (cSpyB[a]!=0)
		  canreadoutB = true;}
	      
	      if (canreadoutA = true)              // write input of cSpyA in cSpyA_Type
		{
		  for (int i=0; i < cSpyA.size(); i++){
		    if ( (cSpyA[i] & 0xff) != 0) {
		      // cout << hex << ((cSpyA[i] >> 4) & 0xff) << " " ;
		      cSpyA_Type.push_back(((cSpyA[i] >> 4) & 0xf));}                       // cSpyA_Type should contain just "8 9 a b 7 7 7 7 7 7 7 7 c d e f ..." so only important information of tbm header/trailer and ROC header
		  }
		}
	      else 
		cout << "cSpyA is empty" << endl;  
	      
	      // cout << "\nChannel B" << endl;

	      if (canreadoutB = true)              // write input of cSpyB in cSpyB_Type
		{             
		  for (int i=0; i < cSpyB.size(); i++){
		    if ( (cSpyB[i] & 0xff) != 0) {
		      //  cout << hex << ((cSpyB[i] >> 4) & 0xf) << " " ;
		      cSpyB_Type.push_back(((cSpyB[i] >> 4) & 0xf));}                        // cSpyB_Type should contain just "8 9 a b 7 7 7 7 7 7 7 7 c d e f ..." so only important information of tbm header/trailer and ROC header
		  }
		}
	      else 
		cout << "cSpyB is empty" << endl; 

	    }
	  
	  /* 
	  for (int a=0; a < cSpyA_Type.size(); a++){
	    cout << hex  << cSpyA_Type[a]  << "\t";}
	  cout << dec << endl;
	  */
	  //read in finished 
	  //---------------------------------------------------------------------------------------------------------
	  
	  
	  if (cnt >= 6){                  //looks, whether there was a infinity loop in read out data
	    canreadoutA = false;
	    canreadoutB = false;
	    spam++;
	    FIFOreset();
	    usleep(1000);       
	    //p--;
	    if (spam > 100) {
	      cout << "spyfifo has been spamed more than a hundred times, burst canceled" << endl;

	      cout << "G" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << "0" << "\t" << 2*q*burst << "\t" << "100" << "\t" << 2*q*burst << "\t" << endl;
  
	      cout << "A" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << "0" << "\t" << q*burst << "\t" << "100" << "\t"  << q*burst << "\t" << endl;
  
	      cout << "B" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << "0" << "\t" << q*burst << "\t" << "100" << "\t" << q*burst << "\t" << endl;
  
	      return;}
	    //continue;
	  }
	  
	  	  
	  // read out channel A & B
	  //------------------------------------------------------------------------------------------------------
	  //-------------------------------------------------------------------------------------------------------------   
	  
	  int size = 0;
	  int iA = 0;
	  int iB = 0;
	  
	  int sizeA = 0;
	  int goodwordA = 0;
	  //int errorA = 0;
	  int rocerrorA = 0;
	  int rocA = 0;
	  int overflowA = 0;
	  
	  int sizeB = 0;
	  int goodwordB = 0;
	  // int errorB = 0;
	  int rocerrorB = 0;
	  int rocB = 0;
	  int overflowB = 0;
	  
	  
	  sizeA = cSpyA_Type.size();
	  sizeB = cSpyB_Type.size();

	  // cout << "sizeA: " << sizeA << "\tsizeB: " << sizeB << endl;
	  
	  if (sizeA > sizeB)
	    size = sizeA;
	  else
	    size = sizeB;
	  
	  while ((iB < size) || (iA < size)){
	    
	    
	    // analize channel A
	    if( ((iA+4) < sizeA) && (cSpyA_Type[iA] == 0x8) && (cSpyA_Type[iA+1] == 0x9) && (cSpyA_Type[iA+2] == 0xa) && (cSpyA_Type[iA+3] == 0xb) ){                    // if we find header
	      iA += 4;                                                                                                                                                     // we look at the things behinde the header
	      overflowA = 0;

	      while ( ((iA+1) < sizeA) && (cSpyA_Type[iA] != 0x8) && (cSpyA_Type[iA] != 0x9) && (cSpyA_Type[iA] != 0xa) && (cSpyA_Type[iA] != 0xb) &&                    // count roc header till we see (part of) tbm header or trailer
		                          (cSpyA_Type[iA] != 0xc) && (cSpyA_Type[iA] != 0xd) && (cSpyA_Type[iA] != 0xe) && (cSpyA_Type[iA] != 0xf) ){
		overflowA++;
		if (cSpyA_Type[iA] == 7)
		  rocA++;
		if (overflowA >=20){
		  overflowA = 0;
		  notrailerA++;
		  //cout << "tbmA header but no trailer in sight" << endl;
		  //FIFOreset();
		  continue;}
		//	cout << "rocA: " << rocA << "\t";
		iA++;
	      }
	      
	      if (rocA == 8){
		goodwordA++;
		rocA = 0;}
	      else {
		//if ((iA+2) < sizeA){
		// cout << "errorNrA: " << rocerrorA << "     " << cSpyA_Type[iA-10] << "  " << cSpyA_Type[iA-9] << "  " << cSpyA_Type[iA-8] << "  " << cSpyA_Type[iA-7] << "  " << cSpyA_Type[iA-6] << "  " << cSpyA_Type[iA-5] << "  " 
		//      << cSpyA_Type[iA-4] << "  " << cSpyA_Type[iA-3] << "  " << cSpyA_Type[iA-2] << "  " << cSpyA_Type[iA-1] << "  " << cSpyA_Type[iA] << "  " << cSpyA_Type[iA+1] << "  " << cSpyA_Type[iA+2] << endl;}
		//else{ 
		// cout << "errorNrA: " << rocerrorA << "  " << cSpyA_Type[iA-10] << "  " << cSpyA_Type[iA-9] << "  " << cSpyA_Type[iA-8] << "  " << cSpyA_Type[iA-7] << "  " << cSpyA_Type[iA-6] << "  " << cSpyA_Type[iA-5] << "  " 
		//      << cSpyA_Type[iA-4] << "  " << cSpyA_Type[iA-3] << "  " << cSpyA_Type[iA-2] << "  " << cSpyA_Type[iA-1] << "  " << cSpyA_Type[iA] << endl;}
		rocerrorA++;
		rocA = 0;}
	      
	      
	      if ( (((iA+4) < sizeA) && cSpyA_Type[iA] == 0xc) && (cSpyA_Type[iA+1] == 0xd) && (cSpyA_Type[iA+2] == 0xe) && (cSpyA_Type[iA+3] == 0xf) )     // if after ROC header tbm trailer: skip this tbm header
		iA += 4;
	    }
	    else {
	      // if (iA <= sizeA)                     // just increase iA to a value which is 1 higher than the size of the vector, to make sure it doesn't go into the if-condition again
		iA++;
	      // errorA++;
	    }
	    // cout << "start channel B" << endl;
	    
	    // analize channel B
	    if( ((iB+4) < sizeB) && (cSpyB_Type[iB] == 0x8) && (cSpyB_Type[iB+1] == 0x9) && (cSpyB_Type[iB+2] == 0xa) && (cSpyB_Type[iB+3] == 0xb) ){                    // if we find header
	      iB += 4;                                                                                                                                                     // we look at the things behinde the header
	      overflowB = 0;

	      while ( ((iB+1) < sizeB) && (cSpyB_Type[iB] != 0x8) && (cSpyB_Type[iB] != 0x9) && (cSpyB_Type[iB] != 0xa) && (cSpyB_Type[iB] != 0xb) &&                    // count roc header till we see (part of) tbm header or trailer
		                          (cSpyB_Type[iB] != 0xc) && (cSpyB_Type[iB] != 0xd) && (cSpyB_Type[iB] != 0xe) && (cSpyB_Type[iB] != 0xf) ){
		overflowB++;
		if (cSpyB_Type[iB] == 7)
		  rocB++;
		if (overflowB >=20){
		  overflowB = 0;
		  notrailerB++;
		  //cout << "tbmB header but no trailer in sight" << endl;
		  //FIFOreset();
		  continue;}
		iB++;
	      }
	      
	      if (rocB == 8){
		goodwordB++;
		rocB = 0;}
	      else {
		//if ((iB+2) < sizeB){
		// cout << "\nerrorNrB: " << rocerrorB << "     " << cSpyB_Type[iB-10] << "  " << cSpyB_Type[iB-9] << "  " << cSpyB_Type[iB-8] << "  " << cSpyB_Type[iB-7] << "  " << cSpyB_Type[iB-6] << "  " << cSpyB_Type[iB-5] << "  " 
		//      << cSpyB_Type[iB-4] << "  " << cSpyB_Type[iB-3] << "  " << cSpyB_Type[iB-2] << "  " << cSpyB_Type[iB-1] << "  " << cSpyB_Type[iB] << "  " << cSpyB_Type[iB+1] << "  " << cSpyB_Type[iB+2] << endl;}
		//else{
		//  cout << "\nerrorNrB: " << rocerrorB << "  " << cSpyB_Type[iB-10] << "  " << cSpyB_Type[iB-9] << "  " << cSpyB_Type[iB-8] << "  " << cSpyB_Type[iB-7] << "  " << cSpyB_Type[iB-6] << "  " << cSpyB_Type[iB-5] << "  " 
		//       << cSpyB_Type[iB-4] << "  " << cSpyB_Type[iB-3] << "  " << cSpyB_Type[iB-2] << "  " << cSpyB_Type[iB-1] << "  " << cSpyB_Type[iB] << endl;}
		rocerrorB++;
		rocB = 0;}
	      
	      
	      if ( (((iB+4) < sizeB) && cSpyB_Type[iB] == 0xc) && (cSpyB_Type[iB+1] == 0xd) && (cSpyB_Type[iB+2] == 0xe) && (cSpyB_Type[iB+3] == 0xf) )     // if after ROC header tbm trailer: skip this tbm header
		iB += 4;
	    }
	    else {
	      // if (iB <= sizeB)	      
		iB++;
	      // errorB++;
	    }

	  }
	  
	  
	  //count up errors 
	  if (rocerrorA > 100){
	    FIFOreset();
	    goodwordA = 0;
	    rocerrorA = 100;}
	  if (rocerrorB > 100){
	    FIFOreset();
	    goodwordB = 0;
	    rocerrorB = 100;}
	  if (goodwordA == 100)
	    rocerrorA = 0;
	  if (goodwordB == 100)
	    rocerrorB = 0;

	  /* if ((goodwordA < 50)){
	    FIFOreset();
	    cout << "cSpyA:" << endl;
	    for (int i; i < cSpyA.size(); i++)
	      cout << cSpyA[i] << "  ";
	    cout << endl;
	    cout << "cSpyA_Type:" << endl;
	    for (int i; i < cSpyA_Type.size(); i++)
	      cout << cSpyA_Type[i] << "  ";
	    cout << endl; }

	  if ((goodwordB < 50)){
	    FIFOreset();
	    cout << "cSpyB:" << endl;
	    for (int i; i < cSpyB.size(); i++)
	      cout << cSpyB[i] << "  ";
	    cout << endl;
	    cout << "cSpyB_Type:" << endl;
	    for (int i; i < cSpyB_Type.size(); i++)
	      cout << cSpyB_Type[i] << "  ";
	    cout << endl; }
	  */
	  //toterrorA += errorA;
	  totgoodwordA += goodwordA;
	  totrocerrorA += rocerrorA;
	  
	  //toterrorB += errorB;
	  totgoodwordB += goodwordB;
	  totrocerrorB += rocerrorB;
	  
	}
    } 
  
  //print out
  //cout << "Frequency" << "\t" << "good words" << "\t" << "tot error" << "\t" << "error in %" << "\t" << "bad or missed events (tbm header/trailer not correct)"  << endl;
  cout << "G" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << (totgoodwordA + totgoodwordB) << "\t" << (totrocerrorA + totrocerrorB) << "\t" 
       << ((float)(((float) (2*q*burst -(totgoodwordA + totgoodwordB))/(2*q*burst)))*100) << "\t" << (2*q*burst - (totgoodwordA + totgoodwordB)) << "\t" << endl;
  
  cout << "A" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodwordA << "\t" << totrocerrorA << "\t" << ((float)(((float) (q*burst - totgoodwordA)/(q*burst)))*100) << "\t" 
       << (q*burst - totgoodwordA) << "\t" << endl;
  
  cout << "B" << "\t" << dec << ((float)4/(float)rate*10000000) << "\t" << totgoodwordB << "\t" << totrocerrorB << "\t" << ((float)(((float) (q*burst - totgoodwordB)/(q*burst)))*100) << "\t" 
       << (q*burst - totgoodwordB) << "\t" << endl;
  
  if (spam != 0){
    cout << dec << "cSpy was " << spam << " spamed and the readout avoided!" << endl;
    FIFOreset();} 
  if (notrailerA != 0){
    cout << dec << "Channel A had " << notrailerA << " times no trailer -> readout avoided!" << endl;
    notrailerA = 0;
    FIFOreset();}
  if (notrailerB != 0){
    cout << dec << "Channel B had " << notrailerB << " times no trailer -> readout avoided!" << endl;
    notrailerB = 0;
    FIFOreset();}
}












































void BgoTrigReadspyFifo(){
  SendBGO();
  SendTrigBurst();
  ReadSpyFifo();
}










void TestTBMROCcalInject(int q)                // testprogram with trigger bursts and "tbm enable triggers"
{                     


  FIFOreset ();
  int errorburstcounter = 0;
  
  int spam = 0;
  
  bool canreadoutA = false;
  bool canreadoutB = false;
  bool isEmptyA = false;
  bool isEmptyB = false;
  
  int totrocerrorA = 0;
  int totgoodwordA = 0;
  
  int totrocerrorB = 0;
  int totgoodwordB = 0;

  int notrailerA = 0;
  int notrailerB = 0;

  vector<uint32_t> cSpyA;
  vector<uint32_t> cSpyB;
  vector<uint32_t> cSpyA_Type;
  vector<uint32_t> cSpyA_ID;
  vector<uint32_t> cSpyB_Type;
  vector<uint32_t> cSpyB_ID;

  vector<uint32_t> ColAddrA;
  vector<uint32_t> RowAddrA;
  vector<uint32_t> PixelHitA;
  vector<uint32_t> RocNrA;
  vector<uint32_t> ColAddrB;
  vector<uint32_t> RowAddrB;
  vector<uint32_t> PixelHitB;
  vector<uint32_t> RocNrB;

  for (int p = 0; p < q; p++)        // will go through the data for each burst in the total number of events q
    {                        
      cAmc13Controller.fAmc13Interface->BurstL1A();                   //sends burst
      usleep(sleeptime);
      for (auto& cFED : cSystemController.fPixFEDVector)   // loops over a range of values contained in cSystemController.fPixFEDVector
	{    
	  
	  // read out spyfifo
	  cSpyA.clear();
	  cSpyB.clear();
	  cSpyA_Type.clear();
	  cSpyA_ID.clear();
	  cSpyB_Type.clear();
	  cSpyB_ID.clear();
	  
	  
	  
	  //read in spyFIFO CHANNEL A
	  //------------------------------------------------------------------------------------------------------------------------
	  
	  bool isEmptyA = false;
	  int cnt = 0;
	  bool isEmptyB = false;
	  
	  while((!isEmptyA || !isEmptyB) && !(cnt > 6))
	    {
	      cnt++;
	      cSystemController.fFEDInterface->readSpyFIFO (cFED, cSpyA, cSpyB);
	      uint32_t cMask = 0xf0;
	      
	      isEmptyA = true;
	      isEmptyB = true;
	      
	      for (int i=0; i < cSpyA.size(); i++){       // if anything in cSpyA is not zero, we set isEmptyA = false and read out again the spyFIFO 
		if (cSpyA[i] != 0)
		  isEmptyA = false;}
	      for (int i=0; i < cSpyB.size(); i++){       // if anything in cSpyB is not zero, we set isEmptyB = false and read out again the spyFIFO 
		if (cSpyB[i] != 0)
		  isEmptyB = false;}
	      
	      for (int a=0; a < cSpyA.size(); a++){
		if (cSpyA[a]!=0)
		  canreadoutA = true;}
	      
	      for (int a=0; a < cSpyB.size(); a++){
		if (cSpyB[a]!=0)
		  canreadoutB = true;}
	      
	      if (canreadoutA = true)              // write input of cSpyA in cSpyA_Type
		{
		  for (int i=0; i < cSpyA.size(); i++){
		    if ( (cSpyA[i] & 0xff) != 0) {
		      	cout << hex << ((cSpyA[i] >> 4) & 0xff) << " " ;
		      cSpyA_Type.push_back(((cSpyA[i] >> 4) & 0xff));}                       // cSpyA_Type should contain just "8 9 a b 7 7 7 7 7 7 7 7 c d e f ..." so only important information of tbm header/trailer and ROC header
		  }
		}
	      else 
		cout << "cSpyA is empty" << endl;  
	      
	      
	      if (canreadoutB = true)              // write input of cSpyB in cSpyB_Type
		{             
		  for (int i=0; i < cSpyB.size(); i++){
		    if ( (cSpyB[i] & 0xff) != 0) {
		      cout << hex << ((cSpyB[i] >> 4) & 0xff) << " " ;
		      cSpyB_Type.push_back(((cSpyB[i] >> 4) & 0xff));}                        // cSpyB_Type should contain just "8 9 a b 7 7 7 7 7 7 7 7 c d e f ..." so only important information of tbm header/trailer and ROC header
		  }
		}
	      else 
		cout << "cSpyB is empty" << endl; 
	    }
	  
	  
	  for (int a=0; a < cSpyA_Type.size(); a++){
	    cout << hex  << cSpyA_Type[a]  << "\t";}
	  //cout << dec << endl;
	  
	  //read in finished 
	  //---------------------------------------------------------------------------------------------------------
	  

	  if (cnt >= 7){                  //looks, whether there was a infinity loop in read out data
	    canreadoutA = false;
	    canreadoutB = false;
	    spam++;
	    FIFOreset();
	    usleep(1000);       
	    p--;
	    if (spam > 100) {
	      cout << "spyfifo has been spamed more than a hundred times, burst canceled" << endl;
	      return;}
	    continue;}
	  
	  	  
	  // read out channel A & B
	  //------------------------------------------------------------------------------------------------------
	  //-------------------------------------------------------------------------------------------------------------   
	  
	  int size = 0;
	  int iA = 0;
	  int iB = 0;
	  
	  int sizeA = 0;
	  int goodwordA = 0;
	  //int errorA = 0;
	  int rocerrorA = 0;
	  int rocA = 0;
	  int overflowA = 0;
	  
	  int sizeB = 0;
	  int goodwordB = 0;
	  // int errorB = 0;
	  int rocerrorB = 0;
	  int rocB = 0;
	  int overflowB = 0;
	  bool rocheaderA = false;
	  bool rocheaderB = false;

	  int colAddrA = 0;
	  int rowAddrA = 0;
	  int pixelHitA = 0;
	  int colAddrB = 0;
	  int rowAddrB = 0;
	  int pixelHitB = 0;	  
	  
	  sizeA = cSpyA_Type.size();
	  sizeB = cSpyB_Type.size();
	  
	  if (sizeA > sizeB)
	    size = sizeA;
	  else
	    size = sizeB;

	  // while ((iB < size) || (iA < size)){ ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  while (iA < size){
	    cout << "1" << endl;
	    
	    // analize channel A
	    if(( ((iA+4) < sizeA) && (cSpyA_Type[iA]& 0xf0) == 0x8) && ((cSpyA_Type[iA+1]& 0xf0) == 0x9) && ((cSpyA_Type[iA+2]& 0xf0) == 0xa) && ((cSpyA_Type[iA+3]& 0xf0) == 0xb) ){        // if we find a tbm header
	      iA += 4;                                                                                                                                                     // we look at the things behinde the header
	      overflowA = 0;
	      //rocheaderA = false;
	    cout << "2" << endl;
	      while (((iA+1) < sizeA) && ((cSpyA_Type[iA]& 0xf0) != 0x8) && ((cSpyA_Type[iA]& 0xf0) != 0x9) && ((cSpyA_Type[iA]& 0xf0) != 0xa) && ((cSpyA_Type[iA]& 0xf0) != 0xb) &&    // till we see (part of) tbm header or trailer
		                         ((cSpyA_Type[iA]& 0xf0) != 0xc) && ((cSpyA_Type[iA]& 0xf0) != 0xd) && ((cSpyA_Type[iA]& 0xf0) != 0xe) && ((cSpyA_Type[iA]& 0xf0) != 0xf)){
		overflowA++;

		colAddrA = 0;
		rowAddrA = 0;
		pixelHitA = 0;

		if ((cSpyA_Type[iA]& 0xf0) == 7){
		  //rocheaderA = true;
		  overflowA = 0;
		  rocA++;                                                                  // count roc header till we see (part of) tbm header or trailer
		  while (((iA+1) < sizeA) && ((cSpyA_Type[iA]& 0xf0) != 0x7) && ((cSpyA_Type[iA]& 0xf0) != 0x8) && ((cSpyA_Type[iA]& 0xf0) != 0x9) && ((cSpyA_Type[iA]& 0xf0) != 0xa) &&  // till we see (part of) tbm header or trailer
		                     	     ((cSpyA_Type[iA]& 0xf0) != 0xb) && ((cSpyA_Type[iA]& 0xf0) != 0xc) && ((cSpyA_Type[iA]& 0xf0) != 0xd) && ((cSpyA_Type[iA]& 0xf0) != 0xe) && ((cSpyA_Type[iA]& 0xf0) != 0xf) ){

		    if (((iA+6) < sizeA) && (rocheaderA == true) && ((cSpyA_Type[iA+1]& 0xf0) == 1) && ((cSpyA_Type[iA+2]& 0xf0) == 2) && ((cSpyA_Type[iA+3]& 0xf0) == 3) && 
		                                                   ((cSpyA_Type[iA+4]& 0xf0) == 4) && ((cSpyA_Type[iA+5]& 0xf0) == 5) && ((cSpyA_Type[iA+6]& 0xf0) == 6) ) {
		      //cout << Col Addr << Row Addr << Pixel Hit << endl;
		      colAddrA = ((cSpyA_Type[iA+1] & 0xf) << 2) + ((cSpyA_Type[iA+2]& 0xc) >> 2);
		      rowAddrA = ((cSpyA_Type[iA+2]& 0x3) << 7) + ((cSpyA_Type[iA+3]& 0xf) << 3) + ((cSpyA_Type[iA+4]& 0xe) >> 1);
		      pixelHitA = ((cSpyA_Type[iA+4]& 0x1) << 7) + ((cSpyA_Type[iA+5]& 0xe) << 3) + ((cSpyA_Type[iA+6]& 0xf));

		      RocNrA.push_back(rocA);
		      ColAddrA.push_back(colAddrA);
		      RowAddrA.push_back(rowAddrA);
		      PixelHitA.push_back(pixelHitA);
	    cout << "3" << endl;
		      colAddrA = 0;
		      rowAddrA = 0;
		      pixelHitA = 0;

		      iA += 6;
		      overflowA = 0;
		      // rocheaderA = false;
		    }
		    else
		      iA++;

	      }
		  if (overflowA >=20){      // has to be somewhere else, but not here
		    overflowA = 0;
		    notrailerA++;
		    rocheaderA = false;
		    //cout << "tbmA header but no trailer in sight" << endl;
		    //FIFOreset();
		    continue;}
		  //	cout << "rocA: " << rocA << "\t";
		  //iA++;
		}
		iA++;  // gives us a problem, because if header/trailer is found it arises nevertheless by one
		
		if (((cSpyA_Type[iA]& 0xf0) == 0x7) || ((cSpyA_Type[iA]& 0xf0) == 0x8) || ((cSpyA_Type[iA]& 0xf0) == 0x9) || ((cSpyA_Type[iA]& 0xf0) == 0xa) ||  // till we see (part of) tbm header or trailer
		    ((cSpyA_Type[iA]& 0xf0) == 0xb) || ((cSpyA_Type[iA]& 0xf0) == 0xc) || ((cSpyA_Type[iA]& 0xf0) == 0xd) || ((cSpyA_Type[iA]& 0xf0) == 0xe) || ((cSpyA_Type[iA]& 0xf0) == 0xf) ){
		  iA--;
		}
		
		
	      }

	     
	      if (rocA == 8){
		goodwordA++;
		for (int i = 0; i < ColAddrA.size(); i++){
		  cout << dec << RocNrA[i] << "\t" << ColAddrA[i] << "\t" << RowAddrA[i] << "\t" << PixelHitA[i] << endl;}
		RocNrA.clear();
		ColAddrA.clear();
		RowAddrA.clear();
		PixelHitA.clear();
		rocA = 0;}
	      else {
		cout << "not good" << endl;
		//if (iA < (sizeA + 2)){
		//  cout << "errorNrA: " << rocerrorA << "  " << cSpyA_Type[iA-10] << "  " << cSpyA_Type[iA-9] << "  " << cSpyA_Type[iA-8] << "  " << cSpyA_Type[iA-7] << "  " << cSpyA_Type[iA-6] << "  " << cSpyA_Type[iA-5] << "  " 
		//       << cSpyA_Type[iA-4] << "  " << cSpyA_Type[iA-3] << "  " << cSpyA_Type[iA-2] << "  " << cSpyA_Type[iA-1] << "  " << cSpyA_Type[iA] << "  " << cSpyA_Type[iA+1] << "  " << cSpyA_Type[iA+2] << endl;}
		//else{ 
		//  cout << "errorNrA: " << rocerrorA << "  " << cSpyA_Type[iA-10] << "  " << cSpyA_Type[iA-9] << "  " << cSpyA_Type[iA-8] << "  " << cSpyA_Type[iA-7] << "  " << cSpyA_Type[iA-6] << "  " << cSpyA_Type[iA-5] << "  " 
		//       << cSpyA_Type[iA-4] << "  " << cSpyA_Type[iA-3] << "  " << cSpyA_Type[iA-2] << "  " << cSpyA_Type[iA-1] << "  " << cSpyA_Type[iA] << endl;}
		RocNrA.clear();
		ColAddrA.clear();
		RowAddrA.clear();
		PixelHitA.clear();
		rocerrorA++;
		rocA = 0;}
	  
	      if ( (iA < (sizeA + 4)) && (cSpyA_Type[iA] == 0xc) && (cSpyA_Type[iA+1] == 0xd) && (cSpyA_Type[iA+2] == 0xe) && (cSpyA_Type[iA+3] == 0xf) )     // if after ROC header tbm trailer: skip this tbm header
		iA += 4;
	    }

	    else {
	      // errorA++;
	      iA++; }

	  }


          
	  //count up errors 
	  if (rocerrorA > 100){
	    goodwordA = 0;
	    rocerrorA = 100;}
	  if (rocerrorB > 100){
	    goodwordB = 0;
	    rocerrorB = 100;}
	  if (goodwordA == 100)
	    rocerrorA = 0;
	  if (goodwordB == 100)
	    rocerrorB = 0;
	  
	  //toterrorA += errorA;
	  totgoodwordA += goodwordA;
	  totrocerrorA += rocerrorA;
	  
	  //toterrorB += errorB;
	  totgoodwordB += goodwordB;
	  totrocerrorB += rocerrorB;
	  
	}
    } 

}






	  






























///*





void Testprogram(int q) {                 // testprogram with trigger burst 8 and roc trigger enabled

int n = 0;
int j = 0;
int l = 0;
int r = 8;
 for (int p = 1; p <= q; p= p+r){   

   cAmc13Controller.fAmc13Interface->BurstL1A();

   for (auto& cFED : cSystemController.fPixFEDVector) {     // loops over a range of values contained in cSystemController.fPixFEDVector
     vector<uint32_t> cFifo1A;
     vector<uint32_t> cFifo1B;
     vector<uint32_t> cMarkerA;
     vector<uint32_t> cMarkerB;
     cSystemController.fFEDInterface->readFIFO1 (cFED,cFifo1A,cFifo1B,cMarkerA, cMarkerB);
     for (int i=0; i < cFifo1A.size(); i++)
       cout << (cFifo1A.at(i)) << "     " << cMarkerA.at(i)<< "      "  << (cFifo1B.at(i)) << "     " << cMarkerB.at(i)   << endl;
     //cout << (cFifo1A.at(i)& 0xff) << "     " << cMarkerA.at(i)<< "      "  << (cFifo1B.at(i)& 0xff) << "     " << cMarkerB.at(i)   << endl;


     for (int h=0; h<=7; h++){

       n++; 

       if (!(cMarkerA.at(h*8+0)==8 && cMarkerA.at(h*8+1)==12 && cMarkerA.at(h*8+2)==12 && cMarkerA.at(h*8+3)==12 && cMarkerA.at(h*8+4)==12 && cMarkerA.at(h*8+5)==12 &&
            cMarkerA.at(h*8+6)==12 &&  cMarkerA.at(h*8+7)==12 && cMarkerA.at(h*8+8)==12 &&  cMarkerA.at(h*8+9)==4)){
	 j++;
	 cout << "found " << j << " Header/Trailer errors in Channel A in " << n << " of " << q << " cycles" << endl;
       }

       if (!(cMarkerB.at(h*8+0)==8 && cMarkerB.at(h*8+1)==12 && cMarkerB.at(h*8+2)==12 && cMarkerB.at(h*8+3)==12 && cMarkerB.at(h*8+4)==12 && cMarkerB.at(h*8+5)==12 &&
            cMarkerB.at(h*8+6)==12 &&  cMarkerB.at(h*8+7)==12 && cMarkerB.at(h*8+8)==12 &&  cMarkerB.at(h*8+9)==4)){
	 j++;
	 cout << "found " << j << " Header/Trailer errors in Channel B in " << n << " of " << q << " cycles" << endl;
       } 

       if (!(((cFifo1A.at(h*8+1) >> 21)& 0x1f)==1 && ((cFifo1A.at(h*8+2) >> 21)& 0x1f)==2 && ((cFifo1A.at(h*8+3) >> 21)& 0x1f)==3 && ((cFifo1A.at(h*8+4) >> 21)& 0x1f)==4 &&
          ((cFifo1A.at(h*8+5) >> 21)& 0x1f)==5 && ((cFifo1A.at(h*8+6) >> 21)& 0x1f)==6 && ((cFifo1A.at(h*8+7) >> 21)& 0x1f)==7 && ((cFifo1A.at(h*8+8) >> 21)& 0x1f)==8)){
        l++;
        cout << "found " << l << " ROC numeration errors in Channel A in " << n << " of " << q << " cycles" << endl;
       }
       if (!(((cFifo1B.at(h*8+1) >> 21)& 0x1f)==1 && ((cFifo1B.at(h*8+2) >> 21)& 0x1f)==2 && ((cFifo1B.at(h*8+3) >> 21)& 0x1f)==3 && ((cFifo1B.at(h*8+4) >> 21)& 0x1f)==4 &&
          ((cFifo1B.at(h*8+5) >> 21)& 0x1f)==5 && ((cFifo1B.at(h*8+6) >> 21)& 0x1f)==6 && ((cFifo1B.at(h*8+7) >> 21)& 0x1f)==7 && ((cFifo1B.at(h*8+8) >> 21)& 0x1f)==8)){
        l++;
        cout << "found " << l << " ROC numeration errors in Channel B in " << n << " of " << q << " cycles" << endl;
       }


    }



    vector<uint32_t> cSpyA;
     vector<uint32_t> cSpyB;
     cSystemController.fFEDInterface->readSpyFIFO (cFED, cSpyA, cSpyB);
     uint32_t cMask = 0xf0;

     for (auto& cWord : cSpyA ){
       
        if (cWord != 0)
        {
            if ( (cWord & 0xff) != 0) std::cout << std::hex << (cWord & 0xff) << " " ;

            if ( ( (cWord & cMask) >> 4) == 11 ) std::cout << "       " << ( (cWord >> 20) & 0xfff) << std::endl;

            if ( ( (cWord & cMask) >> 4) ==  6 ) std::cout << "       " << ( (cWord >> 20) & 0xfff) << std::endl;

            if ( ( (cWord & cMask) >> 4) ==  7 ) std::cout << "       " << ( (cWord >> 20) & 0xfff) << std::endl;

	    if ( ( (cWord & cMask) >> 4) == 15 ) std::cout << "       " << ( (cWord >> 20) & 0xfff) << std::endl;
        }

    }





   }
   if (p % 500 == 0)
     cout << p << " out of " << q << " cycles" << endl; 
 } 

 cout << "We found " << n-j-l << "out of " << q << " correct transmitted data streams" << endl;
 cout << "we found " << j << " header/trailer errors" << endl;
 cout << "we found " << l << " ROC numeration errors" << endl;
}

//*/







void Testprogram(int q, int r) {

int n = 0;
int j = 0;
int l = 0;
 for (int p = 1; p <= q; p = p+r){   
   //   usleep(10000);
   //cAmc13Controller.fAmc13Interface->BurstL1A(r);
   cAmc13Controller.fAmc13Interface->BurstL1A();
   //usleep(1);
   // usleep(r);
   for (auto& cFED : cSystemController.fPixFEDVector) {     // loops over a range of values contained in cSystemController.fPixFEDVector
     vector<uint32_t> cFifo1A;
     vector<uint32_t> cFifo1B;
     vector<uint32_t> cMarkerA;
     vector<uint32_t> cMarkerB;
     cSystemController.fFEDInterface->readFIFO1 (cFED,cFifo1A,cFifo1B,cMarkerA, cMarkerB);
     // for (int i=0; i < cFifo1A.size(); i++)
     //  cout << (cFifo1A.at(i)& 0xff) << "     " << cMarkerA.at(i)<< "      "  << (cFifo1B.at(i)& 0xff) << "     " << cMarkerB.at(i)   << endl;
     if (cMarkerA.at(0)==8 && cMarkerA.at(1)==12 && cMarkerA.at(2)==12 && cMarkerA.at(3)==12 && cMarkerA.at(4)==12 && cMarkerA.at(5)==12 && cMarkerA.at(6)==12 &&
	cMarkerA.at(7)==12 && cMarkerA.at(8)==12 &&  cMarkerA.at(9)==4){
       n++;
     }
     else{
       j++;
       cout << "found " << j << " Header/Trailer errors in " << n << " of " << q << " cycles" << endl;
     }
     if (!(((cFifo1A.at(1) >> 21)& 0x1f)==1 && ((cFifo1A.at(2) >> 21)& 0x1f)==2 && ((cFifo1A.at(3) >> 21)& 0x1f)==3 && ((cFifo1A.at(4) >> 21)& 0x1f)==4 &&
	   ((cFifo1A.at(5) >> 21)& 0x1f)==5 && ((cFifo1A.at(6) >> 21)& 0x1f)==6 && ((cFifo1A.at(7) >> 21)& 0x1f)==7 && ((cFifo1A.at(8) >> 21)& 0x1f)==8)){
       l++;
       cout << "found " << l << " ROC numeration errors in " << n << " of " << q << " cycles" << endl;
     }

   }
   if (p % 500 == 0)
     cout << p << " out of " << q << " cycles" << endl; 
 }
 cout << "We found " << n << " correct transmitted data streams" << endl;
 cout << "we found " << j << " header/trailer errors" << endl;
 cout << "we found " << l << " ROC numeration errors" << endl;
}








void ReadErrorFifo() {
  for (auto& cFED : cSystemController.fPixFEDVector){
    cSystemController.fFEDInterface->readErrorFIFO (cFED, true);
    cSystemController.fFEDInterface->WriteBoardReg (cFED, "fe_ctrl_regs.decode_reg_reset", 1);
  }
}

void ReadData() {
  for (auto& cFED : cSystemController.fPixFEDVector)      
    cSystemController.fFEDInterface->ReadData (cFED, 0 );
}

void SendEC0() {
  cAmc13Controller.fAmc13Interface->SendEC0();
  cout << "EC0 is sent" << endl;
}

void SetBGO(int bgoform, int repeat, int prescale, int BX) {
  cAmc13Controller.fAmc13->addBGO(bgoform, repeat, prescale, BX);
  if (repeat == 0)
    cout << "BGO is set on the form: " << bgoform << endl << "it will be sent just one BGO" << endl << "BGO has a prescale of: " << prescale << endl << "and a BX of: " << BX << endl;
  if (repeat == 1)
    cout << "BGO is set on the form: " << bgoform << endl << "BGO will send continuously" << endl << "BGO has a prescale of: " << prescale << endl << "and a BX of: " << BX << endl;
}

void SendBGO() {
  cAmc13Controller.fAmc13Interface->SendBGO();
  cout << "BGO is sent" << endl;
}

void SendNBGO(int N) {
  for (int i = 1; i <= N; i++)
    SendBGO();
  cout << N << " BGOs were sent" <<endl;
}

void EnableBGO() {
  for (int i = 0; i<=3; i++){
    cAmc13Controller.fAmc13Interface->EnableBGO(i);
    cout << "Enabled BGO on channel " << i << endl;
  }
}

void EnableBGOi(int i) {
  cAmc13Controller.fAmc13Interface->EnableBGO(i);
  cout << "Enabled BGO on channel " << i << endl;
}

void DisableBGO() {
  for (int i = 0; i<=3; i++){
    cAmc13Controller.fAmc13Interface->DisableBGO(i);
    cout << "Disabled BGO on channel " << i << endl;
  }
}

void DisableBGOj(int j) {
  cAmc13Controller.fAmc13Interface->DisableBGO(j);
  cout << "Disabled BGO on channel " << j << endl;
}

void getTrigger () {
  cout <<  cAmc13Controller.fAmc13->getTrigger() << endl;
}

void getBGO () {
  for (int k = 0; k <= 3; k++)
    cout << "For channel " << k << " BGO is: " << cAmc13Controller.fAmc13->getBGO(k) << endl;
}




void exec(SimpleCommand* c){

  int value=0;
  int slot=0;
  int N=1;
  int n=1;
  int m=1;
  int i=0;
  int j=0;
  int bgoform = 0x2c;
  int repeat = 1;
  int prescale = 1;
  int BX = 374;
  float f = 0;


  //int fibersel=0;

  if(c->Keyword("fed", slot)){
    fedSlot=slot;
    cout << "fed " << fedSlot << endl;
  }  
 
  else if(c->Keyword("starttrig")){
    StartContTrig();
  }else if(c->Keyword("stoptrig")){
    StopContTrig();
  }else if(c->Keyword("trigger")){
    SendTrigBurst();
  }else if(c->Keyword("trigger", n)){
    SendNTrigBurst(n);
  }else if(c->Keyword("spyfifo")){
    ReadSpyFifo();
  }else if(c->Keyword("fifo1")){
    ReadFifo1();
  }else if(c->Keyword("errorfifo")){
    ReadErrorFifo();
  }else if(c->Keyword("data")){
    ReadData();
  }else if(c->Keyword("ec0")){
    SendEC0();
  }else if(c->Keyword("bgo")){
    SendBGO();
  }else if(c->Keyword("bgo", N)){
    SendNBGO(N);
  }else if(c->Keyword("enablebgo")){
    EnableBGO();
  }else if(c->Keyword("enablebgo", i)){
    EnableBGOi(i);
  }else if(c->Keyword("disablebgo")){
    DisableBGO();
  }else if(c->Keyword("disablebgo", j)){
    DisableBGOj(j);
  }/*else if(c->Keyword("setbgo", bgoform, repeat, prescale, BX)) {
    SetBGO(bgoform, repeat, prescale, BX);
    }*/else if(c->Keyword("gettrig")) {
    getTrigger();
  }else if(c->Keyword("getbgo")) {
    getBGO();
  }else if(c->Keyword("testprogram", n)) {
    Testprogram(n); 
  }else if(c->Keyword("test", n)) {
    TestTBMA(n);
  }else if(c->Keyword("testprogram", n, m)) {
    Testprogram(n, m);
  }else if(c->Keyword("mode", n)) {
    setAMC13Mode(n);
  }else if(c->Keyword("rate", n)) {
    setAMC13Rate(n);
  }else if(c->Keyword("burst", n)) {
    setAMC13Burst(n);
  }else if(c->Keyword("confamc")) {
    ConfigureAMC13();
  }else if(c->Keyword("title", n)) {
    Title(n);
  }else if(c->Keyword("round", f)) {
    Round(f);
  }else if(c->Keyword("titlegb", n)) {
    TitleGainBias(n);
  }else if(c->Keyword("titlepll", n)) {
    TitlePLL(n);
  }else if(c->Keyword("testab", n)) {
    // cout << "aufrufen der Funktion" << endl;
    TestTBMAB(n);
  }else if(c->Keyword("fiforeset")) {
    FIFOreset();
  }else if(c->Keyword("findphases")) {
    FindPhases();
  }else if(c->Keyword("start")) {
    Start();
  }else if(c->Keyword("stop")) {
    Stop();
  }else if(c->Keyword("pause")) {
    Pause();
  }else if(c->Keyword("resume")) {
    Resume();
  }else if(c->Keyword("testroc", n)) {
    TestTBMROCAB(n);
  }else if(c->Keyword("titleroc", n)) {
    TitleROCAB(n);
  }else if(c->Keyword("bgoplus")) {
   BgoTrigReadspyFifo();
  }else if(c->Keyword("testcal", n)) {
    TestTBMROCcalInject(n);
  }




  else{
    cout << "what??  <" << *c << ">" << endl;
  }

  
}
