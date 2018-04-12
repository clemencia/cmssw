import FWCore.ParameterSet.Config as cms

process = cms.Process('test')


process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(1),
    lastValue = cms.uint64(1),
    interval = cms.uint64(1)
)

#process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(1)
#)

# load a mapping
process.load("CondFormats.CTPPSReadoutObjects.CTPPSRPAlignmentCorrectionsDataESSourceXML_cfi")
process.ctppsRPAlignmentCorrectionsDataESSourceXML.XMLFile = cms.string("CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml")


#Database output service
process.load("CondCore.CondDB.CondDB_cfi")
# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:CTPPSRPAlignment.db'


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
    cms.PSet(
        record = cms.string('CTPPSRPAlignmentCorrectionsDataRcd'),
        tag = cms.string('CTPPSRPAlignment_tag'),
    )
  )
)


# print the mapping and analysis mask
process.writeCTPPSRPAlignments = cms.EDAnalyzer("CTPPSRPAlignmentInfoAnalyzer",
    cms.PSet(
        iov = cms.uint64(1),
        record = cms.string("CTPPSRPAlignmentCorrectionsDataRcd")
    )
)

process.path = cms.Path(
  process.writeCTPPSRPAlignments
)
