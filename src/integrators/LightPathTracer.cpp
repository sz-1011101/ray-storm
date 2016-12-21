#include "integrators/LightPathTracer.h"

#include "integrators/PathTraceHelper.h"

using namespace ray_storm::integrators;

void LightPathTracer::sample(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  PathTraceHelper::bidirectional(scene, camera, sampleRay, randHelper);
}