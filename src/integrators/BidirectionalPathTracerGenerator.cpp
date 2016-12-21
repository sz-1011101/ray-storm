#include "integrators/BidirectionalPathTracerGenerator.h"
#include "integrators/IntegratorFactory.h"

using namespace ray_storm::integrators;

AbstractIntegratorPtr BidirectionalPathTracerGenerator::createIntegrator()
{
  return IntegratorFactory::createBidirectionalPathTracer();
}