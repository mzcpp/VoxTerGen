#include "input/ObserverController.hpp"
#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"
#include "utils/MathUtils.hpp"
#include "physics/CollisionSystem.hpp"
#include "world/ChunkManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>

ObserverController::ObserverController(Observer& observer) : 
    observer_(observer), 
    noclip_(true), 
    jump_requested_(false), 
    horizontal_input_direction_(0.0), 
    input_direction_(0.0), 
    mouse_delta_(0.0f)
{
}

void ObserverController::GatherInput(const InputManager& input_manager)
{
    if (input_manager.KeyPressed(SDL_SCANCODE_F))
    {
        ToggleNoclip();
    }

    if (input_manager.KeyDown(SDL_SCANCODE_SPACE) && !noclip_)
    {
        jump_requested_ = true;
    }

    input_direction_ = GetDirectionVector(input_manager);
    horizontal_input_direction_ = GetHorizontalDirectionVector(input_manager);
    mouse_delta_ = input_manager.MouseDelta();
}

void ObserverController::Tick(const CollisionSystem& collision_system, const ChunkManager& chunk_manager, Camera& camera)
{
    const MovementState movement_state = observer_.GetMovementState();
    glm::dvec3 displacement_vector(0.0);

    if (noclip_)
    {
        displacement_vector = GetDisplacementVector(input_direction_);
    }
    else
    {
        glm::dvec3 acc_vec(0.0);

        if (glm::length2(input_direction_) == 0.0)
        {
            if (movement_state == MovementState::GROUNDED)
            {
                DecayObserverHorizontalVelocity(observer_.walk_friction_);
            }
            else if (movement_state == MovementState::AIRBORNE)
            {
                DecayObserverHorizontalVelocity(observer_.air_friction_);
            }
        }
        else
        {
            acc_vec = horizontal_input_direction_ * observer_.walk_acceleration_;
        }

        acc_vec.y += constants::physics::gravity;

        if (jump_requested_ && movement_state == MovementState::GROUNDED)
        {
            observer_.velocity_.y = observer_.jump_velocity_;
            observer_.SetMovementState(MovementState::AIRBORNE);
        }

        observer_.velocity_ += acc_vec * constants::engine::tick_dt;
        ClampObserverHorizontalVelocity(observer_.max_walk_speed_);
        
        displacement_vector = observer_.velocity_ * constants::engine::tick_dt;

        const double desired_y = displacement_vector.y;

        displacement_vector = GetClippedDisplacementVector(displacement_vector, collision_system, chunk_manager);

        if (displacement_vector.y > desired_y)
        {
            observer_.SetMovementState(MovementState::GROUNDED);
            observer_.velocity_.y = 0.0;
        }
        else if (displacement_vector.y < desired_y)
        {
            observer_.velocity_.y = 0.0;
        }
        else
        {
            // SWIMMING LATER
            observer_.SetMovementState(MovementState::AIRBORNE);
        }
    }

    ApplyDisplacementVector(displacement_vector, camera);
    jump_requested_ = false;
}

void ObserverController::ApplyChanges(Camera& camera)
{
    ApplyMouseRotation(mouse_delta_, camera);
}

glm::dvec3 ObserverController::GetDirectionVector(const InputManager& input_manager) const
{
    glm::dvec3 dir_vec(0.0);

    if (input_manager.KeyDown(SDL_SCANCODE_W))
    {
       dir_vec += observer_.Front();
    }

    if (input_manager.KeyDown(SDL_SCANCODE_S))
    {
       dir_vec -= observer_.Front();
    }

    if (input_manager.KeyDown(SDL_SCANCODE_A))
    {
       dir_vec -= observer_.Right();
    }

    if (input_manager.KeyDown(SDL_SCANCODE_D))
    {
       dir_vec += observer_.Right();
    }

    if (input_manager.KeyDown(SDL_SCANCODE_SPACE))
    {
        if (noclip_)
        {
            dir_vec += constants::math::world_up;
        }
    }

    if (input_manager.KeyDown(SDL_SCANCODE_LCTRL))
    {
       dir_vec -= constants::math::world_up;
    }

    if (glm::length2(dir_vec) > 0.0)
    {
       dir_vec = glm::normalize(dir_vec);
    }

    return dir_vec;
}

glm::dvec3 ObserverController::GetHorizontalDirectionVector(const InputManager& input_manager) const
{
    glm::dvec3 dir_vec(0.0);
    glm::dvec3 horizontal_vec(0.0);

    if (input_manager.KeyDown(SDL_SCANCODE_W))
    {
        horizontal_vec = observer_.Front();
        horizontal_vec.y = 0.0;
        dir_vec += horizontal_vec;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_S))
    {
        horizontal_vec = observer_.Front();
        horizontal_vec.y = 0.0;
        dir_vec -= horizontal_vec;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_A))
    {
        horizontal_vec = observer_.Right();
        horizontal_vec.y = 0.0;
        dir_vec -= horizontal_vec;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_D))
    {
        horizontal_vec = observer_.Right();
        horizontal_vec.y = 0.0;
        dir_vec += horizontal_vec;
    }

    if (glm::length2(dir_vec) > 0.0)
    {
        dir_vec = glm::normalize(dir_vec);
    }

    return dir_vec;
}

