import FWCore.ParameterSet.Config as cms

ctppsRPAlignmentCorrectionsDataESSourceXML = cms.ESSource("CTPPSRPAlignmentCorrectionsDataESSourceXML",
    verbosity = cms.untracked.uint32(0),
#    MeasuredFiles = cms.vstring(),
    XMLFile = cms.string("")
#    MisalignedFiles = cms.vstring()
)
