#include "../../shader/Basis.hlsl"
Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
Texture2D	g_txDepthMap : register(t4);

SamplerState samLinear : register (s0);
SamplerState samShadowMap : register(s1);
SamplerComparisonState samComShadowMap : register (s2);


//cbuffer cb0
//{
//	float4x4 g_matWorld		: packoffset(c0);
//	float4x4 g_matView		: packoffset(c4);
//	float4x4 g_matProj		: packoffset(c8);
//};

struct VS_IN
{
	float3 pos		: POSITION;
	float3 nor		: NORMAL;
	float4 col		: COLOR;
	float2 tex	    : TEXCOORD;
};

struct VS_OUTPUT_Mix
{
	float4 pos		 : SV_POSITION;
	float4 nor		 : NORMAL;
	float4 col		 : COLOR0;
	float2 tex		 : TEXCOORD0;
	// ³ë¸»
	float3 vEye		 : TEXCOORD1;
	float3 vHalf	 : TEXCOORD2;
	float3 vLightDir : TEXCOORD3;
	// È¯°æ
	float3 ref		 : TEXCOORD4;
	float3 fre		 : TEXCOORD5;
	float3 eye		 : TEXCOORD6;
	// ½¦µµ¿ì
	float4 TexShadow : TEXCOORD7;
};

// ÇÁ·¹³Ú °è»ê
float ComputeFresnel(float3 reflect, float3 normal, float f0)
{
	float cosAngle = 1.0f - saturate(dot(reflect, normal));
	float result = pow(cosAngle, 5.0f);
	result = saturate(mad(result, 1.0f - saturate(f0), f0));
	return result;
}


VS_OUTPUT_Mix VS(VS_IN input)
{
	VS_OUTPUT_Mix output = (VS_OUTPUT_Mix)0;
	float4 WorldPos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(WorldPos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	float3 vNormal = normalize(mul(input.nor, (float3x3)g_matWorld)); // normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(vNormal, (output.pos.w - NEAR) / (FAR - NEAR));
	output.tex = input.tex;

	//float3 vLightDir = -cb_LightVector;// normalize(cb_LightVector.xyz - WorldPos.xyz);
	float3 vLightDir = normalize(cb_LightVector.xyz - WorldPos.xyz);

	float fDot = max(0.3f, lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.2f) + 0.2f);
	output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;
	//float fDot = lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.15f) + 0.2f;
	//output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;

	// ½¦µµ¿ì ÅØ½ºÃ³ ÁÂÇ¥
	if (cb_useShadow)
	{
		output.TexShadow = mul(float4(input.pos, 1.0f), g_matShadow);
	}
	return output;
}



PBUFFER_OUTPUT PS(VS_OUTPUT_Mix input) : SV_Target
{
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color1 = input.nor;
	output.color0 = g_txDiffuse.Sample(samLinear, input.tex) * input.col;
	
	//// ½¦µµ¿ì
	//if (cb_useShadow)
	//{
	//	static const float	iNumKernel = 3;
	//	float fLightAmount = 0.0f;
	//	float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;
	//
	//	const float fdelta = 1.0f / SMapSize;
	//	int iHalf = (iNumKernel - 1) / 2;
	//	for (int v = -iHalf; v <= iHalf; v++)
	//	{
	//		for (int u = -iHalf; u <= iHalf; u++)
	//		{
	//			float2 vOffset = float2(u * fdelta, v * fdelta);
	//			fLightAmount += g_txDepthMap.SampleCmpLevelZero(samComShadowMap,
	//				ShadowTexColor.xy + vOffset, ShadowTexColor.z);
	//		}
	//	}
	//	fLightAmount /= iNumKernel * iNumKernel;
	//	///float fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
	//	//output.color0 *= max(0.5f, fLightAmount);
	//
	//	output.color0 *= max(cb_useShadow, fLightAmount);
	//
	//	//output.color0 = cb_useShadow > fLightAmount ? output.color0 * cb_useShadow : output.color0;
	//}

	output.color0.w = 1.0f;
	return  output;
}