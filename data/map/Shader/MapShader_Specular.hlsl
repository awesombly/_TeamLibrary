#define NORMAL_VECTOR

SamplerState sample0		: register (s0);
Texture2D AlphaMap		: register (t0);
Texture2D AlphaTexture1 : register (t2);
Texture2D AlphaTexture2 : register (t3);
Texture2D AlphaTexture3 : register (t4);
Texture2D AlphaTexture4 : register (t5);
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
	float  g_fDamping			: packoffset(c10.w);	// 라이트 벡터 감쇠
	float3 g_vSightDirection	: packoffset(c11);
	float  g_fIntensity			: packoffset(c11.w);	// 시선벡터 세기
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

//float4 PS(VS_OUT input) : SV_TARGET
//{
//	float4 vAlphaMap = AlphaMap.Sample(sample0,input.t);
//
//	// 눈에 보이도록하기 위해
//	if (vAlphaMap.x + vAlphaMap.y + vAlphaMap.z + vTextureColor.w <= 0)
//	{
//		vTextureColor.w = 1.0f;
//	}
//
//	float4 vDiffuse = Diffuse(input.n);
//	float4 vSpecular = Specular(input.n);
//	float4 vFinalColor = input.c * (vDiffuse + vSpecular) *vTextureColor;
//	return vFinalColor;
//}

float4 PS(VS_OUT input) : SV_TARGET
{
	// rgba에 알파값이 들어있는 텍스처
	float4 vAlphaMap = AlphaMap.Sample(sample0,input.t);
	// 나머지 텍스처들을 섞어준다.
	float4 vMultiTexture;
	vMultiTexture = AlphaTexture1.Sample(sample0, input.t * 20) * vAlphaMap.x;
	vMultiTexture += AlphaTexture2.Sample(sample0, input.t * 20) * vAlphaMap.y;
	vMultiTexture += AlphaTexture3.Sample(sample0, input.t * 20) * vAlphaMap.z;
	vMultiTexture += AlphaTexture4.Sample(sample0, input.t * 20) * vAlphaMap.w;
	
	float4 vDiffuse = Diffuse(input.n);
	float4 vSpecular = Specular(input.n);
	float4 vFinalColor = input.c * (vDiffuse + vSpecular) * vMultiTexture;
	
	return vFinalColor;
}

float4 AlphaMap_PS(VS_OUT input) : SV_TARGET
{
	float4 vAlpha = AlphaMap.Sample(sample0,input.t);
	vAlpha.w = 1.0f;
	
	return vAlpha;
}