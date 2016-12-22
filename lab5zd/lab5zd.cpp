#include "stdafx.h"
#define _USE_MATH_DEFINES
//#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace std;
float const a = 6;
float const b = 4;
float const a2 = 9;
float const b2 = 7;
int const N = 40;//okres pierwszy
int const N2 = 40;//sqrt((pow(40,2)*pow(10,3))/pow(7,3));okres drugi
float xu, xv, yu, yv, zu, zv;
float X, Y, Z;
float vLength;
int model = 2;
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 100.0, 0.0, 0.0, 0.0 };
// inicjalizacja po?o?enia obserwatora
static GLfloat light_position[] = { 0.0, 0.0, 1.0, 1.0 };
/*************************************************************************************/

// Funkcja rysuj?ca osie uk?adu wspó?rz?dnych

static GLfloat theta = 0.0;   // k?t obrotu obiektu
static GLfloat theta2 = 0.0;   // k?t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_x;     // przelicznik pikseli na stopnie
static GLfloat pix2angle_y;     // przelicznik pikseli na stopnie

static GLint status = 0;      // stan klawiszy myszy

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 0;

static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c? i poprzedni? kursora myszy
static int delta_y = 0;
static int delta_z = 0;

double R = 10;
float fi = 0.0;


struct punkty
{
	double x;
	double y;
	double z;
};
const float PI = 3.14159265;
//static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu

float const r = 1;
bool spin = true;
float speed = 0.008;

static GLfloat theta1[] = { 0.0, 90.0, 0.0 };
struct wsp
{
	wsp(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	wsp()
	{
		wsp(0, 0, 0);
	}

	float x;
	float y;
	float z;
};

wsp colors_sun[N][N];
wsp colors_planet[N][N];
wsp colors_planet2[N2][N2];
typedef float point3[3];
wsp planetPosition(-a, 0, 0);
wsp planetPosition2(-a, 0, 0);

// Funkcja "bada" stan myszy i ustawia warto?ci odpowiednich zmiennych globalnych

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}

	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

		z_pos_old = y;
		//cout << viewer[2];
		status = 2;
	}

	else

		status = 0;         // nie zosta? wci?ni?ty ?aden klawisz
}

/*************************************************************************************/

// Funkcja "monitoruje" po?o?enie kursora myszy i ustawia warto?ci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	delta_y = y - y_pos_old;
	double pom = y - z_pos_old;
	if (status == 2 && viewer[2] + pom < 270 && viewer[2] + pom > 30) viewer[2] += pom;

	x_pos_old = x;            // podstawienie bie?acego po?o?enia jako poprzednie
	y_pos_old = y;
	z_pos_old = y;

	glutPostRedisplay();    // przerysowanie obrazu sceny
}

/*************************************************************************************/
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba 
// przerysowaæ scenê)

void savewspToOpenGL(wsp point)
{
	glVertex3f(point.x, point.y, point.z);
}

void SaveColor(wsp color)
{
	glColor3f(color.x, color.y, color.z);
}

void spinPlanet()
{
	if (spin) {
		theta1[0] += 0.15;
		if (theta1[0] > 360.0) theta1[0] -= 360.0;

		theta1[1] += 0.15;
		if (theta1[1] > 360.0) theta1[1] -= 360.0;

		theta1[2] += 0.15;
		if (theta1[2] > 360.0) theta1[2] -= 360.0;

		glutPostRedisplay();
	}
}

void DrawOrbit()
{
	wsp points[N];

	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < N; i++)
	{
		float phi = (2 * M_PI / N) * i;

		wsp point;
		point.x = a*cos(phi);
		point.y = b*sin(phi);
		point.z = 0;
		points[i] = point;
	}

	glBegin(GL_LINES);
	for (int i = 0; i < N - 1; i++)
	{
		savewspToOpenGL(points[i]);
		savewspToOpenGL(points[i + 1]);
	}
	savewspToOpenGL(points[N - 1]);
	savewspToOpenGL(points[0]);

	glEnd();
}


