/*
 * Courbe.h
 *
 *  Created on: 7 juil. 2013
 *      Author: anton
 */

#ifndef COURBE_H_
#define COURBE_H_
#include <vector>
#include "Point.h"

class Courbe {
	std::vector<Point> _pts;
public:
	Courbe();
	Courbe(std::vector<Point>);

	std::vector<Point> & pts();

	void flatten();

	std::vector<std::vector<Point> >rotateX(int);
	std::vector<std::vector<Point > *> rotateY(int);
	std::vector<std::vector<Point> >rotateZ(int);

	std::vector<std::vector<Point> *>extrudeZ(int,float,float);

	~Courbe();
};

#endif /* COURBE_H_ */
