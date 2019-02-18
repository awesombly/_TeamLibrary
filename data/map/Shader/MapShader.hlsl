SamplerState sample0		: register (s0);
Texture2D	 g_txDiffuse	: register (t0);

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4	g_vLight		: packoffset(c12);
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

float Diffuse(float3 vNormal)
{
	float3 vLight = float3(g_vLight.x, g_vLight.y, g_vLight.z);
	float fDot = max(0, dot(vNormal, normalize(-vLight)));
	return fDot;
}

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.p = float4(input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	float fDot = Diffuse(input.n);
	output.c = float4(fDot, fDot, fDot, 1.0f);
	output.n = input.n;
	output.t = input.t*10.0f;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float4 vColor = input.c *g_txDiffuse.Sample(sample0,input.t);
	return vColor;
}