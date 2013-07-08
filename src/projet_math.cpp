#include<windows.h>

#include <GL/glut.h>

#include <stdlib.h>
#include <iostream>
#include"Courbe.h"

using namespace std;

vector<Point*> CONTROLE_POINT;
Point * dragedPoint = NULL;
Courbe bspline;


float calc_poids(int p, int i, float t, vector<float> & T) {
	if (p == 0) {
		if (T[i] <= t && t < T[i + 1])
			return 1;
		else
			return 0;
	}

	float denominateur_gauche = T[i + p] - T[i];
	float numerateur_gauche = t - T[i];

	float denominateur_droit = T[i + 1 + p] - T[i + 1];
	float numerateur_droit = T[i + 1 + p] - t;

	float sous_poids_gauche = (denominateur_gauche == 0) ? 0 : numerateur_gauche / denominateur_gauche * calc_poids(p - 1, i, t, T);
	float sous_poids_droit = (denominateur_droit == 0) ? 0 : numerateur_droit / denominateur_droit * calc_poids(p - 1, i + 1, t, T);

	return sous_poids_gauche + sous_poids_droit;

}

Point calcule_point_BSpline(float t, int p, vector<Point*> & P, vector<float> & T) {

	Point point = (*P[0]) * calc_poids(p, 0, t, T);
	for (unsigned int i = 1; i < P.size(); ++i) {
		float poids = calc_poids(p, i, t, T);
		point = point + (*P[i]) * poids;
	}
	return point;
}

Courbe comput_points(vector<Point*> & point_ctrls, vector<float> & tableau_noeuds, int degree_bspline, float step, bool isClamp) {
	Courbe resultat;
	if (point_ctrls.size() > 2) {
		tableau_noeuds.clear();

		if (isClamp) {
			for (int i = 0; i < degree_bspline; ++i) {
				tableau_noeuds.push_back(0);
			}
		}
		for (unsigned int i = 0; i < point_ctrls.size() + degree_bspline + 1; ++i) {
			tableau_noeuds.push_back(i);
		}
		if (isClamp) {
			for (int i = 0; i < degree_bspline; ++i) {
				tableau_noeuds.push_back(point_ctrls.size() + degree_bspline);
			}
		}

		vector<Point> tableau_point_bspline;
		for (float t = tableau_noeuds[(degree_bspline)]; t < tableau_noeuds[tableau_noeuds.size() - 1 - (degree_bspline)]; t += step) {
			Point p = calcule_point_BSpline(t, degree_bspline, point_ctrls, tableau_noeuds);
			tableau_point_bspline.push_back(p);
		}
		resultat = tableau_point_bspline;
	}
	return resultat;
}

char presse;
int anglex, angley, x, y, xold, yold;

void mousemotion3D(int x, int y);
void mouse3D(int bouton, int etat, int x, int y);
void special3D(int touche, int x, int y);
void keyBoard3D(unsigned char touche, int x, int y);

void mouse2D(int button, int state, int x, int y);
void mouseMotion2D(int x, int y);
void keyBoard2D(unsigned char touche, int x, int y);
void initRendering()

{

	glEnable(GL_DEPTH_TEST);

}

void handleResize(int w, int h)

{

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	gluPerspective(45.0, (double) w / (double) h, 1.0, 200);

}
void pointVert(Point & p) {
	glVertex3f(p.X(), p.Y(), p.Z());
}
void drawSurface(vector<vector<Point> > & surface) {

	for (unsigned int i = 0; i < surface.size() - 1; ++i) {
		glBegin(GL_TRIANGLE_STRIP);

		for (unsigned int j = 0; j < surface[0].size(); ++j) {

			pointVert(surface[i + 1][j]);
			pointVert(surface[i][j]);
		}

		glEnd();

	}
}

void drawWires(vector<vector<Point> > & surface) {
	for (unsigned int i = 0; i < surface.size() - 1; ++i) {
		int jmax = surface[i].size() - 1;
		for (int ji = 0; ji < jmax; ++ji) {
			glBegin(GL_LINE_STRIP);
			pointVert(surface[i][ji]);
			pointVert(surface[i + 1][ji + 1]);
			pointVert(surface[i + 1][ji]);
			pointVert(surface[i][ji]);
			pointVert(surface[i][ji + 1]);
			pointVert(surface[i + 1][ji + 1]);
			glEnd();
		}
	}
}

