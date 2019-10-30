import FWCore.ParameterSet.Config as cms
import sys


if len(sys.argv) > 2:
    runno = int(sys.argv[2])
else:
    runno=1

if len(sys.argv) >3 :
    tagname = sys.argv[3]
else:
    tagname="CTPPSRPAlignment_real_offline"

if len(sys.argv) > 4:
    rcdname=sys.argv[4]
else:
    rcdname="RPRealAlignmentRecord"

if len(sys.argv)>5:
    connect_str=sys.argv[5]
else:
    connect_str="frontier://FrontierProd/CMS_CONDITIONS"

process = cms.Process("ProcessOne")

process.source= cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(runno),
    lastValue = cms.uint64(runno),
    interval = cms.uint64(1)
)

#Database output service
process.load("CondCore.CondDB.CondDB_cfi")
# input database (in this case local sqlite file)
process.CondDB.connect = connect_str

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
    process.CondDB,
    DumpStat=cms.untracked.bool(True),
    toGet = cms.VPSet(
      cms.PSet(
        record = cms.string(rcdname),
        tag = cms.string(tagname)
      )
    )
)

process.readSqlite = cms.EDAnalyzer("CTPPSRPAlignmentInfoReader",
                                    cms.PSet(     
        iov = cms.uint64(runno), 
        record=cms.string(rcdname)    
        )
                                    )

process.p = cms.Path(process.readSqlite)
