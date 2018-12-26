#include <math.h>

int sign(float x)
{
	if (x > 0)
		return 1;
	else if (x == 0)
		return 0;
	else
		return -1;
}

float abs(float x)
{
	if (x >= 0)
		return x;
	else
		return -x;
}

double sqrt(double x)
{
	double down, mid, up;
	double back;
	if (x < 0)    //���󷵻�-1;
		return -1;
	if (x > 1)  //����1
		{
			up = x; //������;
			down = up / 2;
			while (down*down > x)   //�ҳ�������
			{
				down = down / 2;
			}
		}		
	else if (x == 1) //����1
		return x;
	else               //С��1
		{
			down = x;//�����䣻
			up = down * 2;
			while (up*up < x)  //�ҳ������䣻
			{
				up = up * 2;
			}
		}	
	mid = (up + down) / 2;
	back = abs(mid*mid - x);
	while ( back>= 0.0001)
	{
		if (mid*mid > x)
			up = mid;
		else if (mid*mid == x)
			return mid;
		else
			down = mid;
		mid = (down + up) / 2;
		back = abs(mid*mid - x);
	}

	return mid;
}
