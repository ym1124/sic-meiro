#include "skin_mesh.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, float4 bone_weights : WEIGHTS, uint4  bone_indices : BONES)
{
	VS_OUT vout;

	float3 p = { 0, 0, 0 };
	float3 n = { 0, 0, 0 };

	//int i = 0; 
	for (int i = 0; i < 4; i++) {
		p += (bone_weights[i] * mul(position, bone_transforms[bone_indices[i]])).xyz;
		n += (bone_weights[i] * mul(float4(normal.xyz, 0), bone_transforms[bone_indices[i]])).xyz;
	}
	position = float4(p, 1.0f);
	normal = float4(n, 0.0f);

	vout.position = mul(position, world_view_projection);

	//float4 influence = { 0, 0, 0, 1 };
	//for (int j = 0; j < 4; j++) {
	//	float weight = bone_weights[j];
	//	if (weight > 0.0f) {
	//		switch (bone_indices[j]) {
	//		case 0: influence.r = weight; break;
	//		case 1: influence.g = weight; break;
	//		case 2: influence.b = weight; break;
	//		}
	//	}
	//}
	//vout.color = influence;

	normal.w = 0;
	float4 N = normalize(mul(normal, world));
	float4 L = normalize(-light_direction);

	vout.color = material_color * max(0, dot(L, N));
	vout.color.a = material_color.a;

	vout.uv = uv;
	return vout;
}