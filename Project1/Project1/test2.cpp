#include<iostream>
#include<vector>
using namespace std;
using std::vector;

struct point 
{
	int x;
	int y;
};

vector<point>polypoint;
void control(vector<point>polypoint) 
{
	polypoint[0].x += polypoint[0].x;
}

void mydisplay()
{
	if (polypoint.size != 0)
		control(polypoint);
}
int main(void)
{
	int x1 = 1;
	int y1 = 1;
	struct point p = { 1,2 };
	polypoint.push_back(p);
	printf("%d,%d/n", p.x,p.y);
	mydisplay();
	printf("%d,%d", p.x, p.y);
}

