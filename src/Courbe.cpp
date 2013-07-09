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
			float ny, nz;
			ny = p.Y() * cosf(alpha) - p.Z() * sinf(alpha);
			nz = p.Y() * sinf(alpha) + p.Z() * cosf(alpha);
			p.Y(ny);
			p.Z(nz);
			section.push_back(p);
		}

		res.push_back(section);
	}

	return res;
}

vector<vector<Point> *> Courbe::rotateY(int pas) {
	vector<vector<Point> *> res;

	for (int i = 0; i < pas + 1; ++i) {
		float alpha = (i * 1.0f / pas) * (2 * PI);
		vector<Point> *section = new vector<Point>();
		res.push_back(section);
		for (unsigned int j = 0; j < _pts.size(); ++j) {
			Point p(_pts[j]);
			float nx, nz;

			nx = cosf(alpha) * p.X() + sinf(alpha) * p.Z();
			nz = -sinf(alpha) * p.X() + cosf(alpha) * p.Z();

			p.X(nx);
			p.Z(nz);

			section->push_back(p);
		}

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

vector<vector<Point> *> Courbe::extrudeZ(int pas, float longueur, float scale = 1) {
	vector<vector<Point> *> gride;

	for (float i = 0; i <= pas; ++i) {
		float h = (i) / pas;

		vector<Point> *segment = new vector<Point>();
		for (unsigned int ip = 0; ip < _pts.size(); ++ip) {
			Point p(_pts[ip]);
			p.Z(p.Z() + longueur * h);

			//p = p * h * scale;

			segment->push_back(p);
		}
		gride.push_back(segment);
	}
	return gride;
}

