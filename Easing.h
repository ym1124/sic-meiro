#pragma once

#define max(a,b)            (((a) > (b)) ? (a) : (b))

enum EASING_TYPE
{
	_IN,
	_OUT,
	_IN_OUT,
};

enum EASING_FUNC
{
	LINEAR,
	SMOOTH,
	QUAD,
	CUBIC,
	QUART,
	SINE,
	CIRC,
	EXP,
	BACK,
	SOFT_BACK,
	ELASTIC,
	BOUNCE,
};

class Easing
{
private:
	float Type(int type, int func, float t);
	float Function(int func, float t);
	Easing(){}

public:
	~Easing() {};
	
	static Easing GetInstance()
	{
		Easing instance;
		return instance;
	}

//(time:���ݎ���	begin:�J�n�n�_	change:�ړ���	duration:�K�v����)
	float Ease(int type, int func, float t, float b, float c, float d); 

	//void Draw(int type, int func, float t, float b, float c, float d, float x);
private:
	float Linear(float t);
	float Smooth(float t);
	float Quad(float t);
	float Cubic(float t);
	float Quart(float t);
	float Sine(float t);
	float Circ(float t);
	float Expo(float t);
	float Back(float t);
	float SoftBack(float t);
	float Elastic(float t);
	float Bounce(float t);
};

static Easing* const pEasing = &Easing::GetInstance();
