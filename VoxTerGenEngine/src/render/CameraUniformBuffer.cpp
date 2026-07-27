#include "render/CameraUniformBuffer.hpp"

#include "graphics/Camera.hpp"

void CameraUniformBuffer::Initialize(GLsizeiptr size, GLuint binding_point)
{
    buffer_.Initialize(size, binding_point);
}

void CameraUniformBuffer::UpdateMatrices(const Camera& camera, float alpha)
{
    CameraMatrices matrices;
 
    matrices.view_ = camera.InterpolatedViewMatrix(alpha);
    matrices.projection_ = camera.ProjectionMatrix();

    buffer_.UploadData(matrices);
}