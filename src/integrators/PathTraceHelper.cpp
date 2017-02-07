#include "integrators/PathTraceHelper.h"
#include "integrators/PathTraceVertexFunctions.hpp"

const float RUSSIAN_ROULETTE_ALPHA = 0.25f;

const float EXPECTED_BOUNCES = 1.0f/RUSSIAN_ROULETTE_ALPHA;

using namespace ray_storm::integrators;

void PathTraceHelper::randomWalk
(
  scene::Scene *scene,
  const geometry::Ray &initialRay,
  materials::SAMPLE_DIRECTION smplDir,
  random::RandomizationHelper &randHelper,
  RandomWalk &walk
)
{
  walk.vertices.reserve(EXPECTED_BOUNCES);
  const uint32_t maxBounces = EXPECTED_BOUNCES*2;
  geometry::Ray ray = initialRay;

  glm::vec3 Lrefl(1.0f);
  bool absorbed = false;
  int length = 0;

  for (uint32_t b = 0; b < maxBounces; b++) // bounds this loop, biased in theory though
  {
    PathTraceVertex vert(smplDir);
    if (!PathTraceVertexFunctions::intersect(ray, scene, vert))
    {
      break;
    }

    float rr = (b < 2) ? 1.0f : RUSSIAN_ROULETTE_ALPHA;
    if (PathTraceVertexFunctions::isReflecting(vert) &&
      PathTraceVertexFunctions::bounce(randHelper, vert) &&
      (vert.si.delta || randHelper.drawUniformRandom() < rr) &&
      glm::any(glm::greaterThanEqual(vert.bsdf, glm::vec3(0.00001f)))
    )
    { // we do always bounce on in case of dirac delta function...
      const float cosTheta = std::abs(glm::dot(vert.si.n, vert.si.getOut()));
      Lrefl *= vert.si.delta ? vert.bsdf : (1.0f/rr)*vert.bsdf*cosTheta/vert.si.PDF;
      
      vert.cummulative = Lrefl;
      ray = geometry::Ray(vert.si.x, vert.si.getOut());
      // overwrite old vertex
      if (walk.vertices.size() > b)
      {
        walk.vertices[b] = vert;
      }
      else // push new vertex
      {
        walk.vertices.push_back(vert);
      }
      
      length++;
    }
    else
    {
      absorbed = true;
      break;
    }

    if (b == maxBounces - 1)
    {
      absorbed = true;
      break;
    }

  }
  walk.length = length;
  walk.absorbed = absorbed;
}

void PathTraceHelper::naive(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  RandomWalk &walk,
  random::RandomizationHelper &randHelper
)
{
  PathTraceHelper::randomWalk(scene, sampleRay.ray, materials::SAMPLE_DIRECTION::V_TO_L, randHelper, walk);
  const int walkLen = walk.length;
  glm::vec3 L(0.0f);

  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]);
    for (int b = 0; b < walkLen - 1; b++)
    {
      L += walk.vertices[b].cummulative*PathTraceVertexFunctions::emittance(walk.vertices[b + 1]);
    }

    if (!walk.absorbed)
    {
      L += walk.vertices[walkLen - 1].cummulative*scene->sampleSky(walk.vertices[walkLen - 1].si.getOut());
    }
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);

}

void PathTraceHelper::directIllumination(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  RandomWalk &walk,
  random::RandomizationHelper &randHelper
)
{
  PathTraceHelper::randomWalk(scene, sampleRay.ray, materials::SAMPLE_DIRECTION::V_TO_L, randHelper, walk);
  const int walkLen = walk.length;
  glm::vec3 L(0.0f);
  
  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]) + PathTraceHelper::pathDirectLighting(walk, scene, randHelper, false);
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);
}

