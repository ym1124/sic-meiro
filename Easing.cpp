#include "..\Header\Easing.h"
#include <DirectXMath.h>

float Easing::Ease(int type, int func, float t, float b, float c, float d) //(time:現在時間	begin:開始地点	change:移動量	duration:必要時間)
{
	t /= d;
	if (t > 1.0) 
	{
		t = 1.0;
	}
	else if (t < 0.0) 
	{
		t = 0.0;
	}

	return Type(type, func, t)*c + b;
}

float Easing::Type(int type, int func, float t)
{
	switch (type) {
	case _IN:
		return Function(func, t);

	case _OUT:
		return 1 - Function(func, 1 - t);

	case _IN_OUT:
		if (t < 0.5) return Function(func, 2 * t) / 2;
		return 1 - Function(func, 2 - 2 * t) / 2;

	default:
		return -1;
	}
}

float Easing::Function(int func, float t)
{
	switch (func) {
	case LINEAR:		return Linear(t);
	case SMOOTH:		return Smooth(t);
	case QUAD:			return Quad(t);
	case CUBIC:			return Cubic(t);
	case QUART:			return Quart(t);
	case SINE:			return Sine(t);
	case CIRC:			return Circ(t);
	case EXP:			return Expo(t);
	case BACK:			return Back(t);
	case SOFT_BACK:		return SoftBack(t);
	case ELASTIC:		return Elastic(t);
	case BOUNCE:		return Bounce(t);

	default:			return -1;
	}
}

float Easing::Linear(float t)
{
	return t;
}

float Easing::Smooth(float t)
{
	return t*t*(3 - t) / 2;
}

float Easing::Quad(float t)
{
	return t*t;
}

float Easing::Cubic(float t)
{
	return  t*t*t;
}

float Easing::Quart(float t)
{
	return t*t*t*t;
}

float Easing::Sine(float t)
{
	return static_cast<float>(1 - cos(t*3.141592654f / 2));
}

float Easing::Circ(float t)
{
	return static_cast<float>(1 - sqrt(max(0, 1 - t*t)));
}

float Easing::Expo(float t)
{
	return static_cast<float>(pow(2, (t - 1) * 10));
}

float Easing::Back(float t)
{
	return static_cast<float>(t*t*(2.70158*t - 1.70158));
}

float Easing::SoftBack(float t)
{
	return t*t*(2 * t - 1);
}

float Easing::Elastic(float t)
{
	return 56*t*t*t*t*t - 105*t*t*t*t + 60*t*t*t - 10*t*t;
}

float Easing::Bounce(float t)
{
	float pow2, bounce = 4;

	while (t < ((pow2 = static_cast<float>(pow(2, --bounce))) - 1) / 11);

	return static_cast<float>(1 / pow(4, 3 - bounce) - 7.5625*pow((pow2*3 - 2) / 22 - t, 2));
}
