#include "load.hlsli"

float4 main(VS_OUT pin) : SV_TARGET
{
	//return float4(1.0f,1.0f,1.0f,1.0f);
	return pin.color;
}