#include <iostream>
#include <cmath>
#include <gtest/gtest.h>
#include "Vec.h"

TEST(Vec2, constructor){
	Math::Vec2 vec(1.0f, 1.0f);
	ASSERT_TRUE(vec.x() == 1.0f && vec.y() == 1.0f);
	Math::Vec2 vec2;
	ASSERT_EQ(vec2.x(), 0.0f);
	ASSERT_EQ(vec2.y(), 0.0f);
}

TEST(Vec2, write_access){
	Math::Vec2 vec(1.0f, 1.0f);
	auto& [x, y] = vec.xy();
	ASSERT_TRUE(x == 1.0f && y == 1.0f);
	x = 12.0f;
	y = 2.0f;
	ASSERT_EQ(x, 12.0f);
	ASSERT_EQ(y, 2.0f);
}

TEST(Vec2, addition){
	Math::Vec2 vec1(1.0f, 1.0f);
	Math::Vec2 vec3 = vec1 + Math::Vec2(2.0f, 3.0f);
	ASSERT_TRUE(vec1 == Math::Vec2(1.0f, 1.0f));

	ASSERT_EQ(vec3.x(), 3.0f);
	ASSERT_EQ(vec3.y(), 4.0f);

	Math::Vec2 vecA(1.0f, 3.5f);
	vecA = vec1 + vec3;
}

TEST(Vec2, multiplication_by_scalar){
	Math::Vec2 vec1(1.0f, 1.0f);
	vec1 = vec1 * 3.5f;
	ASSERT_TRUE(vec1.x() == 3.5f && vec1.y() == 3.5f);
	vec1 *= 2.0f;
	ASSERT_TRUE(vec1.x() == 7.0f && vec1.y() == 7.0f);

	ASSERT_TRUE(vec1.x() == 7.0f && vec1.y() == 7.0f);


	Math::Vec2 vecA(1.0f, 2.0f);
	Math::Vec2 vecB;
	vecB = 5.0f * vecA;
	ASSERT_EQ(vecB.x(), 5.0f);
	ASSERT_EQ(vecB.y(), 10.0f);
}

TEST(Vec2, substraction){
	Math::Vec2 a(2.0f, 5.0f);
	Math::Vec2 b(1.0f, 3.0f);
	Math::Vec2 c;
	c = a - b;

	ASSERT_TRUE(c == Math::Vec2(1.0f, 2.0f));
	ASSERT_TRUE(a == Math::Vec2(2.0f, 5.0f));
	ASSERT_TRUE(b == Math::Vec2(1.0f, 3.0f));

	c = Math::Vec2(1.0f, 3.0f) - Math::Vec2(1.0f, 2.0f);
	ASSERT_TRUE(c == Math::Vec2(0.0f, 1.0f));

	c -= Math::Vec2(1.0f, 2.0f);
	ASSERT_TRUE(c == Math::Vec2(-1.0f, -1.0f));

	Math::Vec2 x = Math::Vec2(12.0f, 4.0f) - Math::Vec2(14.0f, 3.0f);
	ASSERT_TRUE(x == Math::Vec2(-2.0f, 1.0f));
}

TEST(Vec2, normalize){
	Math::Vec2 a(2.0f, 5.0f);
	ASSERT_EQ(a.length(), sqrt(29.0f));
	a.normalize();
	ASSERT_FALSE(a == Math::Vec2(2.0f, 5.0f));
	ASSERT_EQ(a.length(), 1.0f);


	Math::Vec2 b(0.0f, 0.0f);
	b.normalize();
	ASSERT_TRUE(b == Math::Vec2(0.0f, 0.0f));
	ASSERT_EQ(b.length(), 0.0f);
}

constexpr static double to_deg(const double a){ return (a * 180.0f) / M_PI; };

TEST(Vec2, angle){
	Math::Vec2 a(1.0f, 0.0f);
	ASSERT_EQ(a.angle(), 0.0f);

	a = Math::Vec2(0.5f, 0.5f);
	ASSERT_EQ(to_deg(a.angle()), 45.0f);

	a = Math::Vec2(0.0f, 1.0f);
	ASSERT_EQ(to_deg(a.angle()), 90.0f);

	a = Math::Vec2(-1.0f, 1.0f);
	ASSERT_EQ(to_deg(a.angle()), 90.0f + 45.0f);

	a = Math::Vec2(-1.0f, -1.0f);
	ASSERT_EQ(to_deg(a.angle()), 45.0f - 180.0f);
}

int main(int argc, char ** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
};
