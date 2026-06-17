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

ObserverController::ObserverController(Observer& observer) : observer_(observer), noclip_(true)
{
}

void ObserverController::GatherInput(const InputManager& input_manager)
{

}

void ObserverController::Tick(const InputManager& input_manager, const CollisionSystem& collision_system, const ChunkManager& chunk_manager, Camera& camera)
{
    // TODO CAMERA POSITION DOUBLE?!
    // TODO JUMPING BELOW A BLOCK COLLIDES AND CAMERA DOES NOT PENETRATE! Because you moved the pos_offset to height / 4.0
    // 
    //std::cout << static_cast<int>(observer_.GetMovementState()) << '\n';
    /*std::cout << "------------------------------------------------------------" << '\n';
    std::cout << "POSITION" << '\n';
    std::cout << "Relative Block " << observer_.RelativeBlockPos(constants::observer::pos_offset).x << ' ' << observer_.RelativeBlockPos(constants::observer::pos_offset).y << ' ' << observer_.RelativeBlockPos(constants::observer::pos_offset).z << ' ' << '\n';
    std::cout << "Absolute Block " << observer_.AbsoluteBlockPos(constants::observer::pos_offset).x << ' ' << observer_.AbsoluteBlockPos(constants::observer::pos_offset).y << ' ' << observer_.AbsoluteBlockPos(constants::observer::pos_offset).z << ' ' << '\n';
    std::cout << "Observer Pos " << observer_.Pos().x << ' ' << observer_.Pos().y << ' ' << observer_.Pos().z << ' ' << '\n';
    */

    glm::dvec3 displacement_vector(0.0);
    const glm::dvec3 direction_vector = GetDirectionVector(input_manager);

    auto block_below = GetBlockInfoBelowObserver(chunk_manager);
    //std::cout << static_cast<int>(block_below.block_.Type()) << '\n';

    if (noclip_)
    {
        displacement_vector = GetDisplacementVector(direction_vector);
    }
    else
    {
        //observer_.velocity_ = GetHorizontalVelocityVector(direction_vector);

        observer_.velocity_.x = direction_vector.x * constants::observer::movement_speed;
        observer_.velocity_.y += 0.0;
        observer_.velocity_.z = direction_vector.z * constants::observer::movement_speed;
        
        //if (!observer_.Grounded())
        {
            observer_.velocity_.y += constants::physics::gravity * constants::engine::tick_dt;
        }
            
        displacement_vector.x = observer_.velocity_.x * constants::engine::tick_dt;
        displacement_vector.y = observer_.velocity_.y * constants::engine::tick_dt;
        displacement_vector.z = observer_.velocity_.z * constants::engine::tick_dt;

        const double desired_y = displacement_vector.y;

        displacement_vector = GetClippedDisplacementVector(displacement_vector, input_manager, collision_system, chunk_manager);

        if (displacement_vector.y > desired_y)
        {
            observer_.SetMovementState(MovementState::GROUNDED);
            observer_.velocity_.y = 0.0;
        }
    }
        
    ApplyDisplacementVector(displacement_vector, camera);
    ApplyMouseRotation(input_manager.MouseDelta(), camera);
}

void ObserverController::Apply(Camera& camera)
{
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
        else
        {
            // JUMP
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
    const double displacement_multiplier = constants::observer::movement_speed * constants::engine::tick_dt;
    
    return { 
        dir_vec.x * displacement_multiplier, 
        dir_vec.y * displacement_multiplier, 
        dir_vec.z * displacement_multiplier 
    };
}

glm::dvec3 ObserverController::GetClippedDisplacementVector(glm::dvec3 result_displacement_vector, const InputManager& input_manager, const CollisionSystem& collision_system, const ChunkManager& chunk_manager) const
{
    result_displacement_vector = collision_system.GetClippedDisplacementVector([this, &chunk_manager](glm::ivec3 coords)
        {
            const glm::ivec3 observer_offset_block_pos = observer_.AbsoluteBlockPos(constants::observer::pos_offset);
            const glm::ivec2 chunk_coords = chunk_manager.GetChunkCoords(observer_offset_block_pos);
            return chunk_manager.WorldBlockQuery(chunk_coords, coords);
        },
        observer_, result_displacement_vector);

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
    const glm::ivec2 chunk_coords = chunk_manager.GetChunkCoords(observer_.AbsoluteBlockPos(constants::observer::pos_offset));
    
    glm::ivec3 block_pos_below_observer = observer_.RelativeBlockPos(constants::observer::pos_offset);
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