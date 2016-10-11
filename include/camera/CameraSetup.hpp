#ifndef CAMERA_SETUP_H_
#define CAMERA_SETUP_H_

namespace ray_storm
{
  namespace camera
  {
    struct CameraSetup
    {

      CameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees
      ) : position(position), lookAt(lookAt), up(up),
        aspectRatio(aspectRatio), fov_degrees(fov_degrees)
      {

      }
      
      glm::vec3 position;
      glm::vec3 lookAt; 
      glm::vec3 up;
      float aspectRatio;
      float fov_degrees;
    };
  }
}

#endif