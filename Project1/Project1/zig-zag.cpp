#include<windows.h>
#include<GL/glut.h>
#include<iostream>
#include <conio.h>
#include<vector>
using std::vector;
//using namespace std;
//const int POINTNUM = 7;  //����ε���.
int POINTNUM;
const int Width = 10;
GLfloat x = 0.0;
GLfloat y = 0.0;
GLsizei wh = 800, ww = 800;
bool scan = false;


/******����ṹ�����ڻ��Ա߱�AET���±߱�NET***********************************/
typedef struct XET
{
	float x;
	float dx, ymax;
	XET* next;  //next��ָ��XET���͵�ָ��
} AET, NET;

/******�����ṹ��point******************************************************/
struct point
{
	float x;
	float y;
}p;            //polypoint[7] = { {250,50},{500,150},{550,400},{250,450},{100,350},{100,100},{120,30} };//����ζ���

vector<point>polypoint;

void PolyScan()
{
	/******������ߵ��y����(ɨ�赽�˽���)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}
	//printf("MaxY=%d,MinY=%d", MaxY, MinY);
	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;  /*�ڶ��ϴ���һ��������AET��������ָ��pAET��ʶʹ��*/
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = MinY; i <= MaxY; i++)
	{
		pNET[i] = new NET;  //pNET��ͷ�ڵ�
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);   //����ֱ�ߵ���ɫ��ɫ
	//glLineWidth(1);
	//glBegin(GL_LINE_LOOP);
	/******ɨ�貢����NET������һ�γ��ֵıߣ�*********************************************************/

	for (i = MinY; i <= MaxY; i++)               //ÿ��ɨ���߶�Ҫ����һ���������еĵ�
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶Ρ�����ε�����˳���
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next; //Ϊɶ����p->next = NULL;
					pNET[i]->next = p;  //ɨ����i����һ��ָ��p
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	for (i = MinY; i <= MaxY; i++)
	{
		if (pNET[i]->next)
		{
			float a = pNET[i]->next->x;
			float b = pNET[i]->next->dx;
			float c = pNET[i]->next->ymax;
			printf("ɨ����i=%d��һ�ڵ��X = %f��DX =%f MAXY=%f\n", i, a, b, c);
			if (pNET[i]->next->next)
			{
				float a = pNET[i]->next->next->x;
				float b = pNET[i]->next->next->dx;
				float c = pNET[i]->next->next->ymax;
				printf("ɨ����i=%d�ڶ��ڵ��X = %f��DX =%f MAXY=%f\n", i, a, b, c);
			}
		}
		if (!pNET[i]->next)
		{
			printf("ɨ����i=%d�޽ڵ�\n", i);
		}
	}

	/******���������»��Ա߱�AET*****************************************************/
	for (i = MinY; i <= MaxY; i += Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next; // NET *p = new NET;  p = pAET->next; 
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//���º���AET������*************************************************************/
		//�ϱ�����,���ٿ��ٿռ�
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		AET *q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax <= i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//��NET�е��µ����AET,���ò��뷨��Xֵ��������**********************************/
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		/******��������ɫ***************************************************************/

		p = pAET->next;
		while (p && p->next)
		{
			printf("ɨ����i=%d,����һX=%f,dx=%f,ymax=%f      ", i, p->x, p->dx, p->ymax);
			printf("ɨ����i=%d,�����X=%f,dx=%f,ymax=%f\n", i, p->next->x, p->next->dx, p->next->ymax);
			if (i == MinY) //��Y����Ϊ��͵㣬����Ϊ����
			{
				float j = p->x; //printf("j = %f", j);
				glPointSize(3);
				glBegin(GL_POINTS);
				glVertex2i(static_cast<int>(j), i);
				glEnd();
			}
			if ((i / 10) % 2 == 0 && i != MinY) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
			{
				float j = p->x;
				float h = p->next->x - 10;//����ƽ��
				glBegin(GL_LINES);
				//glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				//glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2);

				glEnd();
			}
			if ((i / 10) % 2 == 1 && i != MinY)//��Y����Ϊ������X���������ƣ���ߵ�X���٣�
			{
				float j = p->x + 10;//����ƽ��				
				float h = p->next->x;//����ƽ��
				glBegin(GL_LINES);
				//glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				//glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2);
				glEnd();
			}
			p = p->next;//���Ƕ˵����

		}

	}


	//glEnd();
	//glFlush();

}


