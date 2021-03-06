#include "HistoManager.h"

ClassImp(HistoManager)

HistoManager::HistoManager(const char* name, const char* title,Configuration *config) {
  fConfig = config;
  fSatCount = 0;
  fNRowsX = config->GetNRowsX();
  fNRowsY = config->GetNRowsY();
  fNTimeBins = config->GetNTimeBins();
  fNChannels = config->GetNChannels();
  fTimeBinSize = config->GetTimeBinSize();
  fNDevices = config->GetNDevices();
  fPitchX = config->GetXPitch();
  fDAQSystem = fConfig->GetDAQSystem();
  fDevNum = config->GetDevNum();
}

//////////////////////////////////////////////////////

HistoManager::~HistoManager() {
  delete fRawSamplesXY;
  delete fRawSamplesST;
if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {  
  for (int i=0; i < fNDevices; ++i) {
    delete fRawSamplesVec[i];
  }
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    delete fSamplesVec[i];
  }
 }
 else {
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    delete fRawSamplesVec[i];
    delete fSamplesVec[i];
  }
 }
  delete fSamplesXT;
  delete fSamplesYT;
  delete fHitMap;
  delete fChargeMapInt;
  delete fChargeMapAvg;
  delete fStripOccupancyX;
  delete fStripOccupancyY;
  delete fStripOccupancyS;
  delete fTimeDist;
  delete fChargeMap;
}

//////////////////////////////////////////////////////
void HistoManager::Reset() {
  fRawSamplesXY->Reset();
  fRawSamplesST->Reset();
if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {    
  for (int i=0; i < fNDevices; ++i) {
    fRawSamplesVec[i]->Reset();
  }
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    fSamplesVec[i]->Reset();
  }
 }
 else {
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    fSamplesVec[i]->Reset();
    fRawSamplesVec[i]->Reset();
    cout << " reset " << i << endl;
  }
 }
  fSamplesXT->Reset();
  fSamplesYT->Reset();
  fSamplesST->Reset();
  fHitMap->Reset();
  fChargeMapInt->Reset();
  fChargeMapAvg->Reset();
  fStripOccupancyX->Reset();
  fStripOccupancyY->Reset();
  fStripOccupancyS->Reset();
  fTimeDist->Reset();
  fChargeMap->Reset();
}

////////////////////////////////////////////////////////

void HistoManager::InitRawDataHisto() {
  char tBSize[50];
  sprintf(tBSize, "Time bin (%d ps)", fTimeBinSize);
  fRawSamplesXY = new TH2F("RawSamplesXY", "Raw Data XY", fNRowsX, 0, fNRowsX, fNRowsY, 0, fNRowsY);
  fRawSamplesXY->SetXTitle("X Row");
  fRawSamplesXY->SetYTitle("Y Row");
  fRawSamplesXY->SetZTitle("ADC Count");
  fRawSamplesXY->GetXaxis()->SetTitleOffset(1.4);
  fRawSamplesXY->GetYaxis()->SetTitleOffset(1.4);

  fRawSamplesST = new TH2F("RawSamplesST", "Raw Data ST", fNRowsX*fNRowsY, 0, fNRowsX*fNRowsY, fNTimeBins, 0, fNTimeBins);
  fRawSamplesST->SetXTitle("Strip");
  fRawSamplesST->SetYTitle(tBSize);
  fRawSamplesST->SetZTitle("ADC Count");
  fRawSamplesST->GetXaxis()->SetTitleOffset(1.4);
  fRawSamplesST->GetYaxis()->SetTitleOffset(1.4);


if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {  
  for (int i=0; i < fNDevices; ++i) {
    char t[100];
    sprintf(t, "RawSamples%d",i);
    TH1F *rawSamples = new TH1F(t, t,4500 , -0.5, 4499.5);
    fRawSamplesVec.push_back(rawSamples);
  }
 }
 else {
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    char t[100];
    sprintf(t, "RawSamples%d",i);
    TH1F *rawSamples = new TH1F(t, t,1024 , -0.5, 1023.5);
    fRawSamplesVec.push_back(rawSamples);
  }

 }
}

