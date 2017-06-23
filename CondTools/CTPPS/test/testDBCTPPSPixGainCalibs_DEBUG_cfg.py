import FWCore.ParameterSet.Config as cms

process = cms.Process("CTPPSPixelGainDB")


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations   = cms.untracked.vstring('myDebugOutputFile'),
                                    myDebugOutputFile = cms.untracked.PSet(  threshold = cms.untracked.string('DEBUG'),      
                                                                             default = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
                                                                             debugModules = cms.untracked.vstring('pixGainDB') 
                                                                             )
                                    )

#process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.autoCond_condDBv2 import autoCond
#process.GlobalTag.globaltag = autoCond['run2_design']

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
#process.TFileService = cms.Service("TFileService", fileName = cms.string('/tmp/rougny/histos.root') )

process.pixGainDB = cms.EDAnalyzer("myCTPPSPixGainCalibsMaker",
    inputrootfile = cms.untracked.string('CTPPSPixGainCalibration_new.root'),
    record = cms.untracked.string('CTPPSPixelGainCalibrationsRcd')#,
#    useMeanWhenEmpty = cms.untracked.bool(True),
#    badChi2Prob = cms.untracked.double(0.00001)                                       
    )

#process.gainDBHLT = cms.EDAnalyzer("SiPixelGainCalibrationDBUploader",
#    inputrootfile = cms.untracked.string('file:///tmp/rougny/test.root'),
#    record = cms.untracked.string('SiPixelGainCalibrationForHLTRcd'),
#    useMeanWhenEmpty = cms.untracked.bool(True),  
#    badChi2Prob = cms.untracked.double(0.00001)                             
#    )

process.source = cms.Source("EmptyIOVSource",                            
    #lastRun = cms.untracked.uint32(1),
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(10),
    interval = cms.uint64(1)
    )

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(10),
        authenticationPath = cms.untracked.string('.')
        ),
    toPut = cms.VPSet(
        cms.PSet(
            record = cms.string('CTPPSPixelGainCalibrationsRcd'),
            tag = cms.string('CTPPSPixelGainCalibNew_v3')
            )
        ),
    connect = cms.string('sqlite_file:ctppspixnew3.db')
)

process.p = cms.Path(process.pixGainDB)