void PolyScan1()
{
	/******������ߵ����͵��y����(ɨ�赽�˽���)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);             //����ֱ�ߵ���ɫ��ɫ
	glLineWidth(1);
	glBegin(GL_LINES);
	/******ɨ�貢����NET��*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶�
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	/******���������»��Ա߱�AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//���º���AET������*************************************************************/
		//�ϱ�����,���ٿ��ٿռ�
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		AET *q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//��NET�е��µ����AET,���ò��뷨��Xֵ��������**********************************/
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		/******��������ɫ***************************************************************/

		p = pAET->next;
		while (p && p->next)
		{
			if (i == MinY)
			{
				float j = p->x;
				glVertex2i(static_cast<int>(j), i);

			}

			if ((i / 10) % 2 == 0 && i != MinY) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x - 10;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i - 2);
				glVertex2i(static_cast<int>(j), i);

				//glEnd();
			}

			p = p->next;//���Ƕ˵����

		}

		//glFlush();
	}
	glEnd();
}


void PolyScan2()
{
	/******������ߵ����͵��y����(ɨ�赽�˽���)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);             //����ֱ�ߵ���ɫ��ɫ
	glLineWidth(1);
	glBegin(GL_LINES);
	/******ɨ�貢����NET��*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶�
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	/******���������»��Ա߱�AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//���º���AET������*************************************************************/
		//�ϱ�����,���ٿ��ٿռ�
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		AET *q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//��NET�е��µ����AET,���ò��뷨��Xֵ��������**********************************/
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		/******��������ɫ***************************************************************/

		p = pAET->next;
		/*
		if (p&&p->next&&i!= MinY)
		{

			float h = p->next->x;
			glVertex2i(static_cast<int>(h), i);
		}
		else
			continue;
		*/
		while (p && p->next)
		{
			if (i == MinY)
			{
				float j = p->x;
				glVertex2i(static_cast<int>(j), i);

			}

			if ((i / 10) % 2 == 1 && i != MinY) //��Y����Ϊ������X���������ƣ���ߵ�X���ӣ�
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(h), i - 2);
				glVertex2i(static_cast<int>(h), i);

				//glEnd();
			}

			p = p->next;//���Ƕ˵����

		}



	}
	glEnd();
}


void PolyScan3()
{
	/******������ߵ����͵��y����(ɨ�赽�˽���)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);             //����ֱ�ߵ���ɫ��ɫ
	glLineWidth(1);
	glBegin(GL_LINES);
	/******ɨ�貢����NET��*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶�
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	/******���������»��Ա߱�AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//���º���AET������*************************************************************/
		//�ϱ�����,���ٿ��ٿռ�
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		AET *q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//��NET�е��µ����AET,���ò��뷨��Xֵ��������**********************************/
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		/******��������ɫ***************************************************************/

		p = pAET->next;
		/*
		if (p&&p->next&&i!= MinY)
		{

			float h = p->next->x;
			glVertex2i(static_cast<int>(h), i);
		}
		else
			continue;
		*/
		while (p && p->next)
		{

			if ((i / 10) % 2 == 1 && i != MinY) //��Y����Ϊ������X���������ƣ���ߵ�X���ӣ�
			{
				float j = p->x + 10;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i - 2);
				glVertex2i(static_cast<int>(j), i);

				//glEnd();
			}

			p = p->next;//���Ƕ˵����

		}



	}
	glEnd();
}


