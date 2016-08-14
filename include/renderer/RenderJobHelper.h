#ifndef RENDER_JOB_HELPER_H_
#define RENDER_JOB_HELPER_H_

#include <vector>

#include "utility/common.hpp"
#include "renderer/RenderJob.hpp"

namespace ray_storm
{
  namespace renderer
  {

    typedef std::vector<RenderJob> RenderJobVector;

    class RenderJobHelper
    {
    public:
      
      static void makeRenderJobs(uint32_t width, uint32_t height, uint32_t jobCntPerDimension, RenderJobVector &jobs);

    private:

      RenderJobHelper() {};
      
    };

  }

}


#endif