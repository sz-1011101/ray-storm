#include "renderer/PathTracer.h"
#include "random/RandomizationHelper.h"

using namespace ray_storm::renderer;

const uint BOUNCES = 2;
const uint SAMPLES = 16;
const glm::vec3 SKY = glm::vec3(0.0, 0.2, 0.8);

void PathTracer::render(const scene::Scene &scene, camera::AbstractCameraPtr &camera)
{
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

      for (uint s = 0; s < SAMPLES; s++)
      {
        // path tracing vars
        glm::vec3 weights[BOUNCES];
        glm::vec3 emittance[BOUNCES];
        int depth = 0;

        geometry::Ray ray;
        camera->spawnRay(static_cast<float>(x)/width, static_cast<float>(y)/height, ray);

        geometry::Intersection<geometry::Object> intersection;
        
        // light path reverse traversal
        for (uint b = 0; b < BOUNCES; b++)
        {
          depth = b;
          // we hit the sky...
          if (!scene.intersect(ray, intersection))
          {
            emittance[b] = SKY;
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

        radianceSum += radiance;
      }

      this->renderedData->setPixelSRGB(x, y, radianceSum/static_cast<float>(SAMPLES));
      
    }
  }

  this->renderedData->signalChanged();

  puts("done!");
}