//////////////////////////////////////////////////////

void HistoManager::InitMonitoringHistos() {
  // Single Event 2d: X vs. T
  fSamplesXT = new TH2F("SamplesXT", "Single Event XT", fNRowsX, 0, fNRowsX, fNTimeBins, 0, fNTimeBins);
  fSamplesXT->SetXTitle("X Row");
  char tBSize[50];
  sprintf(tBSize, "Time bin (%d ns)", fTimeBinSize);
  fSamplesXT->SetYTitle(tBSize);
  fSamplesXT->SetZTitle("ADC Count");
  fSamplesXT->GetXaxis()->SetTitleOffset(1.4);
  fSamplesXT->GetYaxis()->SetTitleOffset(1.4);

  // Single Event 2d: Y vs. T
  fSamplesYT = new TH2F("SamplesYT", "Single Event YT", fNRowsY, 0, fNRowsY, fNTimeBins, 0, fNTimeBins);
  fSamplesYT->SetXTitle("Y Row");
  sprintf(tBSize, "Time bin (%d ns)", fTimeBinSize);
  fSamplesYT->SetYTitle(tBSize);
  fSamplesYT->SetZTitle("ADC Count");
  fSamplesYT->GetXaxis()->SetTitleOffset(1.4);
  fSamplesYT->GetYaxis()->SetTitleOffset(1.4);

  // Single Event 2d: Strip vs. T
  fSamplesST = new TH2F("SamplesST", "Single Event ST", fNRowsX*fNRowsY, 0, fNRowsX*fNRowsY, fNTimeBins, 0, fNTimeBins);
  fSamplesST->SetXTitle("Strip");
  sprintf(tBSize, "Time bin (%d ns)", fTimeBinSize);
  fSamplesST->SetYTitle(tBSize);
  fSamplesST->SetZTitle("ADC Count");
  fSamplesST->GetXaxis()->SetTitleOffset(1.4);
  fSamplesST->GetYaxis()->SetTitleOffset(1.4);

  // Single Event 2d: X vs. Y
  fSamplesXY = new TH2F("SamplesXY", "Single Event XY", fNRowsX, 0, fNRowsX, fNRowsY, 0, fNRowsY);
  fSamplesXY->SetXTitle("X Row");
  fSamplesXY->SetYTitle("Y Row");
  fSamplesXY->SetZTitle("ADC Count (Avg Charge)");
  fSamplesXY->GetXaxis()->SetTitleOffset(1.4);
  fSamplesXY->GetYaxis()->SetTitleOffset(1.4);

  // All Events 2d: X vs. Y
  fHitMap = new TH2F("HitMap", "Hit Map", fNRowsX, 0, fNRowsX, fNRowsY, 0, fNRowsY);
  fHitMap->SetXTitle("X Row");
  fHitMap->SetYTitle("Y Row");
  fHitMap->SetZTitle("Frequency");
  fHitMap->GetXaxis()->SetTitleOffset(1.4);
  fHitMap->GetYaxis()->SetTitleOffset(1.4);

  // All Events Charge 2d: X vs. Y
  fChargeMap = new TH2F("ChargeMap", "Peak Charge Map", fNRowsX, 0, fNRowsX,fNRowsY,0,fNRowsY);
  fChargeMap->SetXTitle("X Row");
  fChargeMap->SetYTitle("Y Row");

  //  All Events Charge Avg 2d: X vs. Y
  fChargeMapAvg = new TH2F("ChargeMapAvg", "Charge Map Average (3 Time Bins)", fNRowsX, 0, fNRowsX,fNRowsY,0,fNRowsY);
  fChargeMapAvg->SetXTitle("X Row");
  fChargeMapAvg->SetYTitle("Y Row");

  //  All Events Charge Int 2d: X vs. Y
  fChargeMapInt = new TH2F("ChargeMapInt", "Charge Map Integral", fNRowsX, 0, fNRowsX,fNRowsY,0,fNRowsY);
  fChargeMapInt->SetXTitle("X Row");
  fChargeMapInt->SetYTitle("Y Row");

  // Strip Occupancy X
  fStripOccupancyX = new TH1F("StripOccupancyX", "Strip Occupancy X", fNRowsX, -.5, fNRowsX-.5);
  fStripOccupancyX->SetXTitle("X Row");
  fStripOccupancyX->SetYTitle("Frequency");

 // Strip Occupancy Y
  fStripOccupancyY = new TH1F("StripOccupancyY", "Strip Occupancy Y", fNRowsY, -.5, fNRowsY-.5);
  fStripOccupancyY->SetXTitle("Y Row");
  fStripOccupancyY->SetYTitle("Frequency");

 // Strip Occupancy P
  fStripOccupancyS = new TH1F("StripOccupancyS", "Strip Occupancy", fNRowsX*fNRowsY, -0.5, (fNRowsX*fNRowsY)-.5);
  fStripOccupancyS->SetXTitle("Strip");
  fStripOccupancyS->SetYTitle("Frequency");

  // Time Distribution
  fTimeDist = new TH1F("TimeDistribution", "Time Distribution", fNTimeBins, 0, fNTimeBins);
  fTimeDist->SetXTitle("Time Bin");
  fTimeDist->SetYTitle("Frequency");

  for (int i=0; i < fNDevices*fNChannels; ++i) {
    char t[100];
    sprintf(t, "Samples%d",i);
    TH1F *samples = new TH1F(t, t,1024 , -0.5, 1023.5);
    fSamplesVec.push_back(samples);
  }

}

