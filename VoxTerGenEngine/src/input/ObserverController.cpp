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
    ApplyDisplacementVector(GetClippedDisplacementVector(input_manager, collision_system, world), camera);
    ApplyMouseRotation(input_manager.MouseDelta(), camera);
}

glm::dvec3 ObserverController::GetDirectionVector(const InputManager& input_manager) const
{
    glm::dvec3 dir_vec(0.0f);

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
       dir_vec += constants::math::world_up;
    }

    if (input_manager.KeyDown(SDL_SCANCODE_LCTRL))
    {
       dir_vec -= constants::math::world_up;
    }

    if (glm::length2(dir_vec) > 0.0f)
    {
       dir_vec = glm::normalize(dir_vec);
    }

    return dir_vec;
}

glm::dvec3 ObserverController::GetDisplacementVector(glm::dvec3 dir_vec) const
{
    const double multiplier = constants::observer::movement_speed * constants::engine::tick_dt;
    return { dir_vec.x * multiplier, dir_vec.y * multiplier, dir_vec.z * multiplier };
}

glm::dvec3 ObserverController::GetClippedDisplacementVector(const InputManager& input_manager, const CollisionSystem& collision_system, const World& world) const
{
    const glm::dvec3 direction_vector = GetDirectionVector(input_manager);
    glm::dvec3 displacement_vector = GetDisplacementVector(direction_vector);

    displacement_vector = collision_system.GetClippedDisplacementVector([this, &world](glm::ivec3 coords)
        {
            const glm::ivec2 chunk_coords = world.ChunkManagerRef().GetChunkCoords(observer_.AbsoluteBlockPos(constants::observer::pos_offset));
            return world.ChunkManagerRef().WorldBlockQuery(chunk_coords, coords);
        },
        observer_, displacement_vector);

    return displacement_vector;
}

void ObserverController::ApplyKeyboardInput(const InputManager& input_manager, Camera& camera)
{
    observer_.prev_position_ = observer_.position_;

    if (!camera.EnabledMovement())
    {
        camera.SetPrevPos(camera.Pos());
    }
    
    const glm::dvec3 dir_vec = GetDirectionVector(input_manager);

    if (glm::length2(dir_vec) > 0.0f)
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

    if (glm::length2(dir_vec) > 0.0f)
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

    if (glm::length2(displacement_vec) > 0.0f)
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

    // more TODO later (WoW style camera) 
}