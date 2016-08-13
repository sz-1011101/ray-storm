#include "renderer/PathTracer.h"

using namespace ray_storm::renderer;

const uint BOUNCES = 4;
const uint SAMPLES = 32;

PathTracer::PathTracer()
{

}

PathTracer::PathTracer(scene::ScenePtr &scene, camera::AbstractCameraPtr &camera) : 
  scene(scene), camera(camera)
{

}

void PathTracer::setScene(scene::ScenePtr &scene)
{
  this->scene = scene;
}

void PathTracer::setCamera(camera::AbstractCameraPtr &camera)
{
  this->camera = camera;
}

void PathTracer::render()
{
  // TODO: more informative error handling
  if (this->scene == nullptr || this->camera == nullptr)
  {
    return;
  }

  puts("rendering...");
  const uint width = this->renderedData->getWidth();
  const uint height = this->renderedData->getHeight();

  this->renderedImage = cv::Mat::zeros(cv::Size(width, height), CV_32FC3);

  random::RandomizationHelper::MTEngine engine;

  for (uint x = 0; x < width; x++)
  {
    for (uint y = 0; y < height; y++)
    {
      glm::vec3 radianceSum(0.0f);
      geometry::Ray ray;
      camera->spawnRay(static_cast<float>(x)/width, static_cast<float>(y)/height, ray);

      for (uint s = 0; s < SAMPLES; s++)
      {
        // we can reuse the first ray
        radianceSum += walkPath(ray, engine);
      }
      this->renderedData->setPixelSRGB(x, y, radianceSum/static_cast<float>(SAMPLES));
    }
  }

  this->renderedData->signalChanged();

  puts("done!");
}

glm::vec3 PathTracer::walkPath(const geometry::Ray &intialRay, random::RandomizationHelper::MTEngine &engine)
{
  geometry::Ray ray = intialRay;
  // path tracing vars
  glm::vec3 weights[BOUNCES];
  glm::vec3 emittance[BOUNCES];
  int depth = 0;

  geometry::Intersection<geometry::Object> intersection;
  
  // light path reverse traversal
  for (uint b = 0; b < BOUNCES; b++)
  {
    depth = b;
    // we hit the sky...
    if (!scene->intersect(ray, intersection))
    {
      emittance[b] = this->scene->getSky();
      continue;
    }

    geometry::Object *iObj = intersection.intersected;
    geometry::SimpleIntersection iSmpl = intersection.intersection;
    materials::AbstractMaterial *iMat = iObj->getMaterial();

    geometry::Ray nextRay;
    // next ray
    nextRay.direction = random::RandomizationHelper::drawUniformRandomHemisphereDirection(
      engine, iSmpl.normal);
    nextRay.origin = iSmpl.position + iSmpl.normal*0.001f;

    const glm::vec3 brdf = iMat->evaluateBRDF(-ray.direction, iSmpl.normal, nextRay.direction);

    // brdf + cos weighting*(pdf for uniform hemisphere)
    weights[b] = brdf*2.0f*glm::dot(iSmpl.normal, nextRay.direction);
    emittance[b] = iMat->getEmittance();

    ray = nextRay;
  }

  // accumulate radiance according to rendering equation
  glm::vec3 radiance = emittance[depth];
  for (int b = depth - 1; b >= 0; b--)
  {
    radiance = emittance[b] + weights[b]*radiance;
  }

  return radiance;
}