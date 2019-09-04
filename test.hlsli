struct VS_IN
{
	float4 pos:POSITION;
	float4 nor:NORMAL0;
};

struct VS_OUT
{
	float4 pos:SV_POSITION;
	float4 posw:POSITION0;
	float4 norw:NORMAL0;
};

struct PS_IN
{
	float4 pos:SV_POSITION;
	float4 posw:POSITION0;
	float4 norw:NORMAL0;
};

#define LIGHT_MAX	16

cbuffer ConstantBuffer
{
	row_major float4x4 WorldViewProjection;
	row_major float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4 Light[LIGHT_MAX];
	float4 Attenuation[LIGHT_MAX];
	float4 LightColor[LIGHT_MAX];
	float4 ObjColor;
	float4 DirectionalLight;
	int4 LightExist[LIGHT_MAX];
};