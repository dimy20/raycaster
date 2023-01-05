#pragma once
#include <vector>
#include <cmath>
#include <ostream>

namespace Math{
	class Vec2{
		public:
			Vec2() : m_xy(0.0f, 0.0f) {}
			Vec2(double x, double y) : m_xy(x, y) {};

			constexpr double x() const  { return m_xy.first; };
			constexpr double y() const  { return m_xy.second; };
			std::pair<double, double>& xy() { return m_xy; };

			Vec2 operator=(const Vec2& other);

			Vec2 operator+(const Vec2& other);
			Vec2 operator-(const Vec2& other);
			Vec2 operator*(double scalar);
			Vec2 operator *= (double scalar);
			bool operator==(const Vec2& other);
			Vec2 operator-=(const Vec2& other);

			void normalize();
			double length() const;
			double angle() const;
			void rotate(double a);

			inline friend std::ostream& operator << (std::ostream& out, const Vec2& vec){
				out << "(" << vec.x() << ", " << vec.y() << ")";
				return out;
			}
			//Vec2 operator*(double scalar, const Vec2& other);
		private:
			std::pair<double, double> m_xy;
	};

	constexpr double to_deg(double rad){ return (rad * 180.0f) / M_PI; };
	constexpr double to_rad(double deg){ return (deg * M_PI) / 180.0f; };

	inline Vec2 operator*(double scalar, const Vec2& other){
		Vec2 tmp = other;
		return tmp * scalar;
	}
};


