﻿#include<windows.h>
#include<GL/glut.h>
#include<iostream>
#include <conio.h>
using namespace std;
const int POINTNUM = 7;      //多边形点数.
const int Width = 10;

/******定义结构体用于活性边表AET和新边表NET***********************************/
typedef struct XET
{
	float x;
	float dx, ymax;
	XET* next;
} AET, NET;

/******定义点结构体point******************************************************/
struct point
{
	float x;
	float y;
} polypoint[POINTNUM] = { {250,50},{500,150},{550,400},{250,450},{100,350},{100,100},{120,30} };//多边形顶点



void PolyScan()
{
	/******计算最高点的y坐标(扫描到此结束)****************************************/
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
	/*******初始化AET表***********************************************************/
	AET *pAET = new AET;  /*在堆上创建一个匿名的AET变量，用指针pAET标识使用*/
	pAET->next = NULL;

	/******初始化NET表************************************************************/
	NET *pNET[1024];
	for (i = MinY; i <= MaxY; i++)
	{
		pNET[i] = new NET;  //pNET是头节点
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //赋值的窗口显示.
	glColor3f(1.0, 0.0, 0.0);   //设置直线的颜色红色
	//glLineWidth(1);
	//glBegin(GL_LINE_LOOP);
	/******扫描并建立NET表（第一次出现的边）*********************************************************/

	for (i = MinY; i <= MaxY; i++)               //每条扫描线都要遍历一遍多边形所有的点
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段。多边形点是有顺序的
				if (polypoint[(j - 1 + POINTNUM) % POINTNUM].y > polypoint[j].y)
				{
					NET *p = new NET;
					p->x = polypoint[j].x;
					p->ymax = polypoint[(j - 1 + POINTNUM) % POINTNUM].y;
					p->dx = Width * ((polypoint[(j - 1 + POINTNUM) % POINTNUM].x - polypoint[j].x) / (polypoint[(j - 1 + POINTNUM) % POINTNUM].y - polypoint[j].y));
					p->next = pNET[i]->next; //p->next=NULL;
					pNET[i]->next = p;  //扫描线i的下一个指向p
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
	/******建立并更新活性边表AET*****************************************************/
	for (i = MinY; i <= MaxY; i += Width)
	{
		//计算新的交点x,更新AET
		NET *p = pAET->next; // NET *p = new NET;  p = pAET->next; 
		//if (pAET->next)printf("%d,i=%d\n", pAET->next, i); else printf("NULL,i=%d\n", i);
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//更新后新AET先排序*************************************************************/
		//断表排序,不再开辟空间
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
		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
		//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
		/******配对填充颜色***************************************************************/

		p = pAET->next;
		while (p && p->next)
		{
			if (i == MinY) //当Y坐标为最低点，设置为基线
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
			if ((i / 10) % 2 == 0 && i != MinY) //当Y坐标为偶数，X坐标向左移（右边的X减少）
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x - 10;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
				glEnd();
			}
			if ((i / 10) % 2 == 1 && i != MinY)//当Y坐标为奇数，X坐标向右移（左边的X减少）
			{
				float j = p->x + 10;//这里平移
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i); glVertex2i(static_cast<int>(h), i);
				glVertex2i(static_cast<int>(j), i - 2); glVertex2i(static_cast<int>(h), i - 2);
				glEnd();
			}
			p = p->next;//考虑端点情况

		}

	}


	//glEnd();
	//glFlush();

}


void PolyScan1()
{
	/******计算最高点和最低点的y坐标(扫描到此结束)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******初始化AET表***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******初始化NET表************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //赋值的窗口显示.
	glColor3f(1.0, 0.0, 0.0);             //设置直线的颜色红色
	glLineWidth(1);
	glBegin(GL_LINES);
	/******扫描并建立NET表*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
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
	/******建立并更新活性边表AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//计算新的交点x,更新AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//更新后新AET先排序*************************************************************/
		//断表排序,不再开辟空间
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
		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
		//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
		/******配对填充颜色***************************************************************/

		p = pAET->next;
		while (p && p->next)
		{
			if (i == MinY)
			{
				float j = p->x;
				glVertex2i(static_cast<int>(j), i);

			}

			if ((i / 10) % 2 == 0 && i != MinY) //当Y坐标为偶数，X坐标向左移（右边的X减少）
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x - 10;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i - 2);
				glVertex2i(static_cast<int>(j), i);

				//glEnd();
			}

			p = p->next;//考虑端点情况

		}

		//glFlush();
	}
	glEnd();
}


void PolyScan2()
{
	/******计算最高点和最低点的y坐标(扫描到此结束)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******初始化AET表***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******初始化NET表************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //赋值的窗口显示.
	glColor3f(1.0, 0.0, 0.0);             //设置直线的颜色红色
	glLineWidth(1);
	glBegin(GL_LINES);
	/******扫描并建立NET表*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
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
	/******建立并更新活性边表AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//计算新的交点x,更新AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//更新后新AET先排序*************************************************************/
		//断表排序,不再开辟空间
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
		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
		//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
		/******配对填充颜色***************************************************************/

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

			if ((i / 10) % 2 == 1 && i != MinY) //当Y坐标为奇数，X坐标向右移（左边的X增加）
			{
				float j = p->x;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(h), i - 2);
				glVertex2i(static_cast<int>(h), i);

				//glEnd();
			}

			p = p->next;//考虑端点情况

		}



	}
	glEnd();
}


