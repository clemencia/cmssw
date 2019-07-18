from __future__ import print_function
import FWCore.ParameterSet.Config as cms

process = cms.Process('test')

import sys
if len(sys.argv) > 3:
    startrun = sys.argv[2]
    subdir = sys.argv[3]+"/"
else:
    print("not able to run")
    exit()
if len(sys.argv) > 5:
    tagname = sys.argv[4]
    filename = sys.argv[5]
else:
    tagname="CTPPSRPAlignment_real_test"
    filename="CTPPSRPRealAlignment_table.db"

process.source = cms.Source("EmptyIOVSource",
    timetype = cms.string('runnumber'),
    firstValue = cms.uint64(int(startrun)), 
    lastValue  = cms.uint64(int(startrun)),
    interval = cms.uint64(1)
)




# load the alignment xml file
##process.load("CondFormats.CTPPSReadoutObjects.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi")
#process.ctppsRPAlignmentCorrectionsDataESSourceXML.XMLFile = cms.string("CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml")
process.load("CalibPPS.ESProducers.ctppsRPAlignmentCorrectionsDataESSourceXML_cfi")
process.ctppsRPAlignmentCorrectionsDataESSourceXML.RealFiles = cms.vstring(
    #"CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml"
    "CondTools/CTPPS/test/"+subdir+"RPixGeometryCorrections-2017-2018.xml",
    "CondTools/CTPPS/test/"+subdir+"timing_RP_2017_preTS2_v2.xml",
    "CondTools/CTPPS/test/"+subdir+"timing_RP_2017_postTS2_v2.xml",
    "CondTools/CTPPS/test/"+subdir+"timing_RP_2018_v4.xml",
    "CondTools/CTPPS/test/"+subdir+"real_alignment_iov"+startrun+".xml"
    )
process.ctppsRPAlignmentCorrectionsDataESSourceXML.MeasuredFiles = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml")
process.ctppsRPAlignmentCorrectionsDataESSourceXML.MisalignedFiles = cms.vstring("CondFormats/CTPPSReadoutObjects/xml/sample_alignment_corrections.xml")

esPreferLocalAlignment = cms.ESPrefer("CTPPSRPAlignmentCorrectionsDataESSourceXML", "ctppsRPAlignmentCorrectionsDataESSourceXML")


#Database output service
process.load("CondCore.CondDB.CondDB_cfi")
# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:'+filename


process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(
    cms.PSet(
        record = cms.string('RPRealAlignmentRecord'),
        tag = cms.string(tagname),
    )
  )
)


# print the mapping and analysis mask
process.writeCTPPSRPAlignments = cms.EDAnalyzer("CTPPSRPAlignmentInfoAnalyzer",
    cms.PSet(
        iov = cms.uint64(int(startrun)),
        record = cms.string("RPRealAlignmentRecord")
    )
)

process.path = cms.Path(
  process.writeCTPPSRPAlignments
)
