Texture2D g_txNormal: register (t0); // Normal
Texture2D g_txHovered : register(t1); // 올린상태
Texture2D g_txPressed : register(t2); // 누른상태

SamplerState samLinear: register (s0);

cbuffer cb0 : register(b0)
{
	matrix g_matWorld : packoffset(c0);
	matrix g_matView: packoffset(c4);
	matrix g_matProj: packoffset(c8);
	float4 g_MeshColor : packoffset(c12);
};
struct VS_INPUT
{
	float3 p  : POSITION;
	float3 n  : NORMAL;
	float4 c  : COLOR;
	float2 t  : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p  : SV_POSITION;
	float3 n  : NORMAL;
	float4 c  : COLOR0;
	float2 t  : TEXCOORD0;
};
// Vertex Shader
VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(float4(vIn.p, 1.0f), g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = vIn.n;
	output.t = vIn.t;
	output.c = g_MeshColor;
	return output;
}
// Pixel Shader
float4 PS(VS_OUTPUT vIn) : SV_Target
{
	float4 txColor = g_txNormal.Sample(samLinear, vIn.t);
	return txColor * vIn.c;
	return (txColor * (1.0f - txColor.a) + vIn.c * txColor.a);
	return (vIn.c * (1.0f - vIn.c.a) + txColor * vIn.c.a) *vIn.c;
	//return vIn.c * (1.0f - txColor.a) + txColor * vIn.c.a;
	return vIn.c * (1.0f - txColor.a) + txColor * txColor.a;
	//return vIn.c * (vColor * vIn.c.a);
}
float4 PS_PARTICLE(VS_OUTPUT vIn) : SV_Target
{
	float4 txColor = g_txNormal.Sample(samLinear, vIn.t);
	return (txColor * (1.0f - txColor.a) + vIn.c * txColor.a);
}
float4 PS_COLOR(VS_OUTPUT vIn) : SV_Target
{
	return vIn.c;
}
float4 PS_Sphere(VS_OUTPUT vIn) : SV_Target
{
	float fdist = (vIn.t.x * vIn.t.x) + (vIn.t.y * vIn.t.y);
	if (fdist < 1.0f)
	{
		return vIn.c;
	}
	else
	{
		return  float4(0,0,0,0);
	}
}
float4 PS_BLACK(VS_OUTPUT vIn) : SV_Target
{
	return float4(0,0,0,1);
}