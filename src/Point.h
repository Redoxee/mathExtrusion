//
//  vector3.h
//  BlueShield_FrameWork
//
//  Created by Matthias SEGUI SERERA on 03/06/13.
//  Copyright (c) 2013 Matthias SEGUI SERERA. All rights reserved.
//

#ifndef __BlueShield_FrameWork__vector3__
#define __BlueShield_FrameWork__vector3__

#include <iostream>
using namespace std;
class Point {
private:
	float x, y, z;

public:
	Point();
	~Point();

	Point(const float p_x, const float p_y, const float p_z);
	Point(const Point & Point);

	void X(const float p_x);
	void Y(const float p_y);
	void Z(const float p_z);
	float X() const;
	float Y() const;
	float Z() const;
    
    void Set(float p_x, float p_y, float p_z);
    
	//operator
    
    bool operator==(const Point & p_v3);
    Point& operator=(const Point & p_v3);
    Point operator+(const Point & p_v3);
    Point operator-(const Point & p_v3);
    Point operator*(const float& scalair);
    float dot(const Point& p_v3)const;

    friend ostream& operator<<(ostream& out, const Point& vec);
    friend double distance_points(Point p1,Point p2);
};

#endif /* defined(__BlueShield_FrameWork__vector3__) */
