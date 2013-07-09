//
//  Point.cpp
//  BlueShield_FrameWork
//
//  Created by Matthias SEGUI SERERA on 03/06/13.
//  Copyright (c) 2013 Matthias SEGUI SERERA. All rights reserved.
//

#include "Point.h"
#include "math.h"

Point::Point() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Point::~Point() {
}

Point::Point(const float p_x, const float p_y, const float p_z) {
	x = p_x;
	y = p_y;
	z = p_z;
}

Point::Point(const Point & p_Point) {
	x = p_Point.X();
	y = p_Point.Y();
	z = p_Point.Z();
}

void Point::X(const float p_x) {
	x = p_x;
}

void Point::Y(const float p_y) {
	y = p_y;
}

void Point::Z(const float p_z) {
	z = p_z;
}

float Point::X() const {
	return x;
}

float Point::Y() const {
	return y;
}
float Point::Z() const {
	return z;
}

void Point::Set(float p_x, float p_y, float p_z) {
	x = p_x;
	y = p_y;
	z = p_z;
}

bool Point::operator==(const Point & p_v3) {
	if (this == &p_v3)
		return true;
	if (x == p_v3.x)
		if (y == p_v3.y)
			if (z == p_v3.z)
				return true;
	return false;
}

Point& Point::operator=(const Point & p_v3) {
	x = p_v3.x;
	y = p_v3.y;
	z = p_v3.y;
	return *this;
}

float Point::dot(const Point & p_v3) const {
	return x * p_v3.x + y * p_v3.y + z * p_v3.z;
}

Point Point::operator+(const Point & p_v3) {
	Point res(p_v3);
	res.x += x;
	res.y += y;
	res.z += z;
	return res;
}
Point Point::operator-(const Point & p_v3) {
	Point res(p_v3);
	res.x -= x;
	res.y -= y;
	res.z -= z;
	return res;
}

Point Point::operator *(const float & scalair) {
	Point res(*this);
	res.x *= scalair;
	res.y *= scalair;
	res.z *= scalair;
	return res;
}

ostream& operator<<(ostream& out, const Point& vec) {
	out << "(" << vec.X() << "," << vec.Y() << "," << vec.Z() << ")";
	return out;
}

Point Point::operator*(const Point& p) const {
	return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y - p.x);
}

double distance_points(Point p1, Point p2) {
	return sqrt(pow(p2.X() - p1.X(), 2) + pow(p2.Y() - p1.Y(), 2));
}