void PolyScan4()
{
	/******������ߵ����͵��y����(ɨ�赽�˽���)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);             //����ֱ�ߵ���ɫ��ɫ
	glLineWidth(1);
	glBegin(GL_LINES);
	/******ɨ�貢����NET��*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶�
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	/******���������»��Ա߱�AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//���º���AET������*************************************************************/
		//�ϱ�����,���ٿ��ٿռ�
		AET *tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p)
		{
			while (tq->next && p->x >= tq->next->x)
				tq = tq->next;
			NET *s = p->next;
			p->next = tq->next;
			tq->next = p;
			p = s;
			tq = pAET;
		}
		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
		AET *q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		//��NET�е��µ����AET,���ò��뷨��Xֵ��������**********************************/
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next && p->x >= q->next->x)
				q = q->next;
			NET *s = p->next;
			p->next = q->next;
			q->next = p;
			p = s;
			q = pAET;
		}
		/******��������ɫ***************************************************************/

		p = pAET->next;
		/*
		if (p&&p->next&&i!= MinY)
		{

			float h = p->next->x;
			glVertex2i(static_cast<int>(h), i);
		}
		else
			continue;
		*/
		while (p && p->next)
		{

			if ((i / 10) % 2 == 0 && i != MinY) //��Y����Ϊ������X���������ƣ���ߵ�X���ӣ�
			{
				float j = p->x + 10;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x - 10;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(h), i - 2);
				glVertex2i(static_cast<int>(h), i);

				//glEnd();
			}

			p = p->next;//���Ƕ˵����

		}



	}
	glEnd();
}


void PolyScantest()
{
	/******������ߵ��y����(ɨ�赽�˽���)****************************************/
	float MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}
	//printf("MaxY=%d,MinY=%d", MaxY, MinY);
	/*******��ʼ��AET��***********************************************************/
	AET *pAET = new AET;  /*�ڶ��ϴ���һ��������AET��������ָ��pAET��ʶʹ��*/
	pAET->next = NULL;

	/******��ʼ��NET��************************************************************/
	NET *pNET[1024];
	for (i = MinY; i <= MaxY; i++)
	{
		pNET[i] = new NET;  //pNET��ͷ�ڵ�
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //��ֵ�Ĵ�����ʾ.
	glColor3f(1.0, 0.0, 0.0);   //����ֱ�ߵ���ɫ��ɫ
	//glLineWidth(1);
	//glBegin(GL_LINE_LOOP);
	/******ɨ�貢����NET������һ�γ��ֵıߣ�*********************************************************/

	for (i = MinY; i <= MaxY; i++)               //ÿ��ɨ���߶�Ҫ����һ���������еĵ�
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//һ�����ǰ���һ�����γ�һ���߶Σ�������ĵ�Ҳ�γ��߶Ρ�����ε�����˳���
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next; //Ϊɶ����p->next = NULL;
					pNET[i]->next = p;  //ɨ����i����һ��ָ��p
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	for (i = MinY; i <= MaxY; i++)
	{
		if (pNET[i]->next)
		{
			float a = pNET[i]->next->x;
			float b = pNET[i]->next->dx;
			float c = pNET[i]->next->ymax;
			printf("ɨ����i=%d��һ�ڵ��X = %f��DX =%f MAXY=%f\n", i, a, b, c);
			if (pNET[i]->next->next)
			{
				float a = pNET[i]->next->next->x;
				float b = pNET[i]->next->next->dx;
				float c = pNET[i]->next->next->ymax;
				printf("ɨ����i=%d�ڶ��ڵ��X = %f��DX =%f MAXY=%f\n", i, a, b, c);
			}
		}
		if (!pNET[i]->next)
		{
			printf("ɨ����i=%d�޽ڵ�\n", i);
		}
	}
	/******���������»��Ա߱�AET*****************************************************/
	AET *p, *q;
	p = pNET[int(MinY)]->next;
	q = pNET[int(MinY)]->next->next;
	for (i = MinY; i <= MaxY; i += Width)
	{
		if (i < p->ymax && i < q->ymax)     //���1��2
		{
			//printf("ɨ����i=%d,����һX=%f,dx=%f,ymax=%f      ", i, p->x, p->dx, p->ymax);
			//printf("ɨ����i=%d,�����X=%f,dx=%f,ymax=%f\n", i, p->next->x, p->next->dx, p->next->ymax);
			if (i == MinY) //��Y����Ϊ��͵㣬����Ϊ����
			{
				float j = p->x; //printf("j = %f", j);
				glPointSize(3);
				glBegin(GL_POINTS);
				glVertex2f(j, i);
				glEnd();
				p->x = p->x + p->dx;
				q->x = q->x + q->dx;
				continue;
			}
			if ((i / 10) % 2 == 0 && i != MinY) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
			{
				float j = p->x;
				float h = p->next->x - 10;//������ƽ��
				glBegin(GL_LINES);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2); //������ƽ��
				glEnd();
				p->x = p->x + p->dx;
				q->x = q->x + q->dx;
				continue;
			}
			if ((i / 10) % 2 == 1 && i != MinY)//��Y����Ϊ������X���������ƣ���ߵ�X���٣�
			{
				float j = p->x + 10;//����ƽ��				
				float h = p->next->x;
				glBegin(GL_LINES);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2);
				glEnd();
				p->x = p->x + p->dx;
				q->x = q->x + q->dx;
				continue;
			}
		}
		else if (i >= p->ymax && i < q->ymax)  //���3��3.1
		{
			for (int ii = i; ii > i - Width; ii--)
			{
				if (pNET[ii]->next)
				{
					p = pNET[ii]->next;
					p->x = p->x + (i - ii) / 10 * p->dx;          //i-ii�Ǽ�϶�ľ���
					continue;
				}
			}

			if ((i / 10) % 2 == 0 && i != MinY) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
			{
				float j = p->x;
				float h = q->x - 10;//������ƽ��
				glBegin(GL_LINES);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2); //������ƽ��
				glEnd();
				p->x = p->x + p->dx;
				q->x = q->x + q->dx;
				continue;
			}
			if ((i / 10) % 2 == 1 && i != MinY)//��Y����Ϊ������X���������ƣ���ߵ�X���٣�
			{
				float j = p->x + 10;//����ƽ��				
				float h = q->x;
				glBegin(GL_LINES);
				glVertex2f(j, i); glVertex2f(h, i);
				glVertex2f(j, i - 2); glVertex2f(h, i - 2);
				glEnd();
				p->x = p->x + p->dx;
				q->x = q->x + q->dx;
				continue;
			}







		}
		else if (i < p->ymax && i >= q->ymax)
		{
			for (int ii = i; ii > i - Width; ii--)
			{
				if (pNET[ii]->next) { q = pNET[ii]->next; break; }
			}
		}



	}

}


