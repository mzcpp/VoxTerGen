#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "utils/Constants.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <array>
#include <iostream>
#include <iomanip>

struct Plane
{
	glm::vec3 normal_;
	float dist_;
};

class CameraController;

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

	void PreTick();

	void Tick(float aspect_ratio);

	glm::mat4 InterpolatedViewMatrix(float alpha) const;

	void UpdateSimulationMatrices(float aspect_ratio);
	
	void UpdateFrustumPlanes();
	
	bool PointInsideFrustum(const glm::vec3& point) const;
	
	bool SanityCheckFrustum() const;
	
	void PrintCamera() const;

	void PrintFrustumPlanes() const;
	
	// Getters
	glm::mat4 ViewMatrix() const { return view_; }
	glm::mat4 ProjectionMatrix() const { return projection_;  }
	glm::vec3 Pos() const { return position_; }
	glm::vec3 Front() const { return front_; }
	glm::vec3 Up() const { return up_; }
	glm::vec3 Right() const { return right_; }
	glm::vec3 WorldUp() const { return world_up_; }
	float Yaw() const { return yaw_; }
	float Pitch() const { return pitch_; }
	float Zoom() const { return zoom_; }
	const std::array<Plane, 6>& GetFrustumPlanes() const { return frustum_planes_; }
	bool EnabledMovement() const { return enabled_movement_; }
	bool Changed() const { return changed_; }
	bool Moving() const { return moving_; }

	// Setters
	void EnableMovement(bool value) { enabled_movement_ = value; }
	void SetMoving(bool value) { moving_ = value; }

private:
	void UpdateCameraVectors();
};

#endif