void PolyScan3()
{
	/******计算最高点和最低点的y坐标(扫描到此结束)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******初始化AET表***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******初始化NET表************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //赋值的窗口显示.
	glColor3f(1.0, 0.0, 0.0);             //设置直线的颜色红色
	glLineWidth(1);
	glBegin(GL_LINES);
	/******扫描并建立NET表*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
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
	/******建立并更新活性边表AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//计算新的交点x,更新AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//更新后新AET先排序*************************************************************/
		//断表排序,不再开辟空间
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
		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
		//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
		/******配对填充颜色***************************************************************/

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

			if ((i / 10) % 2 == 1 && i != MinY) //当Y坐标为奇数，X坐标向右移（左边的X增加）
			{
				float j = p->x + 10;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(j), i - 2);
				glVertex2i(static_cast<int>(j), i);

				//glEnd();
			}

			p = p->next;//考虑端点情况

		}



	}
	glEnd();
}


void PolyScan4()
{
	/******计算最高点和最低点的y坐标(扫描到此结束)****************************************/
	int MaxY = 0, MinY = 1024;
	int i;
	for (i = 0; i < POINTNUM; i++)
	{
		if (polypoint[i].y > MaxY)
			MaxY = polypoint[i].y;
		if (polypoint[i].y < MinY)
			MinY = polypoint[i].y;
	}

	/*******初始化AET表***********************************************************/
	AET *pAET = new AET;
	pAET->next = NULL;

	/******初始化NET表************************************************************/
	NET *pNET[1024];
	for (i = 0; i <= MaxY; i++)
	{
		pNET[i] = new NET;
		pNET[i]->next = NULL;
	}
	//glClear(GL_COLOR_BUFFER_BIT);        //赋值的窗口显示.
	glColor3f(1.0, 0.0, 0.0);             //设置直线的颜色红色
	glLineWidth(1);
	glBegin(GL_LINES);
	/******扫描并建立NET表*********************************************************/
	for (i = 0; i <= MaxY; i++)
	{
		for (int j = 0; j < POINTNUM; j++)
			if (polypoint[j].y == i)
			{
				//一个点跟前面的一个点形成一条线段，跟后面的点也形成线段
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
	/******建立并更新活性边表AET*****************************************************/
	for (i = 0; i <= MaxY; i += Width)
	{
		//计算新的交点x,更新AET
		NET *p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		//更新后新AET先排序*************************************************************/
		//断表排序,不再开辟空间
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
		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
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
		//将NET中的新点加入AET,并用插入法按X值递增排序**********************************/
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
		/******配对填充颜色***************************************************************/

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

			if ((i / 10) % 2 == 0 && i != MinY) //当Y坐标为奇数，X坐标向右移（左边的X增加）
			{
				float j = p->x + 10;
				//glVertex2i(static_cast<int>(j), i-5); glVertex2i(static_cast<int>(j), i+5);
				float h = p->next->x - 10;//这里平移
				//glVertex2i(static_cast<int>(h), i-5); glVertex2i(static_cast<int>(h), i+5);
				//glBegin(GL_LINES);
				glVertex2i(static_cast<int>(h), i - 2);
				glVertex2i(static_cast<int>(h), i);

				//glEnd();
			}

			p = p->next;//考虑端点情况

		}



	}
	glEnd();
}


void init(int argc, char** argv)
{
	glutInit(&argc, argv);  //I初始化 GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  //设置显示模式：单个缓存和使用RGB模型
	glutInitWindowPosition(50, 100);  //设置窗口的顶部和左边位置
	glutInitWindowSize(800, 600);  //设置窗口的高度和宽度
	glutCreateWindow("Scan Program");

	glClearColor(1.0, 1.0, 1.0, 0); //窗口背景颜色为白色
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 600, 0, 450);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(0.8, 0.0, 0.0);

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3i(250, 50, 0);
	glVertex3i(500, 150, 0);
	glVertex3i(550, 400, 0);
	glVertex3i(250, 450, 0);
	glVertex3i(100, 350, 0);
	glVertex3i(100, 100, 0);
	glVertex3i(120, 30, 0);
	glEnd();

	//glColor3f(1.0f, 1.0f, 1.0f);
	//glPointSize(2);
	//glLineWidth(5.0f);
	//glBegin(GL_LINE_STRIP);
	PolyScan();
	//PolyScan1();//左边
	//PolyScan2();//右边
	//PolyScan3();
	//PolyScan4();
	//glEnd();

	//glColor3f(0.0, 0.4, 0.2);
	//glBegin(GL_LINES);
	//PolyScan1();
	//glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
	int ch;
	while (1)
	{
		if (_kbhit())
		{//如果有按键按下，则_kbhit()函数返回真
			ch = _getch();//使用_getch()函数获取按下的键值
			cout << ch;
			if (ch == 27) {  break; }//当按下ESC时循环，ESC键的键值时27.
		}
	}


	init(argc, argv);
	glutDisplayFunc(myDisplay);        //图形的定义传递给window.
	glutMainLoop();

	return 0;
}
