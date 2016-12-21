#include "integrators/IntegratorGeneratorFactory.h"
#include "integrators/PathTracerGenerator.h"
#include "integrators/DirectLightingPathTracerGenerator.h"
#include "integrators/LightPathTracerGenerator.h"
#include "integrators/BidirectionalPathTracerGenerator.h"

using namespace ray_storm::integrators;

AbstractIntegratorGeneratorPtr IntegratorGeneratorFactory::createPathTracerGenerator()
{
  return AbstractIntegratorGeneratorPtr(new PathTracerGenerator());
}

AbstractIntegratorGeneratorPtr IntegratorGeneratorFactory::createDirectLightingPathTracerGenerator(bool mis)
{
  return AbstractIntegratorGeneratorPtr(new DirectLightingPathTracerGenerator(mis));
}

AbstractIntegratorGeneratorPtr IntegratorGeneratorFactory::createLightPathTracerGenerator()
{
  return AbstractIntegratorGeneratorPtr(new LightPathTracerGenerator());
}

AbstractIntegratorGeneratorPtr IntegratorGeneratorFactory::createBidirectionalPathTracerGenerator()
{
  return AbstractIntegratorGeneratorPtr(new BidirectionalPathTracerGenerator());
}