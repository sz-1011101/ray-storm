#include "integrators/DirectLightingPathTracer.h"

#include "integrators/PathTraceHelper.h"

using namespace ray_storm::integrators;

DirectLightingPathTracer::DirectLightingPathTracer(bool mis) : mis(mis)
{

}

void DirectLightingPathTracer::sample(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  random::RandomizationHelper &randHelper
)
{
  if (this->mis)
  {
    PathTraceHelper::directIlluminationBounce(scene, camera, sampleRay, this->walk, randHelper);
  }
  else
  {
    PathTraceHelper::directIllumination(scene, camera, sampleRay, this->walk, randHelper);
  }
}