#include "CondFormats/Serialization/interface/Test.h"

#include "../src/headers.h"

int main()
{
  //testSerialization<BeamSpotObjects>();
  //testSerialization<SimBeamSpotObjects>();
  testSerialization<CTPPSAlignmentCorrections>();
  return 0;
}