void PathTraceHelper::directIlluminationBounce(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  RandomWalk &walk,
  random::RandomizationHelper &randHelper
)
{
  // details for the bsdf/luminaire sample weighting 
  // in http://www.cs.cornell.edu/courses/cs6630/2012sp/slides/07pathtr-slides.pdf
  PathTraceHelper::randomWalk(scene, sampleRay.ray, materials::SAMPLE_DIRECTION::V_TO_L, randHelper, walk);
  const int walkLen = walk.length;

  glm::vec3 L(0.0f);
  
  if (walkLen == 0)
  {
    if (!walk.absorbed)
    {
      L = scene->sampleSky(sampleRay.ray.direction);
    }
  }
  else
  {
    L = PathTraceVertexFunctions::emittance(walk.vertices[0]) + pathDirectLightingBounce(walk, scene, randHelper, false);
  }

  camera->gatherSample(sampleRay.xy, L);
  camera->incrementSampleCnt(sampleRay.xy);
}

void PathTraceHelper::lightPathTracing(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  RandomWalk &walk,
  random::RandomizationHelper &randHelper
)
{
  scene::Scene::LuminaireRay lumRay;
  glm::vec3 Le(0.0f); // the light source emittance where we start the light path walk

  if (scene->sampleLuminaireRay(randHelper, lumRay))
  {
    Le = lumRay.emittance/lumRay.randRay.PDF;
    PathTraceHelper::randomWalk(scene, lumRay.randRay.ray, materials::SAMPLE_DIRECTION::L_TO_V, randHelper, walk);
  }
  PathTraceHelper::pathLightPath(Le, walk, camera, scene, randHelper, false);

  camera->incrementSampleCnt(sampleRay.xy);
}

void PathTraceHelper::bidirectional(
  scene::Scene *scene,
  camera::AbstractCamera *camera,
  const camera::SampleRay &sampleRay,
  RandomWalk &eyeWalk,
  RandomWalk &lightWalk,
  random::RandomizationHelper &randHelper
)
{
  // See "Accelerating the bidirectional path tracing algorithm using a dedicated intersection processor"
  PathTraceHelper::randomWalk(scene, sampleRay.ray, materials::SAMPLE_DIRECTION::V_TO_L, randHelper, eyeWalk);
  const int eyeWalkLen = eyeWalk.length;

  if (eyeWalkLen == 0)
  {
    if (!eyeWalk.absorbed)
    {
      camera->gatherSample(sampleRay.xy, scene->sampleSky(sampleRay.ray.direction));
      camera->incrementSampleCnt(sampleRay.xy);
      return;
    }
  } // HACK to get delta-bounce/sky reflection
  else if (eyeWalkLen == 1 && eyeWalk.vertices[0].si.delta && !eyeWalk.absorbed)
  {
    camera->gatherSample(sampleRay.xy, eyeWalk.vertices[0].bsdf*scene->sampleSky(eyeWalk.vertices[0].si.l));
  }

  scene::Scene::LuminaireRay lumRay;
  glm::vec3 Le(0.0f); // the light source emittance where we start the light path walk

  if (scene->sampleLuminaireRay(randHelper, lumRay))
  {
    Le = lumRay.emittance/lumRay.randRay.PDF;
    PathTraceHelper::randomWalk(scene, lumRay.randRay.ray, materials::SAMPLE_DIRECTION::L_TO_V, randHelper, lightWalk);

    camera::SampleRay srLum;
    if (!lumRay.directional && !lumRay.randRay.delta && camera->generateRay(lumRay.randRay.ray.origin, srLum, randHelper) && 
      scene->visible(srLum.ray.origin, lumRay.randRay.ray.origin))
    {
      const float lumDis = glm::distance(srLum.ray.origin, lumRay.randRay.ray.origin);
      camera->gatherSample(srLum.xy, lumRay.emittance/(lumDis*lumDis)*PathTraceHelper::pathWeighting(0, 0));
    }
  }

  // get direct lighting contribution
  camera->gatherSample(sampleRay.xy, PathTraceHelper::pathDirectLightingBounce(eyeWalk, scene, randHelper, true));
  // get contribution by combining paths
  camera->gatherSample(sampleRay.xy, PathTraceHelper::pathPathCombination(Le, eyeWalk, lightWalk, scene));

  // get direct light bounce contribution
  PathTraceHelper::pathLightPath(Le, lightWalk, camera, scene, randHelper, true);

  camera->incrementSampleCnt(sampleRay.xy);

}

