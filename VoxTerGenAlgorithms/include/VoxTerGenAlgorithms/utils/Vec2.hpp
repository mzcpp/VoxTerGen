#ifndef VEC2_HPP
#define VEC2_HPP

#include <concepts>
#include <cmath>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <limits>

namespace vtg
{
	template<std::floating_point T>
	class Vec2
	{
	private:
		static constexpr T default_abs_eps = std::is_same_v<T, float> ? T(1e-6) : T(1e-12);
		static constexpr T default_rel_eps = std::is_same_v<T, float> ? T(1e-5) : T(1e-9);

	public:
		T x_;
		T y_;

		constexpr Vec2() noexcept : x_(T(0)), y_(T(0))
		{
		}

		constexpr Vec2(T x, T y) noexcept : x_(x), y_(y)
		{
		}

		constexpr Vec2(T v) noexcept : x_(v), y_(v)
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

		constexpr Vec2 operator+() const noexcept
		{
			return *this;
		}

		constexpr Vec2 operator-() const noexcept
		{
			return { -x_, -y_ };
		}

		constexpr Vec2& operator+=(const Vec2& rhs) noexcept
		{
			x_ += rhs.x_;
			y_ += rhs.y_;

			return *this;
		}

		constexpr Vec2& operator-=(const Vec2& rhs) noexcept
		{
			x_ -= rhs.x_;
			y_ -= rhs.y_;

			return *this;
		}

		constexpr Vec2& operator*=(T s) noexcept
		{
			x_ *= s;
			y_ *= s;

			return *this;
		}

		constexpr Vec2& operator/=(T s) noexcept
		{
			x_ /= s;
			y_ /= s;

			return *this;
		}

		constexpr bool operator==(const Vec2& other) const noexcept
		{
			return x_ == other.x_ && y_ == other.y_;
		}

		constexpr bool operator!=(const Vec2& other) const noexcept
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

		[[nodiscard]] constexpr bool Equals(const Vec2& other, T abs_eps = default_abs_eps, T rel_eps = default_rel_eps) const noexcept
		{
			return NearlyEqual(x_, other.x_, abs_eps, rel_eps) && NearlyEqual(y_, other.y_, abs_eps, rel_eps);
		}

		constexpr Vec2& Normalize() noexcept
		{
			const T length = Length();

			if (length != T(0))
			{
				*this /= length;
			}

			return *this;
		}

		[[nodiscard]] constexpr Vec2 Normalized() const noexcept
		{
			Vec2 cpy = *this;

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
			return std::sqrt((x_ * x_) + (y_ * y_));
		}

		[[nodiscard]] T LengthSquared() const
		{
			return (x_ * x_) + (y_ * y_);
		}

		[[nodiscard]] constexpr Vec2 Rotate(T angle) const
		{
			return { x_ * std::cos(angle) - y_ * std::sin(angle), x_ * std::sin(angle) + y_ * std::cos(angle) };
		}

		[[nodiscard]] T Angle(const Vec2& other) const
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
	constexpr Vec2<T> operator+(Vec2<T> a, const Vec2<T>& b)
	{
		return a += b;
	}

	template<std::floating_point T>
	constexpr Vec2<T> operator-(Vec2<T> a, const Vec2<T>& v)
	{
		return a -= v;
	}

	template<std::floating_point T>
	constexpr Vec2<T> operator*(T s, Vec2<T> v)
	{
		return v *= s;
	}

	template<std::floating_point T>
	constexpr Vec2<T> operator*(Vec2<T> v, T s)
	{
		return v *= s;
	}

	template<std::floating_point T>
	constexpr Vec2<T> operator/(Vec2<T> v, T s)
	{
		return v /= s;
	}

	template<std::floating_point T>
	constexpr T Dot(const Vec2<T>& a, const Vec2<T>& b)
	{
		return a.x_ * b.x_ + a.y_ * b.y_;
	}
}


#endif