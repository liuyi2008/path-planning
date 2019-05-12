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
	int x;
	int dx, ymax;
	XET* next;
} AET, NET;

/******�����ṹ��point******************************************************/
struct point
{
	int x;
	int y;
}p;                           //polypoint[POINTNUM] = { {250,50},{500,150},{550,400},{250,450},{100,350},{100,100},{120,30} };//����ζ���

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
	//printf("%d,%d", MaxY, MinY);
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
	/******ɨ�貢����NET����һ�γ��ֵıߣ�*********************************************************/

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
					p->dx = Width *((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next; //p->next=NULL;
					pNET[i]->next = p;  //ɨ����i����һ��ָ��p
				}
				if (polypoint[(j + 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j + 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width *((polypoint[(j + 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j + 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
	}
	/******���������»��Ա߱�AET*****************************************************/
	for (i = MinY; i <= MaxY; i+= Width)
	{
		//�����µĽ���x,����AET
		NET *p = pAET->next; // NET *p = new NET;  p = pAET->next; 
		//if (pAET->next)printf("%d,i=%d\n", pAET->next, i); else printf("NULL,i=%d\n", i);
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
			if (i == MinY) //��Y����Ϊ��͵㣬����Ϊ����
			{
				float j = p->x; //printf("j = %f", j);
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				//float h = p->next->x; //printf("h = %f", h);
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				glPointSize(3);
				glBegin(GL_POINTS);
				glVertex2i(static_cast<int>(j), i);
				glEnd();
			}
			if ((i / 10) % 2 == 0&& i != MinY) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x-10;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
				glEnd();
			}
			if ((i / 10) % 2 == 1 && i != MinY)//��Y����Ϊ������X���������ƣ���ߵ�X���٣�
			{
				float j = p->x+10;//����ƽ��
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//����ƽ��
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
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
			if(i == MinY)
			{
				float j = p->x;
			   glVertex2i(static_cast<int>(j), i);
			   
			}
			
			if ((i / 10) % 2 == 0&& i!= MinY ) //��Y����Ϊż����X���������ƣ��ұߵ�X���٣�
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
				float j = p->x+10;
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
				float h = p->next->x-10;//����ƽ��
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


void ControlPoint()

{

	glPointSize(5);

	for (int i = 0; i < polypoint.size(); i++)

	{

		glBegin(GL_POINTS);

		glColor3f(0.0f, 1.0f, 0.0f); glVertex2i(polypoint[i].x, wh-polypoint[i].y);

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
		glVertex2i(polypoint[i].x, wh - polypoint[i].y);
		
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
		int x = wx;
		int y = wy;
		p = { x,y };
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
	glutInitWindowSize(800, 600);  //���ô��ڵĸ߶ȺͿ��
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
	printf("x:%d,y:%d", polypoint[i].x, wh - polypoint[i].y);
	return 0;
}
