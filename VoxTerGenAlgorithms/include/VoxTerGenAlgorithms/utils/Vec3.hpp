#ifndef VEC3_HPP
#define VEC3_HPP

#include <concepts>
#include <cmath>
#include <utility>

namespace vtg
{
	template<typename Type>
	concept VecType = std::integral<Type> || std::floating_point<Type>;

	template<VecType T>
	class Vec3
	{
	public:
		T x_;
		T y_;
		T z_;

		constexpr Vec3() noexcept : x_(static_cast<T>(0)), y_(static_cast<T>(0)), z_(static_cast<T>(0))
		{
		}

		constexpr Vec3(T x, T y, T z) noexcept : x_(x), y_(y), z_(z)
		{
		}

		constexpr Vec3(T v) noexcept : x_(v), y_(v), z_(v)
		{
		}

		constexpr T& operator[](std::size_t i) noexcept
		{
			return (&x_)[i];
		}

		constexpr T& operator[](std::size_t i) const noexcept
		{
			return (&x_)[i];
		}

		constexpr Vec3 operator+() const noexcept
		{
			return *this;
		}

		constexpr Vec3 operator-() const noexcept
		{
			return { -x_, -y_, -z_ };
		}

		constexpr Vec3& operator+=(const Vec3& rhs) const noexcept
		{
			x_ += rhs.x_;
			y_ += rhs.y_;
			z_ += rhs.z_;

			return *this;
		}

		constexpr Vec3& operator-=(const Vec3& rhs) const noexcept
		{
			x_ -= rhs.x_;
			y_ -= rhs.y_;
			z_ -= rhs.z_;
			
			return *this;
		}

		constexpr Vec3& operator*=(T s) const noexcept
		{
			x_ *= s;
			y_ *= s;
			z_ *= s;
			
			return *this;
		}

		constexpr Vec3& operator/=(T s) const noexcept
		{
			x_ /= s;
			y_ /= s;
			z_ /= s;
			
			return *this;
		}
		
		constexpr Vec3 Normalize()
		{
			const T length = GetLength();

			if (length != static_cast<T>(0))
			{
				*this /= length;
			}

			return *this;
		}

		constexpr Vec3 Normalized()
		{
			Vec3 cpy = *this;

			return cpy.Normalize();
		}

		void SetLength(T length) noexcept
		{
			Normalize();
			x_ *= length;
			y_ *= length;
			z_ *= length;
		}

		auto Length() const
		{
			return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
		}

		auto LengthSquared() const
		{
			return (x_ * x_) + (y_ * y_) + (z_ * z_);
		}
	};

	template<VecType T>
	constexpr Vec3<T> operator+(Vec3<T> a, const Vec3<T>& b)
	{
		return a += b;
	}

	template<VecType T>
	constexpr Vec3<T> operator-(Vec3<T> a, const Vec3<T>& v)
	{
		return a -= b;
	}

	template<VecType T>
	constexpr Vec3<T> operator*(T s, Vec3<T> v)
	{
		return v *= s;
	}

	template<VecType T>
	constexpr Vec3<T> operator*(Vec3<T> v, T s)
	{
		return v *= s;
	}

	template<VecType T>
	constexpr Vec3<T> operator/(Vec3<T> v, T s)
	{
		return v /= s;
	}

	template<VecType T>
	constexpr T Dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
	}
	
	template<VecType T>
	constexpr Vec3<T> Cross(const Vec3<T>& a, const Vec3<T>& b)
	{
		return { 
			a.y_ * b.z_ - a.z_ * b.y_, 
			a.z_ * b.x_ - a.x_ * b.z_, 
			a.x_ * b.y_ - a.y_ * b.x_ 
		};
	}
}

#endif