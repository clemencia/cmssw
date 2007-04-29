#ifndef CD_Strip1DMeasurementTransformator_H_
#define CD_Strip1DMeasurementTransformator_H_

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

/** Helper class for accessing the RecHit and the TrajectoryState parameters
 *  and errors in the measurement frame. The RecHits are treated as
 *  1-dimensional, the second coordinate is ignored.
 */

class Strip1DMeasurementTransformator {

private:

  typedef TrajectoryStateOnSurface TSOS;
  typedef AlgebraicSymMatrix55 ASM55;
  typedef AlgebraicMatrix15 AM15;
  typedef AlgebraicVector5 AV5;

public:

  Strip1DMeasurementTransformator(const TSOS& aTsos, const TransientTrackingRecHit& aHit);
  
  ~Strip1DMeasurementTransformator() {}

  double hitParameters() const;
  AV5 trajectoryParameters() const;
  double projectedTrajectoryParameters() const;
  double hitError() const;
  const ASM55 & trajectoryError() const;
  double projectedTrajectoryError() const;
  AM15 projectionMatrix() const;

  const TransientTrackingRecHit& hit() const {return theRecHit;}
  const TSOS& state() const {return theState;}
  const StripTopology* topology() const {return theTopology;}

private:

  const TransientTrackingRecHit& theRecHit;
  TSOS theState;
  const StripTopology* theTopology;

  void init();
};

#endif //CD_Strip1DMeasurementTransformator_H_