glm::vec3 PathTraceHelper::pathDirectLighting(
  const RandomWalk &eyeWalk,
  scene::Scene *scene,
  random::RandomizationHelper &randHelper,
  bool weight
)
{
  glm::vec3 L(0.0f);
  const int eyeWalkLen = eyeWalk.length;

  int ic = 0;
  for (int i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex &eyeVert = eyeWalk.vertices[i];

    if (eyeVert.si.delta)
    {
      continue;
    }

    scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
      eyeVert, scene, randHelper);
      
    if (!lumSample.shadowed)
    {
      const float cosTheta = glm::dot(eyeVert.si.n, lumSample.direction);
      glm::vec3 Ld = PathTraceVertexFunctions::evaluate(lumSample.direction, eyeVert, eyeVert.si.v)*
        cosTheta*lumSample.emittance/lumSample.PDF;

      if (i > 0)
      {
        Ld *= eyeWalk.vertices[i - 1].cummulative;
      }

      if (weight)
      {
        Ld *= PathTraceHelper::pathWeighting(ic + 1, 0);
      }

      L += Ld;
    }
    ic++;
  }
  return L;
}

glm::vec3 PathTraceHelper::pathDirectLightingBounce(
  const RandomWalk &eyeWalk,
  scene::Scene *scene,
  random::RandomizationHelper &randHelper,
  bool weight
)
{
  const int eyeWalkLen = eyeWalk.length;

  glm::vec3 L(0.0f);

  int ic = 0;
  for (int i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex &vert = eyeWalk.vertices[i];

    glm::vec3 LdLum(0.0f); // direct illumination
    glm::vec3 LdBounce(0.0f); // direct illumination of bounce

    if (!vert.si.delta)
    {
      scene::Scene::LuminaireSample lumSample = PathTraceVertexFunctions::sampleLuminaire(
        vert, scene, randHelper);
      
      if (!lumSample.shadowed)
      {
        const float cosTheta = glm::dot(vert.si.n, lumSample.direction);
        const float bsdfLumPDF = PathTraceVertexFunctions::pdf(vert.si.getIn(), vert, lumSample.direction);
        LdLum = PathTraceVertexFunctions::evaluate(lumSample.direction, vert, vert.si.v)
          *cosTheta*lumSample.emittance/(lumSample.PDF + bsdfLumPDF);
      }
    }

    if (i < eyeWalkLen - 1)
    {
      const float cosTheta = glm::dot(vert.si.n, vert.si.l);
      const PathTraceVertex &nextVert = eyeWalk.vertices[i + 1];
      const float lumBouncePDF = PathTraceVertexFunctions::luminarePDF(vert.si.x, nextVert, scene);
      LdBounce = PathTraceVertexFunctions::emittance(nextVert)*vert.bsdf*cosTheta/(vert.si.PDF + lumBouncePDF);
    }
    else if (!eyeWalk.absorbed) // last vertex
    {
      const float cosTheta = glm::dot(vert.si.n, vert.si.l);
      LdBounce = scene->sampleSky(vert.si.l)*vert.bsdf*cosTheta/(vert.si.PDF + scene->getSkyPDF());
    }
    glm::vec3 Ld = LdLum + LdBounce;

    if (i > 0)
    {
      Ld *= eyeWalk.vertices[i - 1].cummulative;
    }

    if (weight)
    {
      Ld *= PathTraceHelper::pathWeighting(ic + 1, 0);
    }

    L += Ld;

    // this is strange ...?
    if (!vert.si.delta)
    {
      ic++;
    }
  }

  return L;
}

