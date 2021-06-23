import FWCore.ParameterSet.Config as cms

process = cms.Process("Dfinder")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 10

#process.options = cms.untracked.PSet(
#   wantSummary = cms.untracked.bool( True ),
#)

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1)
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                'file:/afs/cern.ch/work/n/nsaha/PbPb2018/PbPb2018_MB.root'
                            ),
                        )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('PbPb2018_Ds_june17.root')
                               )

#Set Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_dataRun2_v6', '')


print('\n\033[31m~*~ USING CENTRALITY TABLE FOR PbPb 2018 DATA ~*~\033[0m\n')
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_DataPbPb_periHYDJETshape_run2v1031x02_offline"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])

process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')
process.hiCentrality.produceHFhits = False
process.hiCentrality.produceHFtowers = False
process.hiCentrality.produceEcalhits = False
process.hiCentrality.produceZDChits = False
process.hiCentrality.produceETmidRapidity = False
process.hiCentrality.producePixelhits = False
process.hiCentrality.produceTracks = False
process.hiCentrality.producePixelTracks = False
process.hiCentrality.reUseCentrality = True
process.hiCentrality.srcReUse = cms.InputTag("hiCentrality","","reRECO")                                                                                               
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("")
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")


process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
             tag = cms.string("JPcalib_Data103X_2018PbPb_v1"),
             connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
             label = cms.untracked.string("HFtowers")
         )
      ])

process.cent_seq = cms.Sequence(process.hiCentrality * process.centralityBin) 


from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import overrideJEC_DATA_PbPb5020_2018
process = overrideJEC_DATA_PbPb5020_2018(process)


############################
# Event Analysis
############################
process.load('HeavyIonsAnalysis.EventAnalysis.hievtanalyzer_data_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.skimanalysis_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.hltobject_cfi')
process.load('HeavyIonsAnalysis.EventAnalysis.l1object_cfi')

from HeavyIonsAnalysis.EventAnalysis.hltobject_cfi import trigger_list_data
process.hltobject.triggerNames = trigger_list_data


#########################
# Track Analyzer
#########################
process.load('HeavyIonsAnalysis.TrackAnalysis.ExtraTrackReco_cff')
process.load('HeavyIonsAnalysis.TrackAnalysis.TrkAnalyzers_cff')


#########################
# RecHits & pfTowers (HF, Castor & ZDC)
#########################
# ZDC RecHit Producer
process.load('RecoHI.ZDCRecHit.QWZDC2018Producer_cfi')
process.load('RecoHI.ZDCRecHit.QWZDC2018RecHit_cfi')

process.load('HeavyIonsAnalysis.JetAnalysis.rechitanalyzer_cfi')
process.rechitanalyzerpp.doZDCRecHit = True
process.rechitanalyzerpp.zdcRecHitSrc = cms.InputTag("QWzdcreco")
process.pfTowerspp.doHF = False


#Recover peripheral primary vertices
process.load("RecoVertex.PrimaryVertexProducer.OfflinePrimaryVerticesRecovery_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")


#########################
# Event Selection
#########################

process.load('HeavyIonsAnalysis.Configuration.collisionEventSelection_cff')
process.pclusterCompatibilityFilter = cms.Path(process.clusterCompatibilityFilter)
process.pprimaryVertexFilter = cms.Path(process.primaryVertexFilter)
process.pBeamScrapingFilter = cms.Path(process.beamScrapingFilter)
process.collisionEventSelectionAOD = cms.Path(process.collisionEventSelectionAOD)
process.collisionEventSelectionAODv2 = cms.Path(process.collisionEventSelectionAODv2)

process.load('HeavyIonsAnalysis.Configuration.hfCoincFilter_cff')
process.phfCoincFilter1Th3 = cms.Path(process.hfCoincFilterTh3)
process.phfCoincFilter2Th3 = cms.Path(process.hfCoincFilter2Th3)
process.phfCoincFilter3Th3 = cms.Path(process.hfCoincFilter3Th3)
process.phfCoincFilter4Th3 = cms.Path(process.hfCoincFilter4Th3)
process.phfCoincFilter5Th3 = cms.Path(process.hfCoincFilter5Th3)
process.phfCoincFilter1Th4 = cms.Path(process.hfCoincFilterTh4)
process.phfCoincFilter2Th4 = cms.Path(process.hfCoincFilter2Th4)
process.phfCoincFilter3Th4 = cms.Path(process.hfCoincFilter3Th4)
process.phfCoincFilter4Th4 = cms.Path(process.hfCoincFilter4Th4)
process.phfCoincFilter5Th4 = cms.Path(process.hfCoincFilter5Th4)
process.phfCoincFilter1Th5 = cms.Path(process.hfCoincFilterTh5)
process.phfCoincFilter4Th2 = cms.Path(process.hfCoincFilter4Th2)

