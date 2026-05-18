#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "utils/Constants.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <array>

/**
 * @brief Represents a plane in 3D space.
 *
 * A Plane is defined by a normal vector and a distance from the origin.
 * It is primarily used for view-frustum representation and culling.
 */
struct Plane
{
	glm::vec3 normal_;
	float dist_;
};

class CameraController;

/**
 * @brief Represents a 3D camera with position, orientation, and frustum.
 *
 * The Camera class manages the camera's position, orientation (yaw/pitch),
 * view and projection matrices, frustum planes for culling, and provides
 * functionality for interpolation and movement simulation.
 */
class Camera
{
	friend class CameraController;

private:
	glm::dvec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;

	float yaw_;
	float pitch_;
	float zoom_;

	float near_plane_;
	float far_plane_;

	glm::mat4 view_;
	glm::mat4 projection_;
	glm::mat4 view_proj_;
	
	std::array<Plane, 6> frustum_planes_;
	
	glm::dvec3 prev_position_;
	float prev_yaw_;
	float prev_pitch_;

    glm::dvec3 fps_offset_;

	bool enabled_movement_;
    bool enabled_rotation_;
    bool enabled_zoom_;
	bool stale_;

public:
    /**
     * @brief Constructs a Camera with initial position, up vector, yaw, and pitch.
     *
     * @param position Initial position in world space
     * @param yaw Initial yaw angle in degrees
     * @param pitch Initial pitch angle in degrees
     */
    Camera(
        glm::dvec3 position = constants::geometry::spawn_point, 
        float yaw = constants::camera::yaw, 
        float pitch = constants::camera::pitch);

    Camera(const Camera& other) = delete;
    Camera& operator=(const Camera& other) = delete;

    Camera(Camera&& other) = delete;
    Camera& operator=(Camera&& other) = delete;

    /**
     * @brief Updates the camera state for the current frame.
     *
     * @param aspect_ratio The aspect ratio of the viewport
     */
    void Tick(float aspect_ratio);

    /**
     * @brief Returns an interpolated view matrix between previous and current camera states.
     *
     * @param alpha Interpolation factor (0 = previous, 1 = current)
	 * 
     * @return Interpolated view matrix
     */
    glm::mat4 InterpolatedViewMatrix(float alpha) const;

    /**
     * @brief Updates the camera's view, projection, and view-projection matrices.
     *
     * @param aspect_ratio The aspect ratio of the viewport
     */
    void UpdateSimulationMatrices(float aspect_ratio);

    /**
     * @brief Updates the six frustum planes from the current view-projection matrix.
     */
    void UpdateFrustumPlanes();

    /**
     * @brief Checks whether a point is inside the camera's view frustum.
     *
     * @param point Point in world space
	 * 
     * @return True if the point is inside the frustum, false otherwise
     */
    bool PointInsideFrustum(const glm::vec3& point) const;

    /**
     * @brief Performs a sanity check on the frustum.
     *
     * @return True if the frustum is valid, false otherwise
     */
    bool SanityCheckFrustum() const;

    /**
     * @brief Prints the camera's position, orientation, and vectors to the logger.
     */
    void LogCameraData() const;

    /**
     * @brief Prints the six frustum planes to the logger.
     */
    void PrintFrustumPlanes() const;
	
	// Getters
	glm::mat4 ViewMatrix() const noexcept { return view_; }
	glm::mat4 ProjectionMatrix() const noexcept { return projection_; }
	glm::dvec3 Pos() const noexcept { return position_; }
	glm::vec3 Front() const noexcept { return front_; }
	glm::vec3 Up() const noexcept { return up_; }
	glm::vec3 Right() const noexcept { return right_; }
	float Yaw() const noexcept { return yaw_; }
	float Pitch() const noexcept { return pitch_; }
	float Zoom() const noexcept { return zoom_; }
	const std::array<Plane, 6>& GetFrustumPlanes() const noexcept { return frustum_planes_; }
	bool EnabledMovement() const noexcept { return enabled_movement_; }
	bool EnabledRotation() const noexcept { return enabled_rotation_; }
	bool EnabledZoom() const noexcept { return enabled_zoom_; }
	bool Stale() const noexcept { return stale_; }
    glm::dvec3 PrevPos() const noexcept { return prev_position_; }
    float PrevYaw() const noexcept { return prev_yaw_; }
	float PrevPitch() const noexcept { return prev_pitch_; }
	glm::dvec3 FPSOffset() const noexcept { return fps_offset_; }

	// Setters
	void SetEnableMovement(bool value) noexcept { enabled_movement_ = value; }
	void SetEnableRotation(bool value) noexcept { enabled_rotation_ = value; }
	void SetEnableZoom(bool value) noexcept { enabled_zoom_ = value; }
	void SetStale(bool value) noexcept { stale_ = value; }
    void SetPos(glm::dvec3 new_pos) noexcept { position_ = new_pos; }
    void SetYaw(float yaw) noexcept { yaw_ = yaw; }
    void SetPitch(float pitch) noexcept { pitch_ = pitch; }
    void SetPrevPos(glm::dvec3 new_prev_pos) noexcept { prev_position_ = new_prev_pos; }
    void SetPrevYaw(float new_prev_yaw) noexcept { prev_yaw_ = new_prev_yaw; }
    void SetPrevPitch(float new_prev_pitch) noexcept { prev_pitch_ = new_prev_pitch; }

private:
	/**
	 * @brief Recalculates the front, right, and up vectors from the current yaw and pitch.
	 */
	void UpdateCameraVectors();
};

#endif // CAMERA_HPP

