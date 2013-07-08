/*
 * Courbe.cpp
 *
 *  Created on: 7 juil. 2013
 *      Author: anton
 */

#include "Courbe.h"
#include "math.h"
#define PI (3.141592653589793)

using namespace std;
Courbe::Courbe() {

}

Courbe::~Courbe() {
}

Courbe::Courbe(std::vector<Point> entre) :
		_pts(entre) {
}
std::vector<Point> & Courbe::pts() {
	return _pts;
}

void Courbe::flatten() {
	for (unsigned int i = 0; i < _pts.size(); ++i) {
		_pts[i].Z(0.0f);
	}
}

vector<vector<Point> > Courbe::rotateX(int pas) {
	vector<vector<Point> > res;

	for (int i = 0; i < pas; ++i) {
		float alpha = (i * 1.0f / pas) * (2 * PI);
		vector<Point> section(_pts.size());
		for (unsigned int j = 0; j < _pts.size(); ++j) {
			Point p(_pts[j]);
			p.Z(cosf(alpha) * p.Z());
			p.Y(sinf(alpha) * p.Y());
			section.push_back(p);
		}

		res.push_back(section);
	}

	return res;
}

vector<vector<Point> > Courbe::rotateY(int pas) {
	vector<vector<Point> > res;

	for (int i = 0; i < pas; ++i) {
		float alpha = (i * 1.0f / pas) * (2 * PI);
		vector<Point> section(_pts.size());
		for (unsigned int j = 0; j < _pts.size(); ++j) {
			Point p(_pts[j]);
			p.X(cosf(alpha) * p.X());
			p.Z(sinf(alpha) * p.Z());
			section.push_back(p);
		}

		res.push_back(section);
	}

	return res;
}

vector<vector<Point> > Courbe::rotateZ(int pas) {
	vector<vector<Point> > res;

	for (int i = 0; i < pas; ++i) {
		float alpha = (i * 1.0f / pas) * (2 * PI);
		vector<Point> section(_pts.size());
		for (unsigned int j = 0; j < _pts.size(); ++j) {
			Point p(_pts[j]);
			p.X(cosf(alpha) * p.X());
			p.Y(sinf(alpha) * p.Y());
			section.push_back(p);
		}

		res.push_back(section);
	}

	return res;
}

vector<vector<Point> > Courbe::extrudeZ(int pas, float longueur, float scale = 1) {
	vector<vector<Point> > gride;

	for (float i = 0; i <= pas; ++i) {
		float h = (i) / pas;

		vector<Point> segment;
		for (unsigned int ip = 0; ip < _pts.size(); ++ip) {
			Point p(_pts[ip]);
			p.Z(p.Z() + longueur * h);

			//p = p * h * scale;

			segment.push_back(p);
		}
		gride.push_back(segment);
	}
	return gride;
}