//////////////////////////////////////////////////////

// 3d histo that shows charge in time and space. Can be called even during pedestal runs
void HistoManager::FillRawDataHisto(Processor *processor) {
  fRawSamplesST->Reset();
  fRawSamplesXY->Reset();
  vector<Packet*> packet_v;
  for (int i=0; i < fNDevices*fNChannels; ++i) {
    fRawSamplesVec[i]->Reset();
  }

  for (int i=0; i < fNDevices; ++i) {
    if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
      Packet *packet;
      int packetNb = fProcessor->GetPacketNb(i);
      packet = (fProcessor->GetEvent())->getPacket(packetNb);
      packet_v.push_back(packet);
    }
  }
  fProcessor = processor;

  map<Int_t,vector<Float_t> > rawData = processor->GetRawData();
  vector<Int_t> xMapping = processor->GetXMapping(); 
  vector<Int_t> yMapping = processor->GetYMapping(); 
  vector<Int_t> stripMapping = processor->GetStripMapping(); 
  Int_t nStrips = processor->GetNStrips();

if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
  for (int h=0; h < fNDevices; ++h) {
    for (int i =0; i < 4500; ++i) {

      (fRawSamplesVec[h])->Fill(i,packet_v[h]->iValue(i,h*2,"RAWSAMPLES"));
    }
  }
 }

  for (int stripNo=0; stripNo < nStrips*fNDevices; ++stripNo) {
    //  cout << " strip number " << stripNo << endl;
    Int_t stripCoordinate = stripMapping[stripNo];
    if (stripCoordinate == -100) continue; // unused channels
    Int_t xCoord = xMapping[stripNo]; 
    Int_t yCoord = yMapping[stripNo];
    Float_t raw = 0;
    int device;
    if (stripNo < nStrips) device = 0;
    else if (stripNo >= nStrips && stripNo < nStrips*2) device = 1;
    else if (stripNo >= nStrips*2 && stripNo < nStrips*3) device = 2;
    else device = 3;
    for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      fRawSamplesST->Fill(stripCoordinate,timeBin,rawData[stripCoordinate][timeBin]);
      raw += rawData[stripCoordinate][timeBin];
      if (!fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
        (fRawSamplesVec[stripNo])->Fill(timeBin,rawData[stripCoordinate][timeBin]);
      }
  
    }
    fRawSamplesXY->Fill(xCoord,yCoord,raw);
  }


 
}

//////////////////////////////////////////////////////

void HistoManager::FillMonitoringHistos(Processor *processor) {
  Int_t fNTimeBins = processor->GetNTimeBins();
  vector<Hit*> hit_v = processor->GetHitVec();
  fSamplesXT->Reset();
  fSamplesYT->Reset();
  fSamplesST->Reset();
  fSamplesXY->Reset();
  Bool_t saturated = 0;
  for (int stripNo = 0; stripNo < (int) hit_v.size(); ++stripNo) {
    Hit * hit = hit_v[stripNo];
    (fSamplesVec[stripNo])->Reset();
    for(int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      fSamplesXT->Fill(hit->GetXRowNo(),timeBin,hit->GetTimeBinCharge(timeBin));
      fSamplesYT->Fill(hit->GetYRowNo(),timeBin,hit->GetTimeBinCharge(timeBin));
      fSamplesST->Fill(hit->GetHitNo(),timeBin,hit->GetTimeBinCharge(timeBin));
      (fSamplesVec[stripNo])->Fill(timeBin,hit->GetTimeBinCharge(timeBin));
    }
    fSamplesXY->Fill(hit->GetXRowNo(),hit->GetYRowNo(), hit->GetAvgCharge());
    if (hit->GetMaxCharge() != 0) fHitMap->Fill(hit->GetXRowNo(),hit->GetYRowNo());
    fChargeMap->Fill(hit->GetXRowNo(), hit->GetYRowNo(), hit->GetMaxCharge());
    fChargeMapAvg->Fill(hit->GetXRowNo(), hit->GetYRowNo(), hit->GetAvgCharge());
    fChargeMapInt->Fill(hit->GetXRowNo(), hit->GetYRowNo(), hit->GetIntegralCharge());
    /*
    // Added to study saturated channels
    fChargeMapInt->SetBinError(strip->GetStripNo()+1,TMath::Sqrt(fChargeMapInt->GetBinContent(strip->GetStripNo()+1)));
    if (strip->GetSaturation() == 1) {
      //cout << " bin error before "  << fChargeMapInt->GetBinError(strip->GetStripNo()+1) << endl;
      fChargeMapInt->SetBinError(strip->GetStripNo()+1,fChargeMapInt->GetBinContent(strip->GetStripNo()+1));
      //fChargeMapInt->Sumw2();
      //cout << " bin error after "  << fChargeMapInt->GetBinError(strip->GetStripNo()+1) << endl;
      saturated = 1;
    }
    if (saturated == 1 && stripNo == (int) strip_v.size()-1 ) ++fSatCount;
    */
    if (hit->GetMaxCharge() != 0) {
      fStripOccupancyX->Fill(hit->GetXRowNo()); 
      fStripOccupancyY->Fill(hit->GetYRowNo());
      fStripOccupancyS->Fill(hit->GetHitNo());
      fTimeDist->Fill(hit->GetMaxTimeBin());
    }
  }
}

//////////////////////////////////////////////////////

void HistoManager::WriteHistos() {
  // Normalize ChargeMap
  fChargeMap->Divide(fHitMap);
  fChargeMapAvg->Divide(fHitMap);
  fChargeMapInt->Divide(fHitMap);

  char fileName[50];
  sprintf(fileName,"../Analysis/Histos/Run%d.root",fProcessor->GetRunNum());
  TFile *file = TFile::Open(fileName,"RECREATE");
  file->cd();
  fRawSamplesXY->Write();
  fRawSamplesST->Write();
  fSamplesXT->Write();
  fSamplesYT->Write();
  fSamplesST->Write();
  fSamplesXY->Write();
  fStripOccupancyX->Write();
  fStripOccupancyY->Write();
  fStripOccupancyS->Write();
  fTimeDist->Write();
  fHitMap->Write();
  fChargeMap->Write();
  fChargeMapAvg->Write();
  fChargeMapInt->Write();
  gDirectory->pwd(); 
  file->Close();
  gROOT->cd(0);
}