glm::vec3 PathTraceHelper::pathPathCombination(
  const glm::vec3 &Le,
  const RandomWalk &eyeWalk,
  const RandomWalk &lightWalk,
  scene::Scene *scene
)
{
  const int eyeWalkLen = eyeWalk.length;
  const int lightWalkLen = lightWalk.length;

  glm::vec3 L(0.0f);

  int ic = 0;
  for (int i = 0; i < eyeWalkLen; i++)
  {
    const PathTraceVertex &eyeVert = eyeWalk.vertices[i];

    if (eyeVert.si.delta)
    {
      continue;
    }

    int jc = 0;
    for (int j = 0; j < lightWalkLen; j++)
    {
      const PathTraceVertex &lightVert = lightWalk.vertices[j];

      if (lightVert.si.delta)
      {
        continue;
      }

      if (scene->visible(eyeVert.si.x, lightVert.si.x, lightVert.si.n))
      {
        L += Le*PathTraceHelper::pathRadiance(eyeWalk, lightWalk, i, j)*PathTraceHelper::pathWeighting(ic + 1, jc + 1);
      }
      jc++;
    }
    ic++;
  }
  return L;
}

void PathTraceHelper::pathLightPath(
  const glm::vec3 &Le,
  const RandomWalk &lightWalk,
  camera::AbstractCamera *camera,
  scene::Scene *scene,
  random::RandomizationHelper &randHelper,
  bool weight
)
{
  const int lightWalkLen = lightWalk.length;

  int jc = 0;
  for (int j = 0; j < lightWalkLen; j++)
  {
    const PathTraceVertex &lightVert = lightWalk.vertices[j];

    if (lightVert.si.delta)
    {
      continue;
    }

    camera::SampleRay sr;
    if (camera->generateRay(lightVert.si.x, sr, randHelper) && scene->visible(sr.ray.origin, lightVert.si.x, lightVert.si.n))
    {
      glm::vec3 Lc = Le;
      glm::vec3 l2c = sr.ray.origin - lightVert.si.x;
      const float l2clen = glm::length(l2c);
      const float l2clenSquared = l2clen*l2clen;
      l2c /= l2clen; // normalize

      if (l2clenSquared >= 0.05f)
      {
        if (j > 0)
        {
          Lc *= lightWalk.vertices[j - 1].cummulative;
        }

        Lc *= std::abs(glm::dot(lightVert.si.n, l2c));
        Lc *= PathTraceVertexFunctions::evaluate(lightVert.si.l, lightVert, l2c)/l2clenSquared;
        if (weight)
        {
          Lc *= PathTraceHelper::pathWeighting(0, jc + 1);
        }
        camera->gatherSample(sr.xy, Lc);
      }
    }
    jc++;
  }
}

glm::vec3 PathTraceHelper::pathRadiance(
  const RandomWalk &eyeWalk,
  const RandomWalk &lightWalk,
  int eyeLen,
  int lightLen
)
{
  // See "Accelerating the bidirectional path tracing algorithm using a dedicated intersection processor"
  const PathTraceVertex &eyeVert = eyeWalk.vertices[eyeLen];
  const PathTraceVertex &lightVert = lightWalk.vertices[lightLen];

  glm::vec3 L(1.0f);

  if (eyeLen > 0)
  {
    L *= eyeWalk.vertices[eyeLen - 1].cummulative;
  }
  if (lightLen > 0)
  {
    L *= lightWalk.vertices[lightLen - 1].cummulative;
  }
  glm::vec3 e2l = lightVert.si.x - eyeVert.si.x;
  float e2llen = glm::length(e2l);
  float e2llensquared = e2llen*e2llen;
  e2l = glm::normalize(e2l);

  if (e2llensquared < 0.05f)
  {
    return glm::vec3(0.0f);
  }

  float G = std::abs(glm::dot(e2l, eyeVert.si.n))*std::abs(glm::dot(-e2l, lightVert.si.n))/e2llensquared;

  L *= PathTraceVertexFunctions::evaluate(e2l, eyeVert, eyeVert.si.v);
  L *= G;
  L *= PathTraceVertexFunctions::evaluate(lightVert.si.l, lightVert, -e2l);

  return L;
}

float PathTraceHelper::pathWeighting(int eyeIndex, int lightIndex)
{
  return 1.0f/(1.0f + eyeIndex + lightIndex);
  //return lightIndex == 3 && eyeIndex == 0 ? 1.0f : 0.0f;
}