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

Vec2 Vec2::operator *= (double scalar){
	auto& [x, y] = m_xy;
	x *= scalar;
	y *= scalar;
	return *this;
}

Vec2 Vec2::operator*(double scalar){ *this *= scalar; return *this; };

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

double Vec2::length() const{ 
	auto [x, y] = m_xy;
	return sqrt(x * x + y * y); 
}

void Vec2::normalize(){
	auto& [x, y] = m_xy;
	double r = sqrt(x * x + y * y);
	double inv_sqrt = 1.0f / ( r == 0.0f ? 1e30 : r);
	x *= inv_sqrt;
	y *= inv_sqrt;
};

double Vec2::angle() const{ return atan2(m_xy.second, m_xy.first); }

void Vec2::rotate(double a){
	double cos_a = std::cos(a);
	double sin_a = std::sin(a);

	double rot_x, rot_y;
	auto& [x, y] = m_xy;

	rot_x = x * cos_a - y * sin_a;
	rot_y = x * sin_a + y * cos_a;

	x = rot_x;
	y = rot_y;
};
