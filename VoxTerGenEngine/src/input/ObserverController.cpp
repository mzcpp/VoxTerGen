#include "input/ObserverController.hpp"
#include "input/InputManager.hpp"
#include "graphics/Camera.hpp"
#include "utils/Constants.hpp"
#include "utils/MathUtils.hpp"
#include "physics/CollisionSystem.hpp"
#include "world/World.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <SDL2/SDL.h>

#include <algorithm>

ObserverController::ObserverController(Observer& observer) : observer_(observer)
{
}

void ObserverController::Tick(const InputManager& input_manager, const CollisionSystem& collision_system, const World& world, Camera& camera)
{
    glm::dvec3 displacement_vector(0.0);
    const glm::dvec3 direction_vector = GetDirectionVector(input_manager);

    if (observer_.Noclip())
    {
        displacement_vector = GetDisplacementVector(direction_vector);
    }
    else
    {
        observer_.velocity_ = GetHorizontalVelocityVector(direction_vector);
        
        if (!observer_.Grounded())
        {
            observer_.velocity_.y += constants::physics::gravity * constants::engine::tick_dt;
        }
            
        displacement_vector.x = observer_.velocity_.x * constants::engine::tick_dt;
        displacement_vector.y = observer_.velocity_.y * constants::engine::tick_dt;
        displacement_vector.z = observer_.velocity_.z * constants::engine::tick_dt;

        const double desired_y = displacement_vector.y;

        displacement_vector = GetClippedDisplacementVector(displacement_vector, input_manager, collision_system, world);

        if (displacement_vector.y < desired_y)
        {
            observer_.SetMovementState(MovementState::GROUNDED);
            //observer_.velocity_.y = 0.0;
        }
    }
        
    ApplyDisplacementVector(displacement_vector, camera);
    ApplyMouseRotation(input_manager.MouseDelta(), camera);
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
        if (observer_.Noclip())
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

glm::dvec3 ObserverController::GetClippedDisplacementVector(glm::dvec3 result_displacement_vector, const InputManager& input_manager, const CollisionSystem& collision_system, const World& world) const
{
    result_displacement_vector = collision_system.GetClippedDisplacementVector([this, &world](glm::ivec3 coords)
        {
            const glm::ivec2 chunk_coords = world.ChunkManagerRef().GetChunkCoords(observer_.AbsoluteBlockPos(constants::observer::pos_offset));
            return world.ChunkManagerRef().WorldBlockQuery(chunk_coords, coords);
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