process.load("HeavyIonsAnalysis.VertexAnalysis.PAPileUpVertexFilter_cff")
process.pVertexFilterCutG = cms.Path(process.pileupVertexFilterCutG)
process.pVertexFilterCutGloose = cms.Path(process.pileupVertexFilterCutGloose)
process.pVertexFilterCutGtight = cms.Path(process.pileupVertexFilterCutGtight)
process.pVertexFilterCutGplus = cms.Path(process.pileupVertexFilterCutGplus)
process.pVertexFilterCutE = cms.Path(process.pileupVertexFilterCutE)
process.pVertexFilterCutEandG = cms.Path(process.pileupVertexFilterCutEandG)

process.load('HeavyIonsAnalysis.JetAnalysis.EventSelection_cff')
process.pHBHENoiseFilterResultProducer = cms.Path(process.HBHENoiseFilterResultProducer)
process.HBHENoiseFilterResult = cms.Path(process.fHBHENoiseFilterResult)
process.HBHENoiseFilterResultRun1 = cms.Path(process.fHBHENoiseFilterResultRun1)
process.HBHENoiseFilterResultRun2Loose = cms.Path(process.fHBHENoiseFilterResultRun2Loose)
process.HBHENoiseFilterResultRun2Tight = cms.Path(process.fHBHENoiseFilterResultRun2Tight)
process.HBHEIsoNoiseFilterResult = cms.Path(process.fHBHEIsoNoiseFilterResult)

#process.pAna = cms.EndPath(process.skimanalysis)

from HLTrigger.Configuration.CustomConfigs import MassReplaceInputTag
process = MassReplaceInputTag(process,"offlinePrimaryVertices","offlinePrimaryVerticesRecovery")
process.offlinePrimaryVerticesRecovery.oldVertexLabel = "offlinePrimaryVertices"

#process.ana_step = cms.Path(
    #process.offlinePrimaryVerticesRecovery +
    #process.phfCoincFilter2Th4 +
    #process.pprimaryVertexFilter +
    #process.pclusterCompatibilityFilter +
    #process.centralityBin +
    #process.hltanalysis +
    #process.hltobject +
    #process.hiEvtAnalyzer +
    #process.jetSequence +
    #process.hiPuRhoR3Analyzer + 
    #process.hiFJRhoAnalyzer +
    #process.hiFJRhoAnalyzerFinerBins +
    #process.trackSequencesPP +
    #process.zdcdigi +
    #process.QWzdcreco +
    #process.rechitanalyzerpp
    #)

#################### D/B finder #################
AddCaloMuon = False
runOnMC = False 
HIFormat = False
UseGenPlusSim = False
VtxLabel = "offlinePrimaryVerticesRecovery"
TrkLabel = "generalTracks"
useL1Stage2 = True
HLTProName = "HLT"

from Analyzers.Dfinder.finderMaker_75X_cff import finderMaker_75X
finderMaker_75X(process, AddCaloMuon, runOnMC, HIFormat, UseGenPlusSim, VtxLabel, TrkLabel, useL1Stage2, HLTProName)

process.Dfinder.MVAMapLabel = cms.InputTag(TrkLabel,"MVAValues")
process.Dfinder.makeBntuple = cms.bool(True)
process.Dfinder.tkPtCut = cms.double(1.0) # before fit
process.Dfinder.dPtCut = cms.vdouble(8., 8., 8., 8., 8., 8., 5., 5., 8., 8., 8., 8., 8., 8., 8., 8.) # before fit
process.Dfinder.Dchannel = cms.vint32(0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0)
process.Dfinder.VtxChiProbCut = cms.vdouble(0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
process.Dfinder.svpvDistanceCut = cms.vdouble(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0)
process.Dfinder.doTkPreCut = cms.bool(True)
process.Dfinder.doMuPreCut = cms.bool(True)

process.p = cms.Path(process.DfinderSequence)

process.eventFilter = cms.Sequence(
    process.offlinePrimaryVerticesRecovery *
    process.hfCoincFilter2Th4 *
    process.primaryVertexFilter *
    process.clusterCompatibilityFilter
)

process.eventFilter_step = cms.Path( process.eventFilter )
process.pcentandef_step = cms.Path(process.eventFilter * process.cent_seq)
process.dsrereco_step = cms.Path(process.eventFilter * process.Dfinder)


