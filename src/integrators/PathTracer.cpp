#include "integrators/PathTracer.h"

#include "integrators/PathTraceHelper.h"

using namespace ray_storm::integrators;

void PathTracer::sample(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  PathTraceHelper::naive(scene, camera, sampleRay, randHelper);
}