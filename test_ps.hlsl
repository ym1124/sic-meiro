#include "test.hlsli"

float4 main(PS_IN input, VS_OUT output) : SV_Target
{
	float3 dir;
float  len;
float  colD = 0.0f;
float  colA = 0.0f;
float3 color = { 0.0f,0.0f,0.0f };

input.norw.w = 0.0f;
float4 N = input.norw;
float4 L = normalize(-DirectionalLight);

color = ObjColor * max(0, dot(L, N));

for (int i = 0; i < 7; i++)
{
	if (!LightExist[i].x)
		continue;
	//頂点から光源へのベクトル
	dir = Light[i].xyz - input.posw.xyz;

	//点光源の距離
	len = length(dir);

	//点光源の方向をnormalize
	dir = dir / len;

	//拡散
	colD = saturate(dot(normalize(input.norw.xyz), dir));
	//減衰
	if(!i)
		colA = saturate(1.0f / (Attenuation[0].x + Attenuation[0].y * len + Attenuation[0].z * len * len));
	else
		colA = saturate(1.0f / (Attenuation[1].x + Attenuation[1].y * len + Attenuation[1].z * len * len));

	//color += color.xyz*colD * colA;
	color += ObjColor*colD * colA;
	colD = 0.0f;
	colA = 0.0f;
}

//color = float4(0.6f, 0.0f, 0.0f, 1.0f);

return float4(color.xyz*0.8f, 1.0f);
}