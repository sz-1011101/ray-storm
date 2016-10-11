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
        this->tanFovHalved = std::tan(utility::Math::degToRad(this->fov_degrees)/2.0f);

        // build basic camera vectors
        this->forward = glm::normalize(this->lookAt - this->position);
        this->right = glm::normalize(glm::cross(this->forward, this->up));
        this->down = glm::normalize(glm::cross(this->forward, this->right));
      }
      
      glm::vec3 position;
      glm::vec3 lookAt; 
      glm::vec3 up;
      float aspectRatio;
      float fov_degrees;

      // camera axes, OpenCV style
      glm::vec3 down;
      glm::vec3 right;
      glm::vec3 forward;

      float tanFovHalved;
    };
  }
}

#endif