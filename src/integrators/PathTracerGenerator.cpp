#include "integrators/PathTracerGenerator.h"
#include "integrators/IntegratorFactory.h"

using namespace ray_storm::integrators;

AbstractIntegratorPtr PathTracerGenerator::createIntegrator()
{
  return IntegratorFactory::createPathTracer();
}