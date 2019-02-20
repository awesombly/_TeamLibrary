#define DirectLight
#define MAX_BONE_MATRICES 255 
#include "../../data/shader/ShaderData.hlsl"
//#define FT_CONSTANTBUFFER 0
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D g_txDiffuse: register (t0);
//Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
Texture2D	g_txDepthMap : register(t4);

SamplerState samLinear: register (s0);
SamplerState samShadowMap : register(s1);
SamplerComparisonState samComShadowMap : register (s2);


cbuffer cb0: register (b0)
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4		g_MeshColor : packoffset(c12);
};

// 환경 데이터
cbuffer cbObjectData : register(b5)
{
	float4 cb_LightVector;
	float4 cb_EyePos;
	float4 cb_EyeDir;
}

// 메테리얼, 쉐도우 데이터
cbuffer cbMaterial : register (b3)
{
	float			cb_useLight : packoffset(c0.x);
	float			cb_useNormalMap : packoffset(c0.y);
	float			cb_useEnviMap : packoffset(c0.z);
	float			cb_useShadow : packoffset(c0.w);
	float4x4		g_matShadow  : packoffset(c1);
	float			g_iObjectID : packoffset(c5.x);
	float			g_iNumKernel : packoffset(c5.y);
}

// 렌더 타겟
struct PBUFFER_OUTPUT
{
	float4 color0 : SV_TARGET0;
	float4 color1 : SV_TARGET1;
};


#ifdef FT_CONSTANTBUFFER
	cbuffer cbAnimMatrices : register (b1)
	{
		matrix m_matConstBoneWorld[MAX_BONE_MATRICES];
	};
	matrix FetchBoneTransform(uint iBone)							////////////////////
	{
		matrix mret;
		mret = m_matConstBoneWorld[iBone];
		return mret;
	}
#else
	Buffer<float4>     g_BufferBoneWorld : register(t1);
	float4x4 FetchBoneTransform(uint iBone)							////////////////////
	{
		float4x4 mret;
		iBone *= 4;
		float4 row1 = g_BufferBoneWorld.Load(iBone + 0);
		float4 row2 = g_BufferBoneWorld.Load(iBone + 1);
		float4 row3 = g_BufferBoneWorld.Load(iBone + 2);
		float4 row4 = g_BufferBoneWorld.Load(iBone + 3);
		mret = float4x4(row1, row2, row3, row4);
		return mret;
	}
#endif

struct PNCT5_VS_INPUT
{
	float3 pos		: POSITION;
	float3 nor		: NORMAL;
	float4 col		: COLOR0;
	float2 tex		: TEXCOORD0;
	float4 w0		: TEXCOORD1;
	float4 i0		: TEXCOORD2;
	float4 w1		: TEXCOORD3;
	float4 i1		: TEXCOORD4;
};
//struct VS_OUTPUT
//{
//	float4 pos : SV_POSITION;
//	float3 nor : NORMAL;
//	float4 col : COLOR0;
//	float2 tex : TEXCOORD0;
//};

struct VS_OUTPUT
{
	float4 pos		 : SV_POSITION;
	float4 nor		 : NORMAL;
	float4 col		 : COLOR0;
	float2 tex		 : TEXCOORD0;
	//// 노말
	//float3 vEye		 : TEXCOORD1;
	//float3 vHalf	 : TEXCOORD2;
	//float3 vLightDir : TEXCOORD3;
	// 환경
	float3 ref		 : TEXCOORD1;
	float3 fre		 : TEXCOORD2;
	//float3 eye		 : TEXCOORD3;
	// 쉐도우
	float4 TexShadow : TEXCOORD3;
};

