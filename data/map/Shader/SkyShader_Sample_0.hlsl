static const int	g_iNumTexture = 6;

Texture2D			g_txDiffuseArray[g_iNumTexture]	: register (t0);
SamplerState		sample0							: register (s0);

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

struct PS_IN
{
	float4 p		: SV_Position;
	float3 n		: NORMAL0;
	float4 c		: COLOR0;
	float2 t		: TEXCOORD0;
	uint   id		: SV_PrimitiveID;
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

float4 PS(PS_IN input) : SV_Target
{
	float4 fColor = float4(1,1,1,1);
	uint index = 0;
	index = input.id / 2;
	switch (index)
	{
		case 0: fColor = g_txDiffuseArray[0].Sample(sample0, input.t); break;
		case 1: fColor = g_txDiffuseArray[1].Sample(sample0, input.t); break;
		case 2: fColor = g_txDiffuseArray[2].Sample(sample0, input.t); break;
		case 3: fColor = g_txDiffuseArray[3].Sample(sample0, input.t); break;
		case 4: fColor = g_txDiffuseArray[4].Sample(sample0, input.t); break;
		case 5: fColor = g_txDiffuseArray[5].Sample(sample0, input.t); break;
	}
	return fColor;
}