void ControlPoint()

{

	glPointSize(5);

	for (int i = 0; i < polypoint.size(); i++)

	{

		glBegin(GL_POINTS);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(polypoint[i].x, polypoint[i].y);

		glEnd();

	}


}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);

	ControlPoint();

	glBegin(GL_POLYGON);
	for (int i = 0; i < polypoint.size(); i++)
	{
		glVertex2i(polypoint[i].x, polypoint[i].y);  //wh-y  ��ת

	}
	glEnd();
	if (scan)
	{
		POINTNUM = polypoint.size();
		PolyScan();
		PolyScan1();
		PolyScan2();
		PolyScan3();
		PolyScan4();
	}
	glutSwapBuffers();
	glutPostRedisplay();
	glFlush();
}


void mymouse(GLint button, GLint state, GLint wx, GLint wy)

{


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)

		exit(0);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)

	{
		x = wx;
		y = wy;
		printf("x:%f,y:%f      ", x, wh - y);
		float a = int(x / 10) * 10;
		float b = int((wh-y) / 10) * 10;
		printf("x:%f,y:%f      ", a, b);
		p = { a , b };
		polypoint.push_back(p);
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
		scan = true;
}


void myreshape(GLint w, GLint h)
{

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();          //��ʼ��

	glOrtho(0, w, 0, h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	ww = w;

	wh = h;

}


void init(int argc, char** argv)
{
	glutInit(&argc, argv);  //I��ʼ�� GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //������ʾģʽ�����������ʹ��RGBģ��
	glutInitWindowPosition(50, 100);  //���ô��ڵ�λ�ã��������Ͻ��������������(x,y)
	glutInitWindowSize(800, 600);  //���ô��ڵĸ߶ȺͿ���
	glutCreateWindow("Scan Program");

	glClearColor(1.0, 1.0, 1.0, 0); //���ڱ�����ɫΪ��ɫ
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 600, 0, 450);
}


int main(int argc, char** argv)
{
	init(argc, argv);
	glutDisplayFunc(myDisplay); //ͼ�εĶ��崫�ݸ�window.
	glutReshapeFunc(myreshape);
	glutMouseFunc(mymouse);
	glutMainLoop();
	return 0;
}
