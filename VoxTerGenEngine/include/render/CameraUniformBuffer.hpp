#ifndef CAMERA_UNIFORM_BUFFER_HPP
#define CAMERA_UNIFORM_BUFFER_HPP

#include "graphics/UniformBuffer.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <glad/glad.h>

class Camera;

struct CameraData
{
    glm::mat4 view_ = glm::mat4(0.0f);
	glm::mat4 projection_ = glm::mat4(0.0f);
    glm::vec3 pos_ = glm::vec3(0.0f);
    float padding_ = 0.0f;
};

class CameraUniformBuffer
{
private:
    UniformBuffer buffer_;

public:
    void Initialize() noexcept;

    void UpdateCameraData(const Camera& camera, float alpha) const noexcept;
};

#endif // CAMERA_UNIFORM_BUFFER_HPP