void RandomColors_sun()
{
	srand(time(NULL));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			wsp color_sun;

			color_sun.x = rand() % 101 / 100.0f + 50.0f;
			color_sun.y = rand() % 70 / 100.0f;
			color_sun.z = 0.0f;

			colors_sun[i][j] = color_sun;
		}
	}
}

void RandomColors_planet()
{
	srand(time(NULL));

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			wsp color_planet;

			color_planet.x = 0.0f;
			color_planet.y = rand() % 101 / 100.0f;
			color_planet.z = rand() % 101 / 100.0f;

			colors_planet[i][j] = color_planet;
		}
	}
}



void DrawSun(float r)
{
	wsp points[N][N];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			float u = (2 * M_PI * i) / (N - 1);
			float v = (M_PI * j / (N - 1)) - 0.5 * M_PI;

			//wzory paranetryczne kuli
			wsp point;
			point.x = r*cos(v)*cos(u);
			point.y = r*cos(v)*sin(u);
			point.z = r*sin(v);

			points[i][j] = point;
		}
	}

	//wypelnianie kolorami
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < N - 1; i++)
	{
		for (int j = 0; j < N - 1; j++)
		{
			glNormal3f(colors_sun[i][j].x, colors_sun[i][j].y, colors_sun[i][j].z);
			SaveColor(colors_sun[i][j]);
			savewspToOpenGL(points[i][j]);

			glNormal3f(colors_sun[i+1][j + 1].x, colors_sun[i + 1][j + 1].y, colors_sun[i + 1][j + 1].z);
			SaveColor(colors_sun[i + 1][j + 1]);
			savewspToOpenGL(points[i + 1][j + 1]);

			glNormal3f(colors_sun[i + 1][j].x, colors_sun[i + 1][j].y, colors_sun[i + 1][j].z);
			SaveColor(colors_sun[i + 1][j]);
			savewspToOpenGL(points[i + 1][j]);

			glNormal3f(colors_sun[i][j].x, colors_sun[i][j].y, colors_sun[i][j].z);
			SaveColor(colors_sun[i][j]);
			savewspToOpenGL(points[i][j]);

			glNormal3f(colors_sun[i][j + 1].x, colors_sun[i][j + 1].y, colors_sun[i][j + 1].z);
			SaveColor(colors_sun[i][j + 1]);
			savewspToOpenGL(points[i][j + 1]);

			glNormal3f(colors_sun[i + 1][j + 1].x, colors_sun[i + 1][j + 1].y, colors_sun[i + 1][j + 1].z);
			SaveColor(colors_sun[i + 1][j + 1]);
			savewspToOpenGL(points[i + 1][j + 1]);
		}
	}
	glEnd();
}

