
//#include <iostream>
//#include <GL/glut.h>

/*
const int boardSize = 8;
const int tileSize = 70;

const float whiteColor[] = { 0.7, 0.6, 0.56 }, blackColor[] = { 0.47, 0.34, 0.23 };

void init() {

	// Window color
	glClearColor(0, 0, 0, 0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 900, 0, 560);
}

void drawTile(int startX, int startY) {

	glBegin(GL_POLYGON);

	glVertex2i(startX, startY + tileSize);
	glVertex2i(startX + tileSize, startY + tileSize);
	glVertex2i(startX + tileSize, startY);
	glVertex2i(startX, startY);

	glEnd();
}

void board() {

	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++) {

			if((i + j) % 2 == 0) glColor3f(whiteColor[0], whiteColor[1], whiteColor[2]);
			else glColor3f(blackColor[0], blackColor[1], blackColor[2]);

			drawTile(i * tileSize, j * tileSize);
		}

	glFlush();
}

int start_glut(int agrc, char** argv) {

	// Initialize glut
	glutInit(&agrc, argv);

	// Set display mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 560);
	glutCreateWindow("Chess board graphics");

	init();

	// Send graphics to display window
	glutDisplayFunc(board);

	// Start the application
	glutMainLoop();

	return 0;
}

*/