float ZOOM = 150.0f;
bool G_ISWIREFRAME = false;
int G_LENGTH_EXTRUSION = 20;
void drawScene3D()

{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 0.1 };
	GLfloat light_ambient[] = { 0.5, 0.4, 0.5, 0.1 };
	GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();

	gluLookAt(/**/
	0.0, 0.0, ZOOM, /* eye position */
	0.0, 0.0, 0.0, /* center is at (0,0,0) */
	0.0, 1.0, 0.); /* up is in positive Y direction */

	glRotatef(angley, 1.0, 0.0, 0.0);
	glRotatef(anglex, 0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);

	//vector<vector<Point> > surface = bspline.extrudeZ(10, G_LENGTH_EXTRUSION, 1);
	vector<vector<Point> > surface = bspline.rotateY(10);

	if (G_ISWIREFRAME) {
		drawWires(surface);
	} else {
		drawSurface(surface);
	}
	glutSwapBuffers();

}

void drawPointCarre(Point p, int largeur = 1) {
	Point p1, p2, p3, p4;

	p1.X(p.X() - largeur);
	p1.Y(p.Y() - largeur);

	p2.X(p.X() - largeur);
	p2.Y(p.Y() + largeur);

	p3.X(p.X() + largeur);
	p3.Y(p.Y() + largeur);

	p4.X(p.X() + largeur);
	p4.Y(p.Y() - largeur);

	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex2f(p1.X(), p1.Y());
	glVertex2f(p2.X(), p2.Y());
	glVertex2f(p2.X(), p2.Y());
	glVertex2f(p3.X(), p3.Y());
	glVertex2f(p3.X(), p3.Y());
	glVertex2f(p4.X(), p4.Y());
	glVertex2f(p4.X(), p4.Y());
	glVertex2f(p1.X(), p1.Y());

	glEnd();
}

void draw_curve(Courbe & c, Point & color) {
	vector<Point> pts = c.pts();
	glBegin(GL_LINES);
	glColor3f(color.X(), color.Y(), color.Z());
	vector<Point>::iterator it = pts.begin();
	if (it != pts.end()) {
		glVertex2f(it->X(), it->Y());
	}
	for (; it != pts.end(); it++) {
		glVertex2f(it->X(), it->Y());
		glVertex2f(it->X(), it->Y());
	}

	glEnd();
}

void draw_curve(vector<Point*> tPoints, Point & color) {
	glBegin(GL_LINES);
	glColor3f(color.X(), color.Y(), color.Z());
	vector<Point*>::iterator it = tPoints.begin();
	if (it != tPoints.end()) {
		glVertex3f((*it)->X(), (*it)->Y(), (*it)->Z());
	}
	for (; it != tPoints.end(); it++) {
		glVertex3f((*it)->X(), (*it)->Y(), (*it)->Z());
		glVertex3f((*it)->X(), (*it)->Y(), (*it)->Z());
	}

	glEnd();
}

void drawScene2D()

{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(/**/
	0.0, 0.0, 150.0, /* eye position */
	0.0, 0.0, 0.0, /* center is at (0,0,0) */
	0.0, 1.0, 0.); /* up is in positive Y direction */

	Point color_ctrl(0, 1, 0);
	draw_curve(CONTROLE_POINT, color_ctrl);
	for (vector<Point*>::iterator it = CONTROLE_POINT.begin(); it != CONTROLE_POINT.end(); it++) {
		drawPointCarre(**it);

	}

	Point color_bs(0, 0, 1);
	draw_curve(bspline, color_bs);

	glutSwapBuffers();

}

int main(int argc, char** argv)

