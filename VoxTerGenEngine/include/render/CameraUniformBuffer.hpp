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
    void Initialize();

    void UpdateMatrices(const Camera& camera, float alpha);
};

#endif // CAMERA_UNIFORM_BUFFER_HPP

