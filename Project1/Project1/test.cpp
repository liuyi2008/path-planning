#include <gl/glut.h>
#include<math.h>
#include <stdlib.h>//exit
#include <vector>
using std::vector;

struct point {

	float x, y;

}p1;

//vector<vector<point> >coord1 = { {{1,1},{2,2},{3,3}},{{1,2},{2,3},{3,4}},{{11,11},{22,22},{33,33}} };
vector<vector<point> >coord1;
vector<point>coord2;
void getcoord1(point A, point B, int i)  //把两点间的点存在coord1[j]中
{
	float xmin, xmax, y, ymin, ymax;
	float x1, x2, y1, y2;
	x1 = A.x;
	y1 = A.y;
	x2 = B.x;
	y2 = B.y;

	float a = (y2 - y1) / (x2 - x1);
	float b = y1 - x1 * a;

	if (A.x > B.x)
	{
		xmin = x2;
		ymin = x2;
		xmax = x1;
		ymax = y1;
	}
	else
	{
		xmin = x1;
		ymin = y1;
		xmax = x2;
		ymax = y2;

	}
	//p = { xmin , ymin };
	//coord.push_back(p);
	for (; xmin < xmax; xmin++)
	{
		y = a * xmin + b;
		p1 = { roundf(xmin) , roundf(y) };
		coord1[i].push_back(p1);

	}
	p1 = { roundf(xmax) , roundf(ymax) };
	coord1[i].push_back(p1);

}

point trans(point p)  //坐标缩放平移
{
	int a = 100;  //放缩
	int b = 0;  //平移
	p = { a * p.x + b , a * p.y + b };
	return p;
}

void triangle()
{
	float Thickness = 0.2; // 一层的厚度

	float a = 1.0f;//单位距离

	float width = 2; //喷头的宽度

	int i = 0; // 第i层，由i和w一起决定点A、B、C的位置

	int m = 0;  //行
	int n = 0;  //列




	for (m; m < 3; m++)
	{
		for (n; n < 3; n++)
		{
			point A = { m*0.5*a + n * a + 0.5*a , m*0.5*sqrt(3)*a + sqrt(3) / 3 * (3 / 2 * a - 2 * sqrt(3)*width*i) };

			point B = { m*0.5*a + n * a + sqrt(3)*width*i ,m * 0.5*sqrt(3)*a + width * i };

			point C = { m*0.5*a + n * a + a - sqrt(3)*width*i ,m*0.5*sqrt(3)*a + width * i };

			point D = { m*0.5*a + n * a + 0.5*a ,m*0.5*sqrt(3)*a + 0.5*sqrt(3)*a };

			point E = { m * 0.5 * a + n * a + 0 , m * 0.5 * sqrt(3) * a + 0 };

			point F = { m*0.5*a + n * a + a , m*0.5*sqrt(3)*a + 0 };
			printf("E.x=%f,E.y=%f\t",trans(E).x, trans(E).y);
			printf("B.x=%f,B.y=%f\t", trans(B).x, trans(B).y);
			printf("m=%d,n=%d\n", m, n);
		}
		n = 0;
	}
}

void main()
{
	//getcoord1({ 1,1 }, { 3,3 }, 0);
	//getcoord1({ 3,3 }, { 5,5 }, 0);
	//getcoord1({ 5,5 }, { 7,7 }, 0);

	//getcoord1({ 1,2 }, { 2,3 }, 1);
	//getcoord1({ 3,4 }, { 4,5 }, 1);
	//getcoord1({ 6,7 }, { 8,9 }, 1);

	//coord1.push_back(coord2);
	//coord2.push_back({ 1,1 });
	triangle();



}