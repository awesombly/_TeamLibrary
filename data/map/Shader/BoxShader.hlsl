SamplerState sample0	: register (s0);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};

struct VS_IN
{
	float3 p	: POSITION;
	float3 n	: NORMAL;
	float4 c    : COLOR;
	float2 t	: TEXCOORD;
};
struct VS_OUT
{
	float4 p    : SV_POSITION;
	float3 n	: NORMAL0;
	float4 c	: COLOR0;
	float2 t	: TEXCOORD0;
};

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.p = float4(input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = input.n;
	output.c = input.c;
	output.t = input.t;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	return input.c;
}