glm::dvec3 ObserverController::GetHorizontalVelocityVector(glm::dvec3 dir_vec) const noexcept
{
    const double velocity_multiplier = constants::observer::movement_speed;

    return { 
        dir_vec.x * velocity_multiplier, 
        0.0, 
        dir_vec.z * velocity_multiplier 
    };
}

glm::dvec3 ObserverController::GetDisplacementVector(glm::dvec3 dir_vec) const noexcept
{
    return dir_vec * constants::observer::movement_speed * constants::engine::tick_dt;
}

glm::dvec3 ObserverController::GetClippedDisplacementVector(glm::dvec3 result_displacement_vector, const CollisionSystem& collision_system, const ChunkManager& chunk_manager) const
{
    const auto world_block_query = [this, &chunk_manager](glm::ivec3 coords) {
        const glm::ivec3 observer_offset_block_pos = chunk_manager.AbsoluteBlockPos(observer_.Pos(), constants::observer::pos_offset);
        const glm::ivec2 chunk_coords = chunk_manager.GetChunkCoords(observer_offset_block_pos);
        
        return chunk_manager.WorldBlockQuery(chunk_coords, coords);
    }
    
    result_displacement_vector = collision_system.GetClippedDisplacementVector(world_block_query, observer_, result_displacement_vector);

    return result_displacement_vector;
}

void ObserverController::ApplyKeyboardInput(const InputManager& input_manager, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }
    
    const glm::dvec3 dir_vec = GetDirectionVector(input_manager);

    if (glm::length2(dir_vec) > 0.0)
    {
        observer_.position_ += GetDisplacementVector(dir_vec);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyDirectionVector(glm::dvec3 dir_vec, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }

    if (glm::length2(dir_vec) > 0.0)
    {
        observer_.position_ += GetDisplacementVector(dir_vec);
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_);
            camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyDisplacementVector(glm::dvec3 displacement_vec, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }

    if (glm::length2(displacement_vec) > 0.0)
    {
        observer_.position_ += displacement_vec;
        observer_.moving_ = true;
        
        if (!camera.EnabledMovement())
        {
            camera.SetPos(observer_.position_ + camera.FPSOffset());
            camera.SetStale(true);
        }
    }
}

void ObserverController::ApplyMouseRotation(glm::vec2 mouse_delta, Camera& camera)
{
    observer_.prev_yaw_ = observer_.yaw_;
    observer_.prev_pitch_ = observer_.pitch_;

    if (!camera.EnabledRotation())
    {
        camera.SetPrevYaw(observer_.prev_yaw_);
        camera.SetPrevPitch(observer_.prev_pitch_);
    }

    if (math_utils::FloatingPointNearZero(mouse_delta.x) && math_utils::FloatingPointNearZero(mouse_delta.y))
    {
        return;
    }

    observer_.yaw_ += mouse_delta.x * constants::observer::move_sensitivity;
    observer_.pitch_ = std::clamp(observer_.pitch_ + (mouse_delta.y * constants::observer::move_sensitivity), constants::observer::pitch_min, constants::observer::pitch_max);
    observer_.moving_ = true;

    if (!camera.EnabledRotation())
    {
        camera.SetYaw(observer_.yaw_);
        camera.SetPitch(observer_.pitch_);
        camera.SetStale(true);
    }
}

BlockInfo ObserverController::GetBlockInfoBelowObserver(const ChunkManager& chunk_manager)
{
    const glm::ivec2 chunk_coords = chunk_manager.GetChunkCoords(chunk_manager.AbsoluteBlockPos(observer_.Pos(), constants::observer::pos_offset));
    
    glm::ivec3 block_pos_below_observer = chunk_manager.RelativeBlockPos(observer_.Pos(), constants::observer::pos_offset);
    --block_pos_below_observer.y;

    return chunk_manager.WorldBlockQuery(chunk_coords, block_pos_below_observer);
}

void ObserverController::ToggleNoclip()
{
    noclip_ = !noclip_;
    observer_.velocity_ = glm::dvec3(0.0);

    if (noclip_)
    {
        observer_.movement_state_ = MovementState::AIRBORNE;
    }
}

void ObserverController::DecayObserverHorizontalVelocity(double friction)
{
    const glm::dvec2 horizontal_velocity = { observer_.velocity_.x, observer_.velocity_.z };
    const double horizontal_speed = glm::length(horizontal_velocity);

    if (horizontal_speed > 0.0)
    {
        const glm::dvec2 horizontal_dir = horizontal_velocity / horizontal_speed;
        const double new_horizontal_speed = std::fmax(0, horizontal_speed - friction * constants::engine::tick_dt);
        observer_.velocity_.x = horizontal_dir.x * new_horizontal_speed;
        observer_.velocity_.z = horizontal_dir.y * new_horizontal_speed;
    }
}

void ObserverController::ClampObserverHorizontalVelocity(double max)
{
    const glm::dvec2 horizontal_velocity = { observer_.velocity_.x, observer_.velocity_.z };
    const double horizontal_speed = glm::length(horizontal_velocity);

    if (horizontal_speed > max)
    {
        const glm::dvec2 horizontal_dir = horizontal_velocity / horizontal_speed;
        observer_.velocity_.x = horizontal_dir.x * max;
        observer_.velocity_.z = horizontal_dir.y * max;
    }
}