// 프레넬 계산
float ComputeFresnel(float3 reflect, float3 normal, float f0)
{
	float cosAngle = 1.0f - saturate(dot(reflect, normal));
	float result = pow(cosAngle, 5.0f);
	result = saturate(mad(result, 1.0f - saturate(f0), f0));
	return result;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(PNCT5_VS_INPUT input)//,uniform bool bHalfVector )
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 Pos = float4(input.pos, 1.0f);
	float3 Norm = input.nor;

	float4x4 matMatrix;
	float4 vLocal;
	for (int iBiped = 0; iBiped < input.w1.w; iBiped++)
	{
		uint iBoneIndex = (uint)input.i0[iBiped];
		float fWeight = input.w0[iBiped];

		vLocal = Pos;
		if (iBiped < 4)
		{
			matMatrix = FetchBoneTransform(iBoneIndex);
			output.pos += fWeight * mul(vLocal, matMatrix);
			output.nor.xyz += fWeight * mul(Norm, (float3x3)matMatrix);
		}
		else
		{
			iBoneIndex = (uint)input.i1[iBiped];
			fWeight = input.w1[iBiped];
			matMatrix = FetchBoneTransform(iBoneIndex);
			output.pos += fWeight * mul(vLocal, matMatrix);
			output.nor.xyz += fWeight * mul(Norm, (float3x3)matMatrix);
		}
	}

	float4 WorldPos = mul(output.pos, g_matWorld);
	output.pos = mul(WorldPos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	//Norm = output.nor.xyz;
	//output.nor.z = -Norm.x;
	//output.nor.x = -Norm.y;
	//output.nor.y = Norm.z;

	
	output.nor.x = Norm.z;	//r
	output.nor.y = Norm.y; //g
	output.nor.z = -Norm.x;	//b

	//output.nor = float4(normalize(mul(output.nor.xyz, (float3x3)g_matWorld)), (output.pos.w - fNEAR) / (fFAR - fNEAR));// g_matWorldInvTrans));
	output.nor = float4(normalize(mul(output.nor.xyz, (float3x3)g_matWorld)), output.pos.w / fFAR);
	//float3 vNormal = output.nor.xyz;
	output.tex = input.tex;

	//if (cb_useLight)
	//{
	#ifdef DirectLight
		float3 vLightDir = -cb_LightVector.xyz;
	#else
		float3 vLightDir = normalize(-cb_LightVector.xyz - WorldPos.xyz);
	#endif
		//float fDot = max(0.2f, lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.2f) + 0.2f);
		//output.col = /*float4(fDot, fDot, fDot, 1.0f) **/ /*input.col **/ fDot;
		output.col.xyz = input.col.xyz * max(cb_useLight, dot(vLightDir, output.nor.xyz) + 0.3f);
		//output.col.xy *= 0.85f;
		output.col.w = input.col.w;

	// 환경
	if (cb_useEnviMap)
	{
		// camera/eye -> V?
		//float3 incident = normalize(cb_EyePos.xyz - WorldPos.xyz);
		float3 incident = normalize(WorldPos.xyz - cb_EyePos.xyz);
		// R = I - 2 * N * (I.N)	?
		//output.ref = normalize(incident - 2.0f * output.nor * dot(incident, output.nor));
		output.ref = normalize(reflect(incident, output.nor.xyz));
		//output.fre = normalize(refract(incident, vNormal, 1.0f / refAtNormal_Incidence));
	}

	// 쉐도우 텍스처 좌표
	//if (cb_useShadow)
	//{
		output.TexShadow = mul(Pos, g_matShadow);
	//}
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PBUFFER_OUTPUT PS(VS_OUTPUT input) : SV_Target
{
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color0	= g_txDiffuse.Sample(samLinear, input.tex);
	if (output.color0.w < 0.1f)
		discard;
	output.color1	= input.nor;

	// 환경
	if (cb_useEnviMap)
	{
		//case 2:
		//{
			//float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// 반사 컬러
			//
			//float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
			//float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);
			//
			//// 디퓨즈맵과 반사맵 보간
			//output.color0.xyz = lerp(output.color0.xyz, reflectColor.xyz, fresnel * 0.2f/* + 0.3f*/);
		//} break;
		//case 3:
		//{
		float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// 반사 컬러
		float4 refractColor = g_txEnviMap.Sample(samLinear, input.fre);		// 굴절 컬러

		float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
		float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);

		// 굴절, 반사 보간
		output.color0.xyz = lerp(output.color0.xyz, lerp(refractColor.xyz, reflectColor.xyz, fresnel * 7.0f), 0.15f)/* * input.col*/;
		//color = lerp(refractColor, color, fresnel * 0.1f);
		//color.a = 1.0f;
	//} break;
	//}
	}

	// 쉐도우
	//if (cb_useShadow)
	//{
	//	static const float iNumKernel = 3;
	//	float fLightAmount = 0.0f;
	//	float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;

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

	//	output.color0.xyz *= max(cb_useShadow, fLightAmount);
	//}

	output.color0 *= input.col;
	return output;
}

//Matrix m =
//FetchBoneTransform(Input.Bones.x) * Input.Weights.x
//+ FetchBoneTransform(Input.Bones.y) * Input.Weights.y
//+ FetchBoneTransform(Input.Bones.z) * Input.Weights.z
//+ FetchBoneTransform(Input.Bones.w) * Input.Weights.w;
//Output.Pos = mul(Pos, m);
//Output.Norm = mul(Norm, (float3x3)m);
//Output.Tan = mul(Tan, (float3x3)m);

/*uint iBone = Input.Bones.x;
float fWeight = Input.Weights.x;
matrix m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.y;
fWeight = Input.Weights.y;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.z;
fWeight = Input.Weights.z;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );

iBone = Input.Bones.w;
fWeight = Input.Weights.w;
m = g_mBoneWorld[ iTimeShift*MAX_BONE_MATRICES + iBone ];
Output.Pos += fWeight * mul( pos, m );
Output.Norm += fWeight * mul( norm, m );
*/