void DrawPlanet(float r)
{
	wsp points[N][N];

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			float u = (2 * M_PI * i) / (N - 1);
			float v = (M_PI * j / (N - 1)) - 0.5 * M_PI;

			//wzory paranetryczne kuli
			wsp point;
			point.x = r*cos(v)*cos(u);
			point.y = r*cos(v)*sin(u);
			point.z = r*sin(v);

			points[i][j] = point;


			//wektory normalne
			xv = -r*sin(v)*cos(u);
			xu = -r*cos(v)*sin(u);
			yv = -r*sin(v)*sin(u);
			yu = -r*cos(v)*cos(u);
			zv = r*cos(v);
			zu = 0;

			//normalizacja
			X = yu*zv - zu*yv;
			Y = zu*xv - xu*zv;
			Z = xu*yv - yu*xv;
			//std::cout << colorsOfPoint[0] << ", " << colorsOfPoint[1] << ", " << colorsOfPoint[2] << std::endl;
			vLength = sqrt(pow(X, 2.0) + pow(Y, 2.0) + pow(Z, 2.0));
			//std::cout << vLength << std::endl;
			//if (vLength == 0) std::cout << "i: " << i << "\tj: " << j << std::endl;

			//std::cout << sqrt(pow(colorsOfPoint[0],2.0)+pow(colorsOfPoint[1],2.0)+pow(colorsOfPoint[2],2.0)) << std::endl;
			X = X / vLength;
			Y = Y / vLength;
			Z = Z / vLength;

			if (j <= N / 2)
			{
				colors_planet[i][j].x = X;
				colors_planet[i][j].y = Y;
				colors_planet[i][j].z = Z;
			}
			else
			{
				colors_planet[i][j].x = -X;
				colors_planet[i][j].y = -Y;
				colors_planet[i][j].z = -Z;
			}
		}
	}


	//wypelnianie kolorami
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < N/2; i++)
	{
		for (int j = 0; j < N - 1; j++)
		{
			glNormal3f(colors_sun[i][j].x, colors_sun[i][j].y, colors_sun[i][j].z);
			SaveColor(colors_planet[i][j]);
			savewspToOpenGL(points[i][j]);

			glNormal3f(colors_sun[i+1][j].x, colors_sun[i + 1][j].y, colors_sun[i + 1][j].z);
			SaveColor(colors_planet[i+1][j]);
			savewspToOpenGL(points[i + 1][j]);

			glNormal3f(colors_sun[i][j + 1].x, colors_sun[i][j + 1].y, colors_sun[i][j + 1].z);
			SaveColor(colors_planet[i][j+1]);
			savewspToOpenGL(points[i][j+1]);
		
			glNormal3f(colors_sun[i][j + 1].x, colors_sun[i][j + 1].y, colors_sun[i][j + 1].z);
			SaveColor(colors_planet[i][j+1]);
			savewspToOpenGL(points[i][j + 1]);

			glNormal3f(colors_sun[i + 1][j + 1].x, colors_sun[i + 1][j + 1].y, colors_sun[i + 1][j + 1].z);
			SaveColor(colors_planet[i+1][j+1]);
			savewspToOpenGL(points[i+1][j+1]);
			
			glNormal3f(colors_sun[i + 1][j].x, colors_sun[i + 1][j].y, colors_sun[i + 1][j].z);
			SaveColor(colors_planet[i+1][j]);
			savewspToOpenGL(points[i + 1][j]);
	
		}
	}

	for (int i = N - 1; i > N / 2; i--)
	{
		for (int j = 0; j < N - 1; j++)
		{
			glNormal3f(colors_sun[i][j].x, colors_sun[i][j].y, colors_sun[i][j].z);
			SaveColor(colors_planet[i][j]);
			savewspToOpenGL(points[i][j]);

			glNormal3f(colors_sun[i-1][j].x, colors_sun[i - 1][j].y, colors_sun[i - 1][j].z);
			SaveColor(colors_planet[i-1][j]);
			savewspToOpenGL(points[i - 1][j]);

			glNormal3f(colors_sun[i][j + 1].x, colors_sun[i][j + 1].y, colors_sun[i][j + 1].z);
			SaveColor(colors_planet[i][j+1]);
			savewspToOpenGL(points[i][j + 1]);

			glNormal3f(colors_sun[i][j + 1].x, colors_sun[i][j + 1].y, colors_sun[i][j + 1].z);
			SaveColor(colors_planet[i][j+1]);
			savewspToOpenGL(points[i][j + 1]);

			glNormal3f(colors_sun[i - 1][j+1].x, colors_sun[i - 1][j+1].y, colors_sun[i - 1][j+1].z);
			SaveColor(colors_planet[i-1][j+1]);
			savewspToOpenGL(points[i - 1][j + 1]);

			glNormal3f(colors_sun[i - 1][j].x, colors_sun[i - 1][j].y, colors_sun[i - 1][j].z);
			SaveColor(colors_planet[i-1][j]);
			savewspToOpenGL(points[i - 1][j]);

		}
	}
	glEnd();
}



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	
	if (model == 2)
	{
		if (status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
		{
			theta += delta_x*pix2angle_x;		// modyfikacja k¹ta obrotu o k¹t proporcjonalny
			fi += delta_y*pix2angle_y;		// do ró¿nicy po³o¿eñ kursora myszy

			if (theta >= 360.0)
				theta = 0.0;
			if (fi >= 360.0)
				fi = 0.0;
		}

		else if (status == 2) {	// jeœli prawy klawisz myszy wciœniêty
			R += 0.1* delta_y;	// modyfikacja polozenia obserwatora(zoom)
			if (R <= 6.0)
				R = 6.0;
			if (R >= 25.0)
				R = 25.0;
		}

		viewer[0] = R * cos(theta) * cos(fi);
		viewer[1] = R * sin(fi);
		viewer[2] = R * sin(theta) * cos(fi);
		light_position[0] = R * cos(theta) * cos(fi);
		light_position[1] = R * sin(fi);
		light_position[2] = R * sin(theta) * cos(fi);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	}

	else if (model == 1)
	{

		viewer[0] = planetPosition.x;
		viewer[1] = planetPosition.y;
		viewer[2] = planetPosition.z;
		//		wsp points[N][N];


	}
	glPushMatrix();
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], 0.0, cos(fi), 0.0);

	// do ró?nicy po?o?e? kursora myszy

	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);


	DrawOrbit();
	glPopMatrix();

	//DrawOrbit2();
	glPushMatrix();
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], 0.0, cos(fi), 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);
	glTranslated(-sqrt(a*a - b*b), 0, 0);
	DrawSun(r);
	glPopMatrix();

	glLoadIdentity();

	glPushMatrix();

	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], 0.0, cos(fi), 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k?t
	glRotatef(theta2, 1.0, 0.0, 0.0);
	glTranslated(planetPosition.x, planetPosition.y, planetPosition.z);
	glRotatef(theta1[1], 0.0, 1.0, 0.0);
	DrawPlanet(r / 2);
	glPopMatrix();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
	//
}
/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
}

void MyInit(void)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny

	// Definicja materia³u z jakiego zrobiony jest czajnik

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego               

	GLfloat mat_shininess = { 20.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a

	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po³o¿enie Ÿród³a


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = { 0.05f };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { 0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	/*************************************************************************************/
	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a

	/*************************************************************************************/
	// Ustawienie patrametrów materia³u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 

}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_x = 360.0*0.1 / (float)horizontal; // przeliczenie pikseli na stopnie
	pix2angle_y = 360.0*0.1 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	// Prze??czenie macierzy bie??cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie??cej

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
	// relacji pomi?dzy wysoko?ci? i szeroko?ci? okna

	glMatrixMode(GL_MODELVIEW);
	// Prze??czenie macierzy bie??cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

}
float t = 0;

//funckja poruszajaca
void MovePlanet()
{
	float sunToPlanet = sqrt(pow(planetPosition.x - (-sqrt(a*a - b*b)), 2) + pow(planetPosition.y, 2));

	t += speed / sunToPlanet;

	if (t > 2 * M_PI)
		t = 0;

	wsp point;
	point.x = a*cos(t);
	point.y = b*sin(t);
	point.z = 0;

	planetPosition = point;

	if (spin) {
		theta1[0] += 0.05;
		if (theta1[0] > 360.0) theta1[0] -= 360.0;

		theta1[1] += 0.15;
		if (theta1[1] > 360.0) theta1[1] -= 360.0;

		theta1[2] += 0.05;
		if (theta1[2] > 360.0) theta1[2] -= 360.0;

		glutPostRedisplay();
	}
	glutPostRedisplay();

	float sunToPlanet2 = sqrt(pow(planetPosition2.x - (-sqrt(a*a - b*b)), 2) + pow(planetPosition2.y, 2));

	t += speed / sunToPlanet2;

	if (t > 2 * M_PI)
		t = 0;

	wsp point2;
	point2.x = a2*cos(t);
	point2.y = b2*sin(t);
	point2.z = 0;

	planetPosition2 = point2;
	glutPostRedisplay();
}


void main(void)
{
	RandomColors_sun();
	RandomColors_planet();
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 800);

	glutCreateWindow("Solar system");
	glutKeyboardFunc(keys);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(MovePlanet);
	MyInit();
	glEnable(GL_DEPTH_TEST);

	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy
	glutMainLoop();
}