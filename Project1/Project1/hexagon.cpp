#include <gl/glut.h>
#include<math.h>
#include <stdlib.h>//exit

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

void drawHexagon_line(float y)
{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); 

	glVertex2f(0.0f, -y);//�µ�

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5);//���µ�

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);//���ϵ�

	glVertex2f(0.0f, y);//�ϵ�

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//���ϵ�

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);//���µ�

	glEnd();

	glBegin(GL_LINES); //������

	glVertex2f(0.0f, -y);                     glVertex2f(0.0f, -a); //�µ�

	glVertex2f(-y * sqrt(3) * 0.5, -y * 0.5); glVertex2f(-a * sqrt(3) * 0.5, -a * 0.5);//���µ�

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5);  glVertex2f(-a * sqrt(3) * 0.5, a * 0.5);//���ϵ�

	glVertex2f(0.0f, y);                      glVertex2f(0.0f, a);//�ϵ�

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);     glVertex2f(a*sqrt(3) * 0.5, a * 0.5);//���ϵ�

	glVertex2f(y*sqrt(3) * 0.5, -y * 0.5);    glVertex2f(a*sqrt(3) * 0.5, -a* 0.5);//���µ�

	glEnd();

}

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

//	drawHexagon_line(y);

	for (int i = 0; i < 5; i++) // ��i��,j��
	{
		glLoadIdentity(); //����ģ����ͼ����

		glTranslatef(-i % 2 * 0.5 * sqrt(3) * a, i * -1.5 * a, 0.0);  //��һ�� ��i%2 ��һ��������һ�в���

		drawHexagon_line(y);

		for (int j = 1; j < 5; j++) //һ�л�5�����У�
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

	glTranslatef(0.5*sqrt(3) * a, -1.5 * a, 0.0);//��Ϊԭʼλ��

	drawHexagon_line(y2);

	for (int i = 0; i < 5; i++)  //i��j��
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
		Display();
	}
	else if (y > a)
	{
		y2 = 2 * a - y;

		Display2();

		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

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

	glutInitWindowSize(800, 800);//������С����

	glutCreateWindow("����");

	Init();

	glutDisplayFunc(Display);

	glutIdleFunc(&myIdle);

	glutMainLoop();

	return 0;

}
