#include "render/CameraUniformBuffer.hpp"

void CameraUniformBuffer::Tick(const Camera& camera)
{
    buffer_.UploadData(camera.Matrices());
}