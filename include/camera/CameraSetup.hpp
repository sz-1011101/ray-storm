#ifndef CAMERA_SETUP_H_
#define CAMERA_SETUP_H_

#include <memory>

namespace ray_storm
{
  namespace camera
  {
    class CameraSetup
    {
    public:
      
      CameraSetup(
        const glm::vec3 &position, 
        const glm::vec3 &lookAt, 
        const glm::vec3 &up,
        float aspectRatio,
        float fov_degrees
      ) : position(position), lookAt(lookAt), up(up),
        aspectRatio(aspectRatio), fov_degrees(fov_degrees)
      {
        this->tanFovHalvedW = std::tan(utility::Math::degToRad(this->fov_degrees)/2.0f);
        this->tanFovHalvedH = std::tan(utility::Math::degToRad(this->fov_degrees/aspectRatio)/2.0f);

        // build basic camera vectors
        this->forward = glm::normalize(this->lookAt - this->position);
        this->right = glm::normalize(glm::cross(this->forward, this->up));
        this->down = glm::normalize(glm::cross(this->forward, this->right));

        glm::vec3 x = this->right;
        glm::vec3 y = this->down;
        glm::vec3 z = this->forward;

        this->cameraMatrix = glm::mat4(
          x.x, y.x, z.x, 0.0f,
          x.y, y.y, z.y, 0.0f,
          x.z, y.z, z.z, 0.0f,
          glm::dot(-x, position), glm::dot(-y, position), glm::dot(-z, position), 1.0f
        );

        this->projMatrix = glm::mat4(
          1.0f, 0.0f, 0.0f, 0.0f,
          0.0f, 1.0f, 0.0f, 0.0f,
          this->tanFovHalvedW, this->tanFovHalvedH, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 1.0f
        );

        this->inverseCameraMatrix = glm::inverse(this->cameraMatrix);
      }

      virtual ~CameraSetup() {};
      
      glm::vec3 position;
      glm::vec3 lookAt; 
      glm::vec3 up;
      float aspectRatio;
      float fov_degrees;

      // camera axes, OpenCV style
      glm::vec3 down;
      glm::vec3 right;
      glm::vec3 forward;

      float tanFovHalvedW;
      float tanFovHalvedH;

      glm::mat4 cameraMatrix;
      glm::mat4 inverseCameraMatrix;

      glm::mat4 projMatrix;
    };

    typedef std::shared_ptr<CameraSetup> CameraSetupPtr;
  }
}

#endif