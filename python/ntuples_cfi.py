import FWCore.ParameterSet.Config as cms

analysis = cms.EDAnalyzer('analyzer',
    nameOfOutput = cms.string('output.root'),
    isData = cms.bool(True),
    EventInfo = cms.InputTag("generator"),
    RunInfo = cms.InputTag("generator"),
    BeamSpot = cms.InputTag("offlineBeamSpot"),
    displacedGlobalCollection = cms.InputTag("displacedGlobalMuons"),
    displacedStandAloneCollection = cms.InputTag("displacedStandAloneMuons"),
    displacedMuonCollection = cms.InputTag("slimmedDisplacedMuons"),
    AlgInputTag = cms.InputTag("gtStage2Digis"),
    l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis"),
    l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
    ReadPrescalesFromFile = cms.bool(False),
    l1Seeds = cms.vstring(["L1_SingleMuOpen"])
)


