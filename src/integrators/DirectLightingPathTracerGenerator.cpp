#include "integrators/DirectLightingPathTracerGenerator.h"
#include "integrators/IntegratorFactory.h"

using namespace ray_storm::integrators;

DirectLightingPathTracerGenerator::DirectLightingPathTracerGenerator(bool mis) : mis(mis)
{

}

AbstractIntegratorPtr DirectLightingPathTracerGenerator::createIntegrator()
{
  return IntegratorFactory::createDirectLightingPathTracer(this->mis);
}
