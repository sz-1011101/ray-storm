#include "integrators/BidirectionalPathTracer.h"

#include "integrators/PathTraceHelper.h"

using namespace ray_storm::integrators;

void BidirectionalPathTracer::sample(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  PathTraceHelper::bidirectional(scene, camera, sampleRay, this->eyeWalk, this->lightWalk, randHelper);
}