#ifndef CAMERA_UNIFORM_BUFFER_HPP
#define CAMERA_UNIFORM_BUFFER_HPP

#include "graphics/UniformBuffer.hpp"

#include <glad/glad.h>

class Camera;

class CameraUniformBuffer
{
private:
    UniformBuffer buffer_;

public:
    void Tick(const Camera& camera);
};

#endif // CAMERA_UNIFORM_BUFFER_HPP

