#pragma once
#include <vector>
#include <cmath>
#include <ostream>

namespace Math{
	class Vec2{
		public:
			Vec2() : m_xy(0.0f, 0.0f) {}
			Vec2(float x, float y) : m_xy(x, y) {};

			constexpr float x() const  { return m_xy.first; };
			constexpr float y() const  { return m_xy.second; };
			std::pair<float, float>& xy() { return m_xy; };

			Vec2 operator=(const Vec2& other);

			Vec2 operator+(const Vec2& other);
			Vec2 operator-(const Vec2& other);
			Vec2 operator*(float scalar);
			Vec2 operator *= (float scalar);
			bool operator==(const Vec2& other);
			Vec2 operator-=(const Vec2& other);

			void normalize();
			float length() const;
			float angle() const;
			void rotate(float a);

			inline friend std::ostream& operator << (std::ostream& out, const Vec2& vec){
				out << "(" << vec.x() << ", " << vec.y() << ")";
				return out;
			}
		private:
			std::pair<float, float> m_xy;
	};

	constexpr float to_deg(float rad){ return (rad * 180.0f) / M_PI; };
	constexpr float to_rad(float deg){ return (deg * M_PI) / 180.0f; };

	inline Vec2 operator*(float scalar, const Vec2& other){
		Vec2 tmp = other;
		return tmp * scalar;
	}

	std::pair<int, int> map2_screen(size_t map_w, size_t map_h, size_t vp_w, size_t vp_h, const Vec2& position);
};


