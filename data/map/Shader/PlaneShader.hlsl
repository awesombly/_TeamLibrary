Texture2D g_txDiffuse	: register (t0);
SamplerState sample0	: register (s0);

cbuffer cb0
{
	float4x4 g_matWorld		: packoffset(c0);
	float4x4 g_matView		: packoffset(c4);
	float4x4 g_matProj		: packoffset(c8);
};

struct VS_IN
{
	float3 p		: POSITION;
	float3 n		: NORMAL;
	float4 c		: COLOR;
	float2 t	    : TEXCOORD;
};

struct VS_OUT
{
	float4 p		: SV_Position;
	float3 n		: NORMAL0;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
};


VS_OUT VS(VS_IN input)
{
	VS_OUT output;
	output.p = float4(input.p, 1.0f);
	output.n = input.n;
	output.c = input.c;
	output.t = input.t;

	return output;
}

float4 PS(VS_OUT input) : SV_Target
{
	return input.c * g_txDiffuse.Sample(sample0,input.t);
}