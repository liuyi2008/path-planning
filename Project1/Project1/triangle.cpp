#include <gl/glut.h>
#include <vector>
#include<math.h>
using std::vector;

float y = 1.0f;//基准坐标

float y2;//基准坐标

float dy = 0.001f;//变换量

float a = 3.0f;//阈值

struct point
{
	int x;
	int y;
};

vector<point>Tpoint;

void Init()

{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	gluOrtho2D(-5.0, 5.0, -5.0, 5.0); //设置显示的范围是X:-5.0~5.0, Y:-5.0~5.0

	glMatrixMode(GL_MODELVIEW);

}

void Reshape(int w, int h)

{

	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //设置视区尺寸

	glMatrixMode(GL_PROJECTION); //指定当前操作投影矩阵堆栈

	glLoadIdentity(); //重置投影矩阵

	//指定透视投影的观察空间

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

}

void drawTriangle_line(float y) //绘制中心在原点，边长为2的正方形

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //下单上双

	glVertex2f(0.0f, -y);//下点

	glVertex2f(-y*sqrt(3) * 0.5, y * 0.5);//左点

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//右点

	glEnd();

	glBegin(GL_LINES); //顶点线

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a); //下点

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5); glVertex2f(-a*sqrt(3) * 0.5 ,a * 0.5); //左点  sqrt()

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

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

	//gluLookAt(0.0 ,0.0 ,0.0 ,5.0 ,-5.0 ,0.0 ,0.0 ,1.0 ,0.0 );

	glLoadIdentity(); //重置模型视图矩阵
 
	drawTriangle_line(y);

	for (int i = 0; i < 5; i++)
	{
		glTranslatef(i * sqrt(3) * a, 0.0, 0.0);

		drawTriangle_line(y);

	}

	glLoadIdentity(); //恢复初始坐标系

	glTranslatef(0.5 * sqrt(3) * a, -0.5 * a - a, 0.0);  //下一行

	drawTriangle_line(y);

	for (int i = 0; i < 5; i++)
	{
		glTranslatef(i * sqrt(3) * a, 0.0, 0.0);

		drawTriangle_line(y);

	}

	glFlush();

	glLoadIdentity();

	//glutSwapBuffers();

}

void Display2(void) 
{

	glTranslatef(0.5*sqrt(3) * a, -1.5f, 0.0);

	drawTriangle_line2(y2);

	glFlush();

	glLoadIdentity();

}

void myIdle(void) //在空闲时调用，达到动画效果

{
	y += dy;

	if (y <= a)
	{
		Display();
	}
	else if (y > a)
	{
		y2 = 2 * a - y;

		Display2();

		glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

	}
	else if (y2 == 0)

		y = 1.0f;

}

int main(int argc, char *argv[])

{

	glutInit(&argc, argv);

	//窗口使用RGB颜色，双缓存和深度缓存

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(1000, 600);

	glutCreateWindow("动画");

	Init();

	glutDisplayFunc(Display);

	//glutReshapeFunc(Reshape);

	glutIdleFunc(&myIdle);

	glutMainLoop();

	return 0;

}
