#include "renderer/RenderJobHelper.h"

using namespace ray_storm::renderer;

void RenderJobHelper::makeRenderJobs(uint32_t width, uint32_t height, uint32_t jobCntPerDimension, RenderJobVector &jobs)
{
  // compute how large the job tiles are
  int xTileWidth = static_cast<int>(std::ceil(static_cast<float>(width)/jobCntPerDimension));
  int yTileHeight= static_cast<int>(std::ceil(static_cast<float>(height)/jobCntPerDimension));

  // compute how many we can actually create
  uint32_t xRealJobs = static_cast<uint32_t>(std::ceil(static_cast<float>(width)/xTileWidth));
  uint32_t yRealJobs = static_cast<uint32_t>(std::ceil(static_cast<float>(height)/yTileHeight));

  jobs.reserve(xRealJobs*yRealJobs);
  uint32_t id = 0;
  for (uint32_t  y = 0; y < yRealJobs; y++)
  {
    for (uint32_t x = 0; x < xRealJobs; x++)
    {
      int xOrg = x*xTileWidth;
      int yOrg = y*yTileHeight;

      int xOvershoot = static_cast<int>(std::max(0, xOrg + xTileWidth - static_cast<int>(width)));
      int yOvershoot = static_cast<int>(std::max(0, yOrg + yTileHeight - static_cast<int>(height)));

      RenderJob job(id++, static_cast<uint32_t>(xOrg), static_cast<uint32_t>(yOrg), 
        static_cast<uint32_t>(xTileWidth - xOvershoot), static_cast<uint32_t>(yTileHeight - yOvershoot));

      jobs.emplace_back(job);
    }
  }

}