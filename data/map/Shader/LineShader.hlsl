cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};

struct VS_IN
{
	float3 p	: POSITION;
	float4 c	: COLOR;
};
struct VS_OUT
{
	float4 p    : SV_POSITION;
	float4 c	: COLOR0;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.p = float4(input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.c = input.c;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	return input.c;
}