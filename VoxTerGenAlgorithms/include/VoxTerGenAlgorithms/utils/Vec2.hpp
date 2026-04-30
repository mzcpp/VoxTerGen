#ifndef VEC2_HPP
#define VEC2_HPP

#include <concepts>
#include <cmath>
#include <utility>

template<typename Type>
concept VecType = std::arithmetic<T> || std::floating_point<T>;

template<VecType T>
class Vec2
{
private:
	T x_;
	T y_;

public:
	Vec2() : x_(static_cast<T>(0)), y_(static_cast<T>(0))
	{
	}

	Vec2(T x, T y) : x_(x), y_(y)
	{
	}

	Vec2(const Vec2& other) = default;
	Vec2& operator=(const Vec2& other) = default;

	Vec2(Vec2&& other) = default;
	Vec2& operator=(Vec2&& other) = default;
	
	void Normalize() noexcept
	{
		const T length = GetLength();

		if (length == static_cast<T>(0))
		{
			return;
		}

		x_ /= length;
		y_ /= length;
	}

	void SetLength(T length) noexcept
	{
		Normalize();
		x_ *= length;
		y_ *= length;
	}

	auto Length() const
	{
		return std::sqrt((x_ * x_) + (y_ * y_));
	}
};

#endif