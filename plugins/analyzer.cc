#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "TLorentzVector.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"



class analyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit analyzer(const edm::ParameterSet&);
      ~analyzer();

      edm::ConsumesCollector iC = consumesCollector();
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::ParameterSet parameters;

      //
      // --- Tokens and Handles
      //

      //edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
      //edm::EDGetTokenT<edm::View<pat::TriggerObjectStandAlone> > triggerObjects_;
      //edm::EDGetTokenT<pat::PackedTriggerPrescales>  triggerPrescales_;

      // L1
      edm::EDGetToken algToken_;
      std::shared_ptr<l1t::L1TGlobalUtil> l1GtUtils_;
      std::vector<std::string> l1Seeds_;
      //
      TString l1Names[100] = {""};
      Bool_t l1Result[100] = {false};

      // displacedMuons (reco::Muon // pat::Muon)
      edm::EDGetTokenT<edm::View<reco::Muon> > muonToken;
      edm::Handle<edm::View<reco::Muon> > muons;

      //
      // --- Variables
      //

      bool isData = true;

      // Event
      Int_t event = 0;
      Int_t lumiBlock = 0;
      Int_t run = 0;

      //
      // --- Output
      //
      std::string output_filename;
      TH1F *counts;
      TFile *file_out;
      TTree *tree_out;

};

// Constructor
analyzer::analyzer(const edm::ParameterSet& iConfig) {

  usesResource("TFileService");

  parameters = iConfig;

  // L1 initialization
  algToken_ = consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("AlgInputTag"));
  l1GtUtils_ = std::make_shared<l1t::L1TGlobalUtil>(iConfig, consumesCollector(), l1t::UseEventSetupIn::RunAndEvent);
  l1Seeds_ = iConfig.getParameter<std::vector<std::string> >("l1Seeds");
  for (unsigned int i = 0; i < l1Seeds_.size(); i++){
    const auto& l1seed(l1Seeds_.at(i));
    l1Names[i] = TString(l1seed);
  }

  counts = new TH1F("counts", "", 1, 0, 1);

  isData = parameters.getParameter<bool>("isData");

  muonToken = consumes<edm::View<reco::Muon> >  (parameters.getParameter<edm::InputTag>("displacedMuonCollection"));

}


// Destructor
analyzer::~analyzer() {
}


// beginJob (Before first event)
void analyzer::beginJob() {

   std::cout << "Begin Job" << std::endl;

   // Init the file and the TTree
   output_filename = parameters.getParameter<std::string>("nameOfOutput");
   file_out = new TFile(output_filename.c_str(), "RECREATE");

   // Analyzer parameters
   isData = parameters.getParameter<bool>("isData");

}

// endJob (After event loop has finished)
void analyzer::endJob()
{

    std::cout << "End Job" << std::endl;
    file_out->cd();
    counts->Write();
    file_out->Close();

}


// fillDescriptions
void analyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

}

// Analyze (per event)
void analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

   iEvent.getByToken(muonToken, muons);

   // Count number of events read
   counts->Fill(0);


   // -> Event info
   event = iEvent.id().event();
   lumiBlock = iEvent.id().luminosityBlock();
   run = iEvent.id().run();

  // L1
  // -> L1 seeds
  bool passL1 = false;
  l1GtUtils_->retrieveL1(iEvent, iSetup, algToken_);
  for (unsigned int i = 0; i < l1Seeds_.size(); i++){
    const auto& l1seed(l1Seeds_.at(i));
    bool l1htbit = 0;
    double prescale = -1;
    l1GtUtils_->getFinalDecisionByName(l1seed, l1htbit);
    l1GtUtils_->getPrescaleByName(l1seed, prescale);
    //l1Result[i] = l1htbit;
    //l1Prescale[i] = prescale;
    std::cout << l1seed << " " << l1htbit << " " << prescale << std::endl;
    if (l1htbit)
      passL1 = true;
  }
  if (!passL1)
    return;

   // displacedMuons
   for (unsigned int i = 0; i < muons->size(); i++) {
     const reco::Muon& muon(muons->at(i));
     std::cout << muon.pt() << std::endl;

     // Access the DSA track associated to the displacedMuon
     //if ( dmuon.isStandAloneMuon() ) {
     //  const reco::Track* outerTrack = (dmuon.standAloneMuon()).get();
     //  dmu_dsa_pt[ndmu] = outerTrack->pt();
     //}
   }

}

DEFINE_FWK_MODULE(analyzer);
