#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "utils/Constants.hpp"

#include <glm/glm.hpp>

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
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 world_up_;

	float yaw_;
	float pitch_;
	float zoom_;

	float near_plane_;
	float far_plane_;

	glm::mat4 view_;
	glm::mat4 projection_;
	glm::mat4 view_proj_;
	
	std::array<Plane, 6> frustum_planes_;
	
	glm::vec3 prev_position_;
	float prev_yaw_;
	float prev_pitch_;

	bool enabled_movement_;
	bool changed_;
	bool moving_;

public:
    /**
     * @brief Constructs a Camera with optional initial position, up vector, yaw, and pitch.
     *
     * @param position Initial position in world space (default: origin)
     * @param up Up vector (default: world Y-axis)
     * @param yaw Initial yaw angle in degrees
     * @param pitch Initial pitch angle in degrees
     */
    Camera
    (
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = Constants::Camera::yaw,
        float pitch = Constants::Camera::pitch
    );

    Camera(const Camera& camera) = delete;
    Camera& operator=(const Camera&) = delete;

    Camera(Camera&& other) = delete;
    Camera& operator=(Camera&& other) = delete;

    /**
     * @brief Stores the camera's state for interpolation in the next frame.
     */
    void PreTick();

	/**
	 * @brief Resets flags at the end of frame.
	 */
	void EndTick();

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
    void PrintCamera() const;

    /**
     * @brief Prints the six frustum planes to the logger.
     */
    void PrintFrustumPlanes() const;
	
	// Getters
	/** @brief Returns the view matrix. */
	glm::mat4 ViewMatrix() const { return view_; }

	/** @brief Returns the projection matrix. */
	glm::mat4 ProjectionMatrix() const { return projection_; }

	/** @brief Returns the camera position. */
	glm::vec3 Pos() const { return position_; }

	/** @brief Returns the camera front vector. */
	glm::vec3 Front() const { return front_; }

	/** @brief Returns the camera up vector. */
	glm::vec3 Up() const { return up_; }

	/** @brief Returns the camera right vector. */
	glm::vec3 Right() const { return right_; }

	/** @brief Returns the world up vector. */
	glm::vec3 WorldUp() const { return world_up_; }

	/** @brief Returns the yaw angle in degrees. */
	float Yaw() const { return yaw_; }

	/** @brief Returns the pitch angle in degrees. */
	float Pitch() const { return pitch_; }

	/** @brief Returns the zoom factor (field of view). */
	float Zoom() const { return zoom_; }

	/** @brief Returns the frustum planes. */
	const std::array<Plane, 6>& GetFrustumPlanes() const { return frustum_planes_; }

	/** @brief Returns whether movement is enabled. */
	bool EnabledMovement() const { return enabled_movement_; }

	/** @brief Returns whether the camera has changed since last update. */
	bool Changed() const { return changed_; }

	/** @brief Returns whether the camera is currently moving. */
	bool Moving() const { return moving_; }

	// Setters
	/**
	 * @brief Enables or disables camera movement.
	 * @param value True to enable movement, false to disable
	 */
	void EnableMovement(bool value) { enabled_movement_ = value; }

	/**
	 * @brief Sets whether the camera is currently moving.
	 * @param value True if moving, false otherwise
	 */
	void SetMoving(bool value) { moving_ = value; }

private:
	/**
	 * @brief Recalculates the front, right, and up vectors from the current yaw and pitch.
	 */
	void UpdateCameraVectors();
};

#endif // CAMERA_HPP

