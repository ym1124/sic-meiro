#include "geometric_primitive.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL)
{
	VS_OUT vout;
	vout.position = mul(position, world_view_projection);

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-light_direction);

	//vout.color = material_color * max(0, dot(L, N));
	//vout.color.a = material_color.a;

	vout.color = material_color;

	return vout;
}