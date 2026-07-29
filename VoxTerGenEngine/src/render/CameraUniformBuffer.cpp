#include "render/CameraUniformBuffer.hpp"

#include "graphics/Camera.hpp"

void CameraUniformBuffer::Initialize() noexcept
{
    constexpr GLuint binding_point = 0;

    buffer_.Initialize(sizeof(CameraMatrices), binding_point);
}

void CameraUniformBuffer::UpdateMatrices(const Camera& camera, float alpha) const noexcept
{
    CameraMatrices matrices;
 
    matrices.view_ = camera.InterpolatedViewMatrix(alpha);
    matrices.projection_ = camera.ProjectionMatrix();

    buffer_.UploadData(matrices);
}