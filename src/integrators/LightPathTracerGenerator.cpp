#include "integrators/LightPathTracerGenerator.h"
#include "integrators/IntegratorFactory.h"

using namespace ray_storm::integrators;

AbstractIntegratorPtr LightPathTracerGenerator::createIntegrator()
{
  return IntegratorFactory::createLightPathTracer();
}