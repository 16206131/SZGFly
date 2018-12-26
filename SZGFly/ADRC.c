#include <ADRC.h>

float fhan(float x1, float x2, float r, float h)
{
	float d, y;
	float a, a0, a1, a2, Sy, Sa;
	
	d = r*h*h;
	a0 = h*x2;
	y = x1 + a0;
	a1 = cb_sqrt(d*(d + 8*cb_abs(y)));
	a2 = a0 + sign(y)*(a1 - d) / 2;
	Sy = (sign(y + d) - sign(y - d)) / 2;
	a = (a0 + y - a2)*Sy + a2;
	Sa = (sign(a + d) - sign(a - d)) / 2;
}