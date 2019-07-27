#include <gl/glut.h>
#include<math.h>
#include <stdlib.h>//exit
#include <vector>

using std::vector;

struct point {

	int x, y;

}p;

vector<point>polypoint;

float y = 0.5f;//��׼����

float y2;//��׼����

float dy = 0.001f;//�任��

float a = 1.5f;//��ֵ

int process = 1;//���ƹ��̣�1��״̬1�仯��2��2��״̬2��ص�1


void Init()

{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	gluOrtho2D(-3.0, 15.0, -15.0, 3.0); //  ���ڣ���ֽ����������ʾ�ķ�Χ��X:-5.0~5.0, Y:-5.0~5.0  ����������һ��

	glMatrixMode(GL_MODELVIEW);

}

void drawTriangle_line(float y)

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //�µ���˫

	glVertex2f(0.0f, -y);//�µ�

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);//���

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//�ҵ�

	glEnd();

	glBegin(GL_LINES); //������

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a); //�µ�

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, a * 0.5); //���  sqrt()

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5); glVertex2f(a*sqrt(3) * 0.5, a * 0.5); //�ҵ�

	glEnd();

}

void drawTriangle_line2(float y) //����������ԭ�㣬�߳�Ϊ2��������

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //��˫�ϵ�

	glVertex2f(0.0f, y);//�ϵ�

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5);//���

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);//�ҵ�

	glEnd();

	glBegin(GL_LINES); //������

	glVertex2f(0.0f, y); glVertex2f(0.0f, a); //�ϵ�

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, -a * 0.5); //���  sqrt()

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5); glVertex2f(a*sqrt(3) * 0.5, -a * 0.5); //�ҵ�

	glEnd();

}

void drawSquare_line(float y) 
{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); 

	glVertex2f(0.0f, -y);//�µ�

	glVertex2f(-y , 0.0f);//���

	glVertex2f(0.0f, y);//�ϵ�

	glVertex2f(y, 0.0f);//�ҵ�

	glEnd();

	glBegin(GL_LINES); //������

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a);//�µ�

	glVertex2f(-y, 0.0f); glVertex2f(-a, 0.0f);//���

	glVertex2f(0.0f, y); glVertex2f(0.0f, a);//�ϵ�

	glVertex2f(y, 0.0f); glVertex2f(a, 0.0f);//�ҵ�

	glEnd();
}

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

	for (int i = 0; i < 5; i++) // ��i��,j��
	{
		glLoadIdentity(); //����ģ����ͼ����

		glTranslatef(i % 2 * 0.5 * sqrt(3) * a, i*-1.5 * a, 0.0);  //��һ�� ��j%2 ��һ��������һ�в���

		drawTriangle_line(y);

		for (int j = 1; j < 5; j++) //һ�л�5�����У�
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);

		}

	}
	/*

		glLoadIdentity(); //����ģ����ͼ����


		glColor3f(0.0f, 0.0f, 1.0f);
		glPointSize(10);
		glBegin(GL_POINTS); //������
		glVertex2f(-5.0, -5.0);
		glVertex2f(5.0, 5.0); //-10.0, 10.0, -10.0, 10.0
		glEnd();

		glBegin(GL_LINES); //������
		glVertex2f(-5.0, -5.0); glVertex2f(5.0, 5.0); //-10.0, 10.0, -10.0, 10.0
		glEnd();


		drawTriangle_line(y);

		for (int i = 1; i < 5; i++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);

		}

		glLoadIdentity(); //�ָ���ʼ����ϵ

		glTranslatef(0.5 * sqrt(3) * a, -1.5 * a, 0.0);  //��һ��

		drawTriangle_line(y);

		for (int i = 1; i < 5; i++)
		{
			glTranslatef(sqrt(3) * a, 0.0, 0.0);

			drawTriangle_line(y);
		}

		glLoadIdentity();

		glTranslatef(0, -3*a, 0.0); //������

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

	glTranslatef(0.5*sqrt(3) * a, -0.5 * a, 0.0);//��Ϊԭʼλ��

	for (int i = 0; i < 5; i++)  //i��j��
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

	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

//	drawSquare_line(y);

	for (int i = 0; i < 5; i++) // ��i��,j��
	{
		glLoadIdentity(); //����ģ����ͼ����

		glTranslatef(0.0 ,- i * 2 * a , 0.0);  //��һ��

		drawSquare_line(y);

		for (int j = 1; j < 5; j++) //һ�л�5�����У�
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
	glTranslatef( - a , a , 0.0);//��Ϊ��ʼλ��

//	drawSquare_line(y2);

	for (int i = 0; i < 6; i++)  //i��j��
	{
		glPushMatrix();

		glTranslatef(0 , -i * 2 * a, 0.0);//��һ��

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

void myIdle(void) //�ڿ���ʱ���ã��ﵽ����Ч��

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

		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

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

	glutInitWindowSize(800, 800);//������С����

	glutCreateWindow("����");

	Init();

	glutDisplayFunc(Display3);

	//glutReshapeFunc(Reshape);

	glutIdleFunc(&myIdle);

	glutMouseFunc(mymouse);

	glutMainLoop();

	return 0;

}
