#include "graphics/Camera.hpp"
#include "utils/Logger.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>

#include <algorithm>

Camera::Camera(glm::dvec3 position, float yaw, float pitch) : 
	position_(position), 
	front_(glm::vec3(0.0f, 0.0f, -1.0f)), 
	up_(glm::vec3(0.0f)), 
	right_(glm::vec3(0.0f)), 
	yaw_(yaw), 
	pitch_(pitch), 
	zoom_(constants::camera::zoom), 
	near_plane_(constants::camera::near_plane), 
	far_plane_(constants::camera::far_plane), 
	prev_position_(position), 
	prev_yaw_(yaw), 
	prev_pitch_(pitch), 
	enabled_movement_(true),
	enabled_rotation_(true),
	enabled_zoom_(true),
	stale_(true), 
	moving_(true)
{
	UpdateCameraVectors();
}

void Camera::Tick(float aspect_ratio)
{
	UpdateSimulationMatrices(aspect_ratio);
	UpdateCameraVectors();
	UpdateFrustumPlanes();

#if _DEBUG
	//PrintCamera();
	//PrintFrustumPlanes();
	SanityCheckFrustum();
#endif

	moving_ = false;
}

glm::mat4 Camera::InterpolatedViewMatrix(float alpha) const
{
	const glm::vec3 interp_pos = glm::vec3(glm::mix(prev_position_, position_, alpha));
	const float interp_yaw = glm::mix(prev_yaw_, yaw_, alpha);
	const float interp_pitch = glm::mix(prev_pitch_, pitch_, alpha);

	glm::vec3 front = { 
		std::cos(glm::radians(interp_yaw)) * std::cos(glm::radians(interp_pitch)), 
		std::sin(glm::radians(interp_pitch)), 
		std::sin(glm::radians(interp_yaw)) * std::cos(glm::radians(interp_pitch))
	};

	front = glm::normalize(front);

	return glm::lookAt(interp_pos, interp_pos + front, constants::math::world_up);
}

void Camera::UpdateSimulationMatrices(float aspect_ratio)
{
	if (!stale_)
	{
		return;
	}

	const glm::vec3 pos = glm::vec3(position_);
	view_ = glm::lookAt(pos, pos + front_, up_);
	projection_ = glm::perspective(glm::radians(zoom_), aspect_ratio, near_plane_, far_plane_);
	view_proj_ = projection_ * view_;
}

void Camera::UpdateCameraVectors()
{
	if (!stale_)
	{
		return;
	}

	const glm::vec3 front = { 
		cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)), 
		sin(glm::radians(pitch_)), 
		sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)) 
	};

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, constants::math::world_up));
	up_ = glm::normalize(glm::cross(right_, front_));
}

void Camera::UpdateFrustumPlanes()
{
	if (!stale_)
	{
		return;
	}

	stale_ = false;

	// Plane order: 0 = left, 1 = right, 2 = bottom, 3 = top, 4 = near, 5 = far

	// Left plane: row4 + row1
	frustum_planes_[0].normal_ = glm::vec3
	(
		view_proj_[0][3] + view_proj_[0][0],
		view_proj_[1][3] + view_proj_[1][0],
		view_proj_[2][3] + view_proj_[2][0]
	);

	frustum_planes_[0].dist_ = view_proj_[3][3] + view_proj_[3][0];

	// Right plane: row4 - row1
	frustum_planes_[1].normal_ = glm::vec3
	(
		view_proj_[0][3] - view_proj_[0][0],
		view_proj_[1][3] - view_proj_[1][0],
		view_proj_[2][3] - view_proj_[2][0]
	);

	frustum_planes_[1].dist_ = view_proj_[3][3] - view_proj_[3][0];

	// Bottom plane: row4 + row2
	frustum_planes_[2].normal_ = glm::vec3
	(
		view_proj_[0][3] + view_proj_[0][1],
		view_proj_[1][3] + view_proj_[1][1],
		view_proj_[2][3] + view_proj_[2][1]
	);

	frustum_planes_[2].dist_ = view_proj_[3][3] + view_proj_[3][1];

	// Top plane: row4 - row2
	frustum_planes_[3].normal_ = glm::vec3
	(
		view_proj_[0][3] - view_proj_[0][1],
		view_proj_[1][3] - view_proj_[1][1],
		view_proj_[2][3] - view_proj_[2][1]
	);

	frustum_planes_[3].dist_ = view_proj_[3][3] - view_proj_[3][1];

	// Near plane: row4 + row3
	frustum_planes_[4].normal_ = glm::vec3
	(
		view_proj_[0][3] + view_proj_[0][2],
		view_proj_[1][3] + view_proj_[1][2],
		view_proj_[2][3] + view_proj_[2][2]
	);

	frustum_planes_[4].dist_ = view_proj_[3][3] + view_proj_[3][2];

	// Far plane: row4 - row3
	frustum_planes_[5].normal_ = glm::vec3
	(
		view_proj_[0][3] - view_proj_[0][2],
		view_proj_[1][3] - view_proj_[1][2],
		view_proj_[2][3] - view_proj_[2][2]
	);

	frustum_planes_[5].dist_ = view_proj_[3][3] - view_proj_[3][2];

	for (Plane& plane : frustum_planes_)
	{
		const float length = glm::length(glm::vec3(plane.normal_));
		plane.normal_ /= length;
		plane.dist_ /= length;
	}
}

bool Camera::PointInsideFrustum(const glm::vec3& point) const
{
	for (int i = 0; i < 6; ++i)
	{
		if (glm::dot(frustum_planes_[i].normal_, point) + frustum_planes_[i].dist_ < constants::math::float_rel_epsilon)
		{
			//Logger::Log(LogLevel::ERROR, "Sanity fail: Point {} {} {} is outside frustum!", point.x, point.y, point.z);
			return false;
		}
	}

	return true;
}

bool Camera::SanityCheckFrustum() const
{
	const glm::vec3 point = glm::vec3(0.0f) + (front_ * ((constants::camera::far_plane - constants::camera::near_plane) / 2.0f));
		
	if (!PointInsideFrustum(point))
	{
		return false;
	}
	
	// TODO: Check more points to make this a stronger sanity check.

	return true;
}

void Camera::LogCameraData() const
{
	Logger::Log(LogLevel::DEBUG, "---------------------------- Camera Info ----------------------------");
	Logger::Log(LogLevel::DEBUG, "Position: {} {} {}", position_.x, position_.y, position_.z);
	Logger::Log(LogLevel::DEBUG, "Front: {} {} {}", front_.x, front_.y, front_.z);
	Logger::Log(LogLevel::DEBUG, "Up: {} {} {}", up_.x, up_.y, up_.z);
	Logger::Log(LogLevel::DEBUG, "Right: {} {} {}", right_.x, right_.y, right_.z);
	Logger::Log(LogLevel::DEBUG, "Yaw: {}", yaw_);
	Logger::Log(LogLevel::DEBUG, "Pitch: {}", pitch_);
}

void Camera::PrintFrustumPlanes() const
{
	static const char* names[6] = { "Left", "Right", "Bottom", "Top", "Near", "Far" };

	std::cout << "---------------------------- Frustum Planes ----------------------------\n";
	
	for (int i = 0; i < 6; ++i)
	{
		const auto& plane = frustum_planes_[i];
		Logger::Log(LogLevel::DEBUG, "{}: Normal = ({}, {}, {}), Dist = {}", names[i], plane.normal_.x, plane.normal_.y, plane.normal_.z, plane.dist_);
	}
}