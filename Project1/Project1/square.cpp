#include <gl/glut.h>
#include<math.h>
#include <stdlib.h>//exit
#include <vector>

using std::vector;

struct point {

	int x, y;

}p;

vector<point>polypoint;

float y = 0.5f;//基准坐标

float y2;//基准坐标

float dy = 0.001f;//变换量

float a = 1.5f;//阈值

int process = 1;//控制过程，1：状态1变化到2，2：状态2变回到1


void Init()

{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	gluOrtho2D(-3.0, 15.0, -15.0, 3.0); //  窗口（画纸），设置显示的范围是X:-5.0~5.0, Y:-5.0~5.0  比例和视区一样

	glMatrixMode(GL_MODELVIEW);

}

void drawTriangle_line(float y)

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //下单上双

	glVertex2f(0.0f, -y);//下点

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);//左点

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//右点

	glEnd();

	glBegin(GL_LINES); //顶点线

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a); //下点

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, a * 0.5); //左点  sqrt()

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5); glVertex2f(a*sqrt(3) * 0.5, a * 0.5); //右点

	glEnd();

}

void drawTriangle_line2(float y) //绘制中心在原点，边长为2的正方形

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //下双上单

	glVertex2f(0.0f, y);//上点

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5);//左点

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);//右点

	glEnd();

	glBegin(GL_LINES); //定点线

	glVertex2f(0.0f, y); glVertex2f(0.0f, a); //上点

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, -a * 0.5); //左点  sqrt()

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5); glVertex2f(a*sqrt(3) * 0.5, -a * 0.5); //右点

	glEnd();

}

void drawSquare_line(float y) 
{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); 

	glVertex2f(0.0f, -y);//下点

	glVertex2f(-y , 0.0f);//左点

	glVertex2f(0.0f, y);//上点

	glVertex2f(y, 0.0f);//右点

	glEnd();

	glBegin(GL_LINES); //顶点线

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a);//下点

	glVertex2f(-y, 0.0f); glVertex2f(-a, 0.0f);//左点

	glVertex2f(0.0f, y); glVertex2f(0.0f, a);//上点

	glVertex2f(y, 0.0f); glVertex2f(a, 0.0f);//右点

	glEnd();
}

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

	for (int i = 0; i < 5; i++) // 画i行,j列
	{
		glLoadIdentity(); //重置模型视图矩阵

		glTranslatef(i % 2 * 0.5 * sqrt(3) * a, i*-1.5 * a, 0.0);  //下一行 ，j%2 是一行右移下一行不动

		drawTriangle_line(y);

		for (int j = 1; j < 5; j++) //一行画5个（列）
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);

		}

	}
	/*

		glLoadIdentity(); //重置模型视图矩阵


		glColor3f(0.0f, 0.0f, 1.0f);
		glPointSize(10);
		glBegin(GL_POINTS); //定点线
		glVertex2f(-5.0, -5.0);
		glVertex2f(5.0, 5.0); //-10.0, 10.0, -10.0, 10.0
		glEnd();

		glBegin(GL_LINES); //定点线
		glVertex2f(-5.0, -5.0); glVertex2f(5.0, 5.0); //-10.0, 10.0, -10.0, 10.0
		glEnd();


		drawTriangle_line(y);

		for (int i = 1; i < 5; i++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);

		}

		glLoadIdentity(); //恢复初始坐标系

		glTranslatef(0.5 * sqrt(3) * a, -1.5 * a, 0.0);  //下一行

		drawTriangle_line(y);

		for (int i = 1; i < 5; i++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);
		}

		glLoadIdentity();

		glTranslatef(0, -3*a, 0.0); //下两行

		drawTriangle_line(y);

		for (int i = 1; i < 5; i++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);
		}
	*/

	glFlush();

	glLoadIdentity();

	//glutSwapBuffers();

}

void Display2(void)
{

	glTranslatef(0.5*sqrt(3) * a, -0.5 * a, 0.0);//此为原始位置

	for (int i = 0; i < 5; i++)  //i行j列
	{
		glPushMatrix();

		glTranslatef(i % 2 * -0.5 * sqrt(3) * a, i*-1.5 * a, 0.0);

		drawTriangle_line2(y2);

		for (int j = 1; j < 5; j++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line2(y2);

		}

		glPopMatrix();

	}

	glFlush();

	glLoadIdentity();

}

void Display3(void) 
{

	glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

//	drawSquare_line(y);

	for (int i = 0; i < 5; i++) // 画i行,j列
	{
		glLoadIdentity(); //重置模型视图矩阵

		glTranslatef(0.0 ,- i * 2 * a , 0.0);  //下一行

		drawSquare_line(y);

		for (int j = 1; j < 5; j++) //一行画5个（列）
		{
			glTranslatef(2 * a, 0.0, 0.0);

			drawSquare_line(y);

		}

	}

	glFlush();

	glLoadIdentity();

}

void Display4(void)
{
	glTranslatef( - a , a , 0.0);//此为起始位置

//	drawSquare_line(y2);

	for (int i = 0; i < 6; i++)  //i行j列
	{
		glPushMatrix();

		glTranslatef(0 , -i * 2 * a, 0.0);//下一行

		drawSquare_line(y2);

		for (int j = 1; j < 6; j++)
		{

			glTranslatef(2 * a, 0.0, 0.0);

			drawSquare_line(y2);

		}

		glPopMatrix();

	}

	glFlush();

	glLoadIdentity();

}

void myIdle(void) //在空闲时调用，达到动画效果

{
	switch (process)
	{
	case 1:y += dy;
		break;
	case 2:y -= dy;
		break;
	}

	if (y <= a)
	{
		Display3();
	}
	else if (y > a)
	{
		y2 = 2 * a - y;

		Display4();

		glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

	}

	if (y > 2 * a)

		process = 2;

	else if (y < 0)

		process = 1;

}

void mymouse(GLint button, GLint state, GLint wx, GLint wy)

{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)

		exit(0);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		int x = wx;
		int y = wy;
		p = { x,y };
		polypoint.push_back(p);
	}
}

void ControlPoint()

{

	glPointSize(5);

	for (int i = 0; i < polypoint.size(); i++)

	{

		glBegin(GL_POINTS);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(polypoint[i].x, polypoint[i].y);

		glEnd();

	}


}

int main(int argc, char *argv[])

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(50, 50);

	glutInitWindowSize(800, 800);//视区大小像素

	glutCreateWindow("动画");

	Init();

	glutDisplayFunc(Display3);

	//glutReshapeFunc(Reshape);

	glutIdleFunc(&myIdle);

	glutMouseFunc(mymouse);

	glutMainLoop();

	return 0;

}
