#include "integrators/IntegratorFactory.h"

#include "integrators/PathTracer.h"
#include "integrators/DirectLightingPathTracer.h"
#include "integrators/LightPathTracer.h"
#include "integrators/BidirectionalPathTracer.h"

using namespace ray_storm::integrators;

AbstractIntegratorPtr IntegratorFactory::createPathTracer()
{
  return AbstractIntegratorPtr(new PathTracer());
}

AbstractIntegratorPtr IntegratorFactory::createDirectLightingPathTracer(bool mis)
{
  return AbstractIntegratorPtr(new DirectLightingPathTracer(mis));
}

AbstractIntegratorPtr IntegratorFactory::createLightPathTracer()
{
  return AbstractIntegratorPtr(new LightPathTracer());
}

AbstractIntegratorPtr IntegratorFactory::createBidirectionalPathTracer()
{
  return AbstractIntegratorPtr(new BidirectionalPathTracer());
}