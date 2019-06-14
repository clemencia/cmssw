#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include <iostream>
#include <sstream>
#include <cassert>

int main(int argc, char** argv) {
  reweight::PoissonMeanShifter shifter1(1.0);
  reweight::PoissonMeanShifter shifter2(-1.0);
  union {
    double f;
    uint64_t i;
  } tmp;
  uint64_t results1[] = {
      4602120534948439556, 4602999367379724634, 4603649431361458099, 4604346298551959050, 4605085334134721371,
      4605952101134374348, 4606930325307275717, 4607641651304380340, 4608342322235126568, 4609192422151148985,
      4610204782303181931, 4611433186344307324, 4612294974902802620, 4613148684003566412, 4614154274749963460,
      4615373700910267687, 4616505466755624961, 4617449995446474307, 4618367705201542662, 4620104415940946570,
      4607361093840994952, 4600992940758467837, 4607581658621163415, 4610803005240562260, 4611154016586699445};
  uint64_t results2[] = {
      4612145272999389010, 4611106990623296879, 4609907727078524390, 4608915764224599765, 4608046613631793607,
      4607285911519454382, 4606029308144226211, 4604816295109791360, 4603752250342390819, 4602842140411693652,
      4601400013051255806, 4600091501186722562, 4598979994784289015, 4597863822652641510, 4596272178486734738,
      4595072059258033044, 4593705811246276916, 4592838850303609584, 4590954616277111816, 4589260254010903968,
      4548171036046341834, 4538272860974572120, 4532501729851183836, 4525211654956168943, 4495387381920187034};

  for (int i = 0; i < 25; ++i) {
    tmp.f = shifter1.ShiftWeight(i);
    assert(results1[i] == tmp.i);
  }

  for (int i = 0; i < 25; ++i) {
    tmp.f = shifter2.ShiftWeight(i);
    assert(results2[i] == tmp.i);
  }
}
