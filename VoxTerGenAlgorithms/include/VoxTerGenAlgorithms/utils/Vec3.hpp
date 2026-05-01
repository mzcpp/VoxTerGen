#ifndef VEC3_HPP
#define VEC3_HPP

#include <concepts>
#include <cmath>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace vtg
{
	template<std::floating_point T>
	class Vec3
	{
	private:
		static constexpr T default_abs_eps = std::is_same_v<T, float> ? T(1e-6) : T(1e-12);
		static constexpr T default_rel_eps = std::is_same_v<T, float> ? T(1e-5) : T(1e-9);

	public:
		T x_;
		T y_;
		T z_;

		constexpr Vec3() noexcept : x_(T(0)), y_(T(0)), z_(T(0))
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

		constexpr const T& operator[](std::size_t i) const noexcept
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

		constexpr Vec3& operator+=(const Vec3& rhs) noexcept
		{
			x_ += rhs.x_;
			y_ += rhs.y_;
			z_ += rhs.z_;

			return *this;
		}

		constexpr Vec3& operator-=(const Vec3& rhs) noexcept
		{
			x_ -= rhs.x_;
			y_ -= rhs.y_;
			z_ -= rhs.z_;
			
			return *this;
		}

		constexpr Vec3& operator*=(T s) noexcept
		{
			x_ *= s;
			y_ *= s;
			z_ *= s;
			
			return *this;
		}

		constexpr Vec3& operator/=(T s) noexcept
		{
			x_ /= s;
			y_ /= s;
			z_ /= s;
			
			return *this;
		}

		constexpr bool operator==(const Vec3& other) const noexcept
		{
			return x_ == other.x_ && y_ == other.y_ && z_ == other.z_;
		}

		constexpr bool operator!=(const Vec3& other) const noexcept
		{
			return !(*this == other);
		}

		[[nodiscard]] constexpr bool NearlyEqual(T a, T b, T abs_eps, T rel_eps) const
		{
			T diff = std::abs(a - b);

			if (diff <= abs_eps)
			{
				return true;
			}

			return diff <= rel_eps * std::max(std::abs(a), std::abs(b));
		}

		[[nodiscard]] constexpr bool Equals(const Vec3& other, T abs_eps = default_abs_eps, T rel_eps = default_rel_eps) const noexcept
		{
			return NearlyEqual(x_, other.x_, abs_eps, rel_eps) && 
				NearlyEqual(y_, other.y_, abs_eps, rel_eps) && 
				NearlyEqual(z_, other.z_, abs_eps, rel_eps);
		}
		
		constexpr Vec3& Normalize() noexcept
		{
			const T length = Length();

			if (length != T(0))
			{
				*this /= length;
			}

			return *this;
		}

		[[nodiscard]] constexpr Vec3 Normalized() const noexcept
		{
			Vec3 cpy = *this;

			return cpy.Normalize();
		}

		void SetLength(T length) noexcept
		{
			T len = Length();
			
			if (len != T(0))
			{
				*this *= (length / len);
			}
		}

		[[nodiscard]] T Length() const
		{
			return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
		}

		[[nodiscard]] T LengthSquared() const
		{
			return (x_ * x_) + (y_ * y_) + (z_ * z_);
		}

		[[nodiscard]] constexpr Vec3 RotateAroundAxis(const Vec3& axis, T angle) const
		{
			const Vec3 k = axis.Normalized();
			const T cos_theta = std::cos(angle);
			const T sin_theta = std::sin(angle);

			return (*this * cos_theta) + (Cross(k, *this) * sin_theta) + (k * Dot(k, *this) * (T(1) - cos_theta));
		}

		[[nodiscard]] T Angle(const Vec3& other) const
		{
			const T denom = Length() * other.Length();
			
			if (denom == T(0))
			{
				return T(0);
			}

			const T cos_theta = std::clamp(Dot(*this, other) / denom, T(-1), T(1));
			return std::acos(cos_theta);
		}
	};

	template<std::floating_point T>
	constexpr Vec3<T> operator+(Vec3<T> a, const Vec3<T>& b)
	{
		return a += b;
	}

	template<std::floating_point T>
	constexpr Vec3<T> operator-(Vec3<T> a, const Vec3<T>& v)
	{
		return a -= v;
	}

	template<std::floating_point T>
	constexpr Vec3<T> operator*(T s, Vec3<T> v)
	{
		return v *= s;
	}

	template<std::floating_point T>
	constexpr Vec3<T> operator*(Vec3<T> v, T s)
	{
		return v *= s;
	}

	template<std::floating_point T>
	constexpr Vec3<T> operator/(Vec3<T> v, T s)
	{
		return v /= s;
	}

	template<std::floating_point T>
	constexpr T Dot(const Vec3<T>& a, const Vec3<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
	}
	
	template<std::floating_point T>
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