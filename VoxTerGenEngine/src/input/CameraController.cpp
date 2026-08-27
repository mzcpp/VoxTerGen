#include "input/CameraController.hpp"

#include "core/Direction.hpp"

#include "graphics/Camera.hpp"

#include "input/InputManager.hpp"

#include "physics/DigitalDifferentialAnalyzer.hpp"

#include "utils/MathUtils.hpp"

#include "world/ChunkManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>
#include <cassert>

namespace dda = digital_differential_analyzer;

CameraController::CameraController(Camera& camera) :
    camera_(camera),
    movement_speed_(constants::camera::movement_speed), 
    move_sensitivity_(constants::camera::move_sensitivity), 
    zoom_sensitivity_(constants::camera::zoom_sensitivity), 
    move_dir_(0.0), 
    mouse_delta_(0.0f), 
    mouse_wheel_(0.0f)
{   
}

void CameraController::GatherInput(const InputManager& input_manager)
{
    move_dir_ = glm::vec3(0.0);

    if (input_manager.KeyDown(SDL_SCANCODE_W))
    {
        move_dir_ += camera_.front_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_S))
    {
        move_dir_ -= camera_.front_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_A))
    {
        move_dir_ -= camera_.right_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_D))
    {
        move_dir_ += camera_.right_;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_SPACE))
    {
        move_dir_ += constants::math::world_up;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_LCTRL))
    { 
        move_dir_ -= constants::math::world_up;
    }

    mouse_delta_ = input_manager.MouseDelta();
    mouse_wheel_ = input_manager.MouseWheel();
}

void CameraController::Tick(const ChunkManager& chunk_manager)
{
    const double max_distance = 6.0;

    CastRay(max_distance, chunk_manager);
}

void CameraController::ApplyChanges(double frame_dt)
{
    if (camera_.enabled_rotation_)
    {
        ApplyMouseRotation();
    }

    if (camera_.enabled_zoom_)
    {
        ApplyZoom();
    }

    if (camera_.enabled_movement_)
    {
        ApplyKeyboardInput(frame_dt);
    }
}

void CameraController::ApplyKeyboardInput(double frame_dt)
{
    assert(camera_.enabled_movement_);

    camera_.prev_position_ = camera_.position_;

    if (glm::length2(move_dir_) > 0.0f)
    {
        move_dir_ = glm::normalize(move_dir_);
        
        camera_.position_ += move_dir_ * movement_speed_ * frame_dt;
        camera_.stale_ = true;
    }
}

void CameraController::ApplyMouseRotation()
{
    assert(camera_.enabled_rotation_);

    camera_.prev_yaw_ = camera_.yaw_;
    camera_.prev_pitch_ = camera_.pitch_;

    if (math_utils::FloatingPointNearZero(mouse_delta_.x) && math_utils::FloatingPointNearZero(mouse_delta_.y))
    {
        return;
    }

    camera_.yaw_ += mouse_delta_.x * move_sensitivity_;
    camera_.pitch_ = std::clamp(camera_.pitch_ + (mouse_delta_.y * move_sensitivity_), constants::camera::pitch_min, constants::camera::pitch_max);
    camera_.stale_ = true;
}

void CameraController::ApplyZoom()
{
    assert(camera_.enabled_zoom_);
    
    if (math_utils::FloatingPointNearZero(mouse_wheel_))
    {
        return;
    }

    camera_.zoom_ = std::clamp(camera_.zoom_ - (mouse_wheel_ * zoom_sensitivity_), constants::camera::zoom_min, constants::camera::zoom_max);
    camera_.stale_ = true;
}

void CameraController::CastRay(double max_distance, const ChunkManager& chunk_manager)
{
    const auto world_block_query = [this, &chunk_manager](glm::ivec3 coords) {
        const glm::ivec3 camera_block_pos = chunk_manager.RelativeBlockPos(camera_.Pos());
        const glm::ivec2 chunk_coords = chunk_manager.GetChunkCoords(camera_block_pos);
        
        return chunk_manager.WorldBlockQuery(chunk_coords, coords);
    };

    camera_.raycast_result_ = dda::CastRay(camera_.Pos(), camera_.Front(), max_distance, world_block_query);
}