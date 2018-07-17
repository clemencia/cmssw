import FWCore.ParameterSet.Config as cms
import sys

arguments=sys.argv

rootfilename = "parametrisation.root"
if len(arguments)>2:
    rootfilename=arguments[2]

process = cms.Process("TEST")

process.MessageLogger=cms.Service("MessageLogger",
                                  cout=cms.untracked.PSet(threshold=cms.untracked.string('INFO')),
                                  destinations=cms.untracked.vstring("cout")
                                  )

process.load("CondCore.CondDB.CondDB_cfi")
#output DB (local sqlite)
process.CondDB.connect = cms.string('sqlite_file:CTPPSROOTFILE_Test.db')
process.CondDB.BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService')
process.CondDB.DBParameters.messageLevel = cms.untracked.int32(1) #3 for high verbosity

process.source = cms.Source("EmptyIOVSource", #needed to EvSetup in order to load data
                            timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(1),
                            lastValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          process.CondDB,
                                          timetype = cms.untracked.string('runnumber'),
                                          toPut = cms.VPSet(cms.PSet(record = cms.string('FileBlob'),
                                                                     tag = cms.string('CTPPSROOTFILE_Test') 
                                                                     )
                                                            ),
                                          logconnect = cms.untracked.string('sqlite_file:CTPPSROOTFILE_Testlog.db') 
                                          )

process.ctppsOpticsParamRootFileTest = cms.EDAnalyzer("CTPPSOpticsParamRootFilePopConAnalyzer",
                                           record = cms.string('FileBlob'),
                                           loggingOn = cms.untracked.bool(True), #always True, needs to create the log db
                                           SinceAppendMode = cms.bool(True),
                                           Source = cms.PSet(ROOTFile = cms.untracked.string(rootfilename),
                                                             firstSince = cms.untracked.uint64(1), #1, 43434, 46335, 51493, 51500
                                                             debug = cms.untracked.bool(True)
                                                             )
                                           )

process.p = cms.Path(process.ctppsOpticsParamRootFileTest)
