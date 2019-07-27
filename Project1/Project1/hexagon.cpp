#include <gl/glut.h>
#include<math.h>
#include <stdlib.h>//exit

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

void drawHexagon_line(float y)
{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); 

	glVertex2f(0.0f, -y);//下点

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5);//左下点

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);//左上点

	glVertex2f(0.0f, y);//上点

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//右上点

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);//右下点

	glEnd();

	glBegin(GL_LINES); //顶点线

	glVertex2f(0.0f, -y);                     glVertex2f(0.0f, -a); //下点

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, -a * 0.5);//左下点

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);  glVertex2f(-a * sqrt(3) * 0.5, a * 0.5);//左上点

	glVertex2f(0.0f, y);                      glVertex2f(0.0f, a);//上点

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);     glVertex2f(a*sqrt(3) * 0.5, a * 0.5);//右上点

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);    glVertex2f(a*sqrt(3) * 0.5, -a* 0.5);//右下点

	glEnd();

}

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

//	drawHexagon_line(y);

	for (int i = 0; i < 5; i++) // 画i行,j列
	{
		glLoadIdentity(); //重置模型视图矩阵

		glTranslatef(-i % 2 * 0.5 * sqrt(3) * a, i * -1.5 * a, 0.0);  //下一行 ，i%2 是一行右移下一行不动

		drawHexagon_line(y);

		for (int j = 1; j < 5; j++) //一行画5个（列）
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawHexagon_line(y);

		}

	}

	glFlush();

	glLoadIdentity();

}

void Display2(void)
{

	glTranslatef(0.5*sqrt(3) * a, -1.5 * a, 0.0);//此为原始位置

	drawHexagon_line(y2);

	for (int i = 0; i < 5; i++)  //i行j列
	{
		glPushMatrix();

		glTranslatef(i % 2 * -0.5 * sqrt(3) * a, i*-1.5 * a, 0.0);  //glTranslatef(i % 2 * 0.5 * sqrt(3) * a, i * -1.5 * a, 0.0);

		drawHexagon_line(y2);

		for (int j = 1; j < 5; j++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawHexagon_line(y2);

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
		Display();
	}
	else if (y > a)
	{
		y2 = 2 * a - y;

		Display2();

		glClear(GL_COLOR_BUFFER_BIT);//清除颜色和深度缓冲区

	}

	if (y > 2 * a)

		process = 2;

	else if (y < 0)

		process = 1;

}

int main(int argc, char *argv[])

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(50, 50);

	glutInitWindowSize(800, 800);//视区大小像素

	glutCreateWindow("动画");

	Init();

	glutDisplayFunc(Display);

	glutIdleFunc(&myIdle);

	glutMainLoop();

	return 0;

}
