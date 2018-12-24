#define NORMAL_VECTOR

SamplerState sample0		: register (s0);

cbuffer cbMatrix: register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};

cbuffer cbLight: register(b1)
{
	matrix g_matWorldInverse	: packoffset(c0);
	float4 g_vDiffuseMaterial	: packoffset(c4);
	float4 g_vAmbientMaterial	: packoffset(c5);
	float4 g_vSpecularMaterial	: packoffset(c6);
	float4 g_vDiffuseLightColor : packoffset(c7);
	float4 g_vAmbientLightColor : packoffset(c8);
	float4 g_vSpecularLightColor: packoffset(c9);
	float3 g_vLightDirection	: packoffset(c10);
	float  g_fDamping : packoffset(c10.w);	// ∂Û¿Ã∆Æ ∫§≈Õ ∞®ºË
	float3 g_vSightDirection	: packoffset(c11);
	float  g_fIntensity : packoffset(c11.w);	// Ω√º±∫§≈Õ ºº±‚
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

float4 Diffuse(float3 vNormal)
{
	float fDot = max(0, dot(vNormal, normalize(g_vLightDirection)));
	float4 vDiffuse = (g_vAmbientMaterial * g_vAmbientLightColor) + (g_vDiffuseLightColor * g_vDiffuseLightColor * fDot);
	return vDiffuse;
}

float4 Specular(float3 vNormal)
{
	float  fPower = 0.0f;
	float3 vReflect = float3(0.0f, 0.0f, 0.0f);
#ifndef NORMAL_VECTOR
	vReflect = reflect(-g_vLightDirection, vNormal);
	fPower = pow(saturate(dot(vReflect, -g_vSightDirection)), g_fIntensity);
#else
	vReflect = normalize(g_vLightDirection + -g_vSightDirection);
	fPower = pow(saturate(dot(vNormal, vReflect)), g_fIntensity);
#endif
	float4 vSpecular = g_vSpecularMaterial * g_vSpecularLightColor * fPower;
	return vSpecular;
}

VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	output.p = float4(input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.c = input.c;
	output.n = normalize(mul(input.n, (float3x3)g_matWorldInverse));
	output.t = input.t;

	return output;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float4 vDiffuse = Diffuse(input.n);
	float4 vSpecular = Specular(input.n);
	float4 vFinalColor = input.c * (vDiffuse + vSpecular);
	return vFinalColor;
}