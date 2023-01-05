#include "Vec.h"

using namespace Math;
Vec2 Vec2::operator=(const Vec2& other){
	if(this == &other)
		return other;

	auto& [x, y] = m_xy;

	x = other.x();
	y = other.y();

	return *this;
};

Vec2 Vec2::operator+(const Vec2& other){
	const auto& [x1, y1] = m_xy;
	Vec2 tmp(x1 + other.x(), y1 + other.y());
	return tmp;
}

Vec2 Vec2::operator *= (float scalar){
	auto& [x, y] = m_xy;
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2 Vec2::operator*(float scalar){ *this *= scalar; return *this; };

Vec2 Vec2::operator-(const Vec2& other){
	Vec2 tmp = *this + (-1.0f * other);
	return tmp;
};

bool Vec2::operator==(const Vec2& other){
	return (other.x() == m_xy.first && other.y() == m_xy.second);
}

Vec2 Vec2::operator-=(const Vec2& other){ 
	auto& [x, y] = m_xy;
	x -= other.x();
	y -= other.y();
	return *this;
};

float Vec2::length() const{ 
	auto [x, y] = m_xy;
	return sqrt(x * x + y * y); 
}

void Vec2::normalize(){
	auto& [x, y] = m_xy;
	float r = sqrt(x * x + y * y);
	float inv_sqrt = 1.0f / ( r == 0.0f ? 1e30 : r);
	x *= inv_sqrt;
	y *= inv_sqrt;
};

float Vec2::angle() const{ return atan2(m_xy.second, m_xy.first); }

void Vec2::rotate(float a){
	float cos_a = std::cos(a);
	float sin_a = std::sin(a);

	float rot_x, rot_y;
	auto& [x, y] = m_xy;

	rot_x = x * cos_a - y * sin_a;
	rot_y = x * sin_a + y * cos_a;

	x = rot_x;
	y = rot_y;
};

std::pair<int, int> Math::map2_screen(size_t map_w, size_t map_h, size_t vp_w, size_t vp_h, const Vec2& position){
		int ans_x, ans_y;
		float map_2_screenX = (float)vp_w / (float)map_w;
		float map_2_screenY = (float)vp_h / (float)map_h;
		ans_x = (int)(position.x() * map_2_screenX);
		ans_y = (int)(position.y() * map_2_screenY);
		std::pair<int, int> ans{ans_x, ans_y};
		return ans;
	};