{

	glutInit(&argc, argv);

	initRendering();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

//window1=

	glutCreateWindow("3D");

	glutDisplayFunc(drawScene3D);

	glutReshapeFunc(handleResize);

	glutMouseFunc(mouse3D);
	glutMotionFunc(mousemotion3D);
	glutSpecialFunc(special3D);
	glutKeyboardFunc(keyBoard3D);

//create the second  window

//window2 =

	glutCreateWindow("2D");

//define a window position for second window

	glutPositionWindow(540, 40);

// register callbacks for second window, which is now current

	glutReshapeFunc(handleResize);

	glutDisplayFunc(drawScene2D);

	glutMouseFunc(mouse2D);
	glutMotionFunc(mouseMotion2D);
	glutKeyboardFunc(keyBoard2D);

	glutMainLoop();

	return 0;

}

void mouse3D(int button, int state, int x, int y) {
// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		presse = 1; //le booleen presse passe à 1 (vrai)
		xold = x; //on sauvegardela position de la souris
		yold = y;

	}

// Si on relache le bouton gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse = 0; //le booleen presse passe à 0 (faux)
}
void mousemotion3D(int x, int y) {
	if (presse) // si le bouton de gauche est pressé
	{
		/* On modifie les angles de rotation de l'objet en fonction de la
		 position actuelle de la souris et de la dernière position suvegardée*/
		anglex = anglex + (x - xold);
		angley = angley + (y - yold);
		glutPostRedisplay(); // Rafraichissement de l'affichage
	}
	xold = x; //sauvegarde des valeurs courantes de la position de la souris
	yold = y;
}

int DISTANCE_SELECTION = 5;
void mouse2D(int button, int state, int x, int y) {
	x = (x * 120 / 500) - 60;
	y = (y * -120 / 500) + 60;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		Point *p = new Point(x, y, 0);

		for (vector<Point*>::iterator it = CONTROLE_POINT.begin(); it != CONTROLE_POINT.end(); ++it) {
			if (distance_points((*p), (**it)) < DISTANCE_SELECTION) {
				dragedPoint = *it;
			}
		}
		if (dragedPoint == NULL) {
			CONTROLE_POINT.push_back(p);
			dragedPoint = p;

			vector<float> tableau_neuds;
			bspline = comput_points(CONTROLE_POINT, tableau_neuds, 3, 0.1f, false);
			bspline.flatten();

			glutPostRedisplay();
			glutSetWindow(1);
			glutPostRedisplay();
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		dragedPoint = NULL;
	}

}
void mouseMotion2D(int x, int y) {
	x = (x * 120 / 500) - 60;
	y = (y * -120 / 500) + 60;
	if (dragedPoint != NULL) {
		(*dragedPoint).X(x);
		(*dragedPoint).Y(y);
		vector<float> tableau_neuds;
		bspline = comput_points(CONTROLE_POINT, tableau_neuds, 3, 0.1f, false);
		bspline.flatten();
		glutPostRedisplay();
		glutSetWindow(1);
		glutPostRedisplay();

	}

}

void keyBoard2D(unsigned char touche, int x, int y) {
	switch (touche) {
	case 8:
		if (CONTROLE_POINT.size() > 0) {
			CONTROLE_POINT.pop_back();
			vector<float> tableau_neuds;
			bspline = comput_points(CONTROLE_POINT, tableau_neuds, 3, 0.1f, false);
			bspline.flatten();
			glutPostRedisplay();
			glutSetWindow(1);
			glutPostRedisplay();
		}
	}
}

void special3D(int touche, int x, int y) {
	switch (touche) {
	case GLUT_KEY_DOWN:
		ZOOM++;
		glutSetWindow(1);
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:
		ZOOM--;
		glutSetWindow(1);
		glutPostRedisplay();
		break;

	}

}
void keyBoard3D(unsigned char touche, int x, int y) {
	switch (touche) {
	case 'w':
		G_ISWIREFRAME = !G_ISWIREFRAME;
		glutSetWindow(1);
		glutPostRedisplay();
		break;
	case '9':
		G_LENGTH_EXTRUSION += 1;
		glutSetWindow(1);
		glutPostRedisplay();
		break;
	case '3':
		if (G_LENGTH_EXTRUSION > 1) {
			G_LENGTH_EXTRUSION -= 1;
		}
		glutSetWindow(1);
		glutPostRedisplay();
		break;
	}
}
