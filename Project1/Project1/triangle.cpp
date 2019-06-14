#include <gl/glut.h>
#include <vector>
#include<math.h>
using std::vector;

float y = 1.0f;//��׼����

float y2;//��׼����

float dy = 0.001f;//�任��

float a = 3.0f;//��ֵ

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

	gluOrtho2D(-5.0, 5.0, -5.0, 5.0); //������ʾ�ķ�Χ��X:-5.0~5.0, Y:-5.0~5.0

	glMatrixMode(GL_MODELVIEW);

}

void Reshape(int w, int h)

{

	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //���������ߴ�

	glMatrixMode(GL_PROJECTION); //ָ����ǰ����ͶӰ�����ջ

	glLoadIdentity(); //����ͶӰ����

	//ָ��͸��ͶӰ�Ĺ۲�ռ�

	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

}

void drawTriangle_line(float y) //����������ԭ�㣬�߳�Ϊ2��������

{
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP); //�µ���˫

	glVertex2f(0.0f, -y);//�µ�

	glVertex2f(-y*sqrt(3) * 0.5, y * 0.5);//���

	glVertex2f(y*sqrt(3) * 0.5, y * 0.5);//�ҵ�

	glEnd();

	glBegin(GL_LINES); //������

	glVertex2f(0.0f, -y); glVertex2f(0.0f, -a); //�µ�

	glVertex2f(-y * sqrt(3) * 0.5, y * 0.5); glVertex2f(-a*sqrt(3) * 0.5 ,a * 0.5); //���  sqrt()

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

void Display(void)

{
	glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

	//gluLookAt(0.0 ,0.0 ,0.0 ,5.0 ,-5.0 ,0.0 ,0.0 ,1.0 ,0.0 );

	glLoadIdentity(); //����ģ����ͼ����
 
	drawTriangle_line(y);

	for (int i = 0; i < 5; i++)
	{
		glTranslatef(i * sqrt(3) * a, 0.0, 0.0);

		drawTriangle_line(y);

	}

	glLoadIdentity(); //�ָ���ʼ����ϵ

	glTranslatef(0.5 * sqrt(3) * a, -0.5 * a - a, 0.0);  //��һ��

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

void myIdle(void) //�ڿ���ʱ���ã��ﵽ����Ч��

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

		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����Ȼ�����

	}
	else if (y2 == 0)

		y = 1.0f;

}

int main(int argc, char *argv[])

{

	glutInit(&argc, argv);

	//����ʹ��RGB��ɫ��˫�������Ȼ���

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(1000, 600);

	glutCreateWindow("����");

	Init();

	glutDisplayFunc(Display);

	//glutReshapeFunc(Reshape);

	glutIdleFunc(&myIdle);

	glutMainLoop();

	return 0;

}
