import FWCore.ParameterSet.Config as cms

process = cms.Process("ProcessOne")



process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
                                    destinations   = cms.untracked.vstring('myREADDebugOutputFile'),
                                    myREADDebugOutputFile = cms.untracked.PSet(  threshold = cms.untracked.string('DEBUG'),      
                                                                             default = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
                                                                             debugModules = cms.untracked.vstring('myprodtest') 
                                                                             )
                                    )

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(10),  
    lastValue = cms.uint64(10),   
    interval = cms.uint64(1)
)
#Database output service

process.load("CondCore.CondDB.CondDB_cfi")
# input database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:ctppspixnew3.db'  


process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(cms.PSet(
        record = cms.string('CTPPSPixelGainCalibrationsRcd'),
        tag = cms.string("CTPPSPixelGainCalibNew_v3")
    )),
)



process.myprodtest = cms.EDAnalyzer("myCTPPSPixGainCalibsESAnalyzer") 
 
process.p = cms.Path(process.myprodtest)


