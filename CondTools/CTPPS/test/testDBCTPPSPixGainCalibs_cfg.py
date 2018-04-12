import FWCore.ParameterSet.Config as cms
import sys,os

arguments = sys.argv
#for i in range(0,len(arguments)):
#    print i," ",arguments[i]
infile = "Gains_Sept299.root"
tagname = "CTPPSPixelGainCalibrations_test"
sqlitename = "ctppsgains_test.db"
runnumber = 1
doDummy = False
gainlo = 0.0
gainhi = 100.0
minNp = 3
if len(arguments)<3:
    print "using defaults"
    print "usage: cmsRun testDBCTPPSPixGainCalibs_cfg.py Gains.root tagname sqlitename.db 111111 True 0.0 1000000.0 3"
else:
    print "here"
    infile = arguments[2]
    if len(arguments)>3: tagname    = arguments[3]
    if len(arguments)>4: sqlitename = arguments[4]
    if len(arguments)>5:
        print "whaat?"
        runnumber  = int(arguments[5])
        print "runno = ",runnumber
    if len(arguments)>6:
        doDummy    = bool(arguments[6])
        print "useDummyValues = ",doDummy
    if len(arguments)>7: 
        gainlo     = float(arguments[7])
        print "gainLowLimit = ",gainlo
    if len(arguments)>8: 
        gainhi     = float(arguments[8])
        print "gainHighLimit = ",gainhi
    if len(arguments)>9: 
        minNp      = int(arguments[9])
        print "minimumNpFit = ",minNp



process = cms.Process("CTPPSPixelGainDB")
#process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
#from Configuration.AlCa.autoCond_condDBv2 import autoCond
#process.GlobalTag.globaltag = autoCond['run2_design']

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
#process.TFileService = cms.Service("TFileService", fileName = cms.string('/tmp/rougny/histos.root') )

process.pixGainDB = cms.EDAnalyzer("WriteCTPPSPixGainCalibrations",
                                   inputrootfile = cms.untracked.string(infile),
                                   record = cms.untracked.string('CTPPSPixelGainCalibrationsRcd'),
                                   useDummyValues = cms.untracked.bool(doDummy),
                                   gainLowLimit = cms.untracked.double(gainlo),
                                   gainHighLimit=cms.untracked.double(gainhi),
                                   minimumNpfit = cms.untracked.int32(minNp)
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
    firstValue = cms.uint64(runnumber),
    lastValue = cms.uint64(runnumber),
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
            tag = cms.string(tagname)
            )
        ),
    connect = cms.string('sqlite_file:'+sqlitename)
)

process.p = cms.Path(process.pixGainDB)
