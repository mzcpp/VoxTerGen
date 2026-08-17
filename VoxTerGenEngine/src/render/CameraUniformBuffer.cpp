#include "render/CameraUniformBuffer.hpp"

#include "graphics/Camera.hpp"

void CameraUniformBuffer::Initialize() noexcept
{
    constexpr GLuint binding_point = 0;

    buffer_.Initialize(sizeof(CameraData), binding_point);
}

void CameraUniformBuffer::UpdateMatrices(const Camera& camera, float alpha) const noexcept
{
    CameraData camera_data;
 
    camera_data.view_ = camera.InterpolatedViewMatrix(alpha);
    camera_data.projection_ = camera.ProjectionMatrix();
    camera_data.pos_ = glm::vec3(camera.Pos());

    buffer_.UploadData(camera_data);
}