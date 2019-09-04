#include "test.hlsli"

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.posw = mul(input.pos, World);
	output.pos = mul(input.pos, WorldViewProjection);

	output.norw.xyz = normalize(mul(input.nor.xyz, World));

	return output;
}