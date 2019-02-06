#define MAX_BONE_MATRICES 255 
//#define FT_CONSTANTBUFFER 0
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D g_txDiffuse: register (t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
Texture2D	g_txDepthMap : register(t4);

SamplerState samLinear: register (s0);
SamplerState samShadowMap : register(s1);
SamplerComparisonState samComShadowMap : register (s2);

static const int SMapSize = 1024;
static const float EPSILON = 0.005f;
static const float refAtNormal_Incidence = 1.33f;

cbuffer cb0: register (b0)
{
	float4x4	g_matWorld	: packoffset(c0);
	float4x4	g_matView	: packoffset(c4);
	float4x4	g_matProj	: packoffset(c8);
	float4		g_MeshColor : packoffset(c12);
};

// ȯ�� ������
cbuffer cbObjectData : register(b1)
{
	//matrix g_matNormal;
	float4 cb_LightVector;
	float4 cb_EyePos;
	float4 cb_EyeDir;
}
// ����? ������
cbuffer cbLightMaterial : register(b2)
{
	float4 cb_AmbientLightColor : packoffset(c0);
	float4 cb_DiffuseLightColor : packoffset(c1);
	float3 cb_SpecularLightColor: packoffset(c2);
	float  cb_SpecularPower : packoffset(c2.w);
};
// ���׸���, ������ ������
cbuffer cbMaterial : register (b3)
{
	float			cb_useLight : packoffset(c0.x);
	float			cb_useNormalMap : packoffset(c0.y);
	float			cb_useEnviMap : packoffset(c0.z);
	float			cb_useShadow : packoffset(c0.w);
	float4x4		g_matShadow  : packoffset(c1);
	float			g_iObjectID : packoffset(c5.x);
	float			g_iNumKernel : packoffset(c5.y);
	//float			g_iDummy	 : packoffset(c4.zw);
}
// ť��� ���
cbuffer cbCubeViewMatrix : register(b4)
{
	matrix g_matCubeView[6] : packoffset(c0);
}

static const float NEAR = 0.1f;
static const float FAR = 2000.0f;
// ���� Ÿ��
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

struct VS_OUTPUT_Mix
{
	float4 pos		 : SV_POSITION;
	float4 nor		 : NORMAL;
	float4 col		 : COLOR0;
	float2 tex		 : TEXCOORD0;
	// �븻
	float3 vEye		 : TEXCOORD1;
	float3 vHalf	 : TEXCOORD2;
	float3 vLightDir : TEXCOORD3;
	// ȯ��
	float3 ref		 : TEXCOORD4;
	float3 fre		 : TEXCOORD5;
	float3 eye		 : TEXCOORD6;
	// ������
	float4 TexShadow : TEXCOORD7;
};

// ������ ���
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
VS_OUTPUT_Mix VS(PNCT5_VS_INPUT input)//,uniform bool bHalfVector )
{
	VS_OUTPUT_Mix output = (VS_OUTPUT_Mix)0;

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

	float4 WorldPos = mul(float4(output.pos.xyz, 1.0f), g_matWorld);
	output.pos = mul(WorldPos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	float3 vNormal = normalize(mul(output.nor.xyz, (float3x3)g_matWorld));
	output.nor = float4(vNormal, (output.pos.w - NEAR) / (FAR - NEAR));// g_matWorldInvTrans));
	output.tex = input.tex;

	///
	float3 vLightDir = normalize(cb_LightVector.xyz - WorldPos.xyz);

	//// �븻
	//if (cb_useNormalMap)
	//{
	//	//float3 vNormal = input.nor;
	//	output.vEye = normalize(cb_EyePos.xyz - WorldPos.xyz);
	//
	//	float3 tangent = normalize(mul(input.tan, (float3x3)g_matNormal));
	//	float3 biNormal = normalize(cross(vNormal, tangent));
	//
	//	float3x3 matTangent = { tangent.x, biNormal.x, vNormal.x,
	//		tangent.y, biNormal.y, vNormal.y,
	//		tangent.z, biNormal.z, vNormal.z };
	//
	//	output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), matTangent));
	//	output.vLightDir = normalize(mul(vLightDir, matTangent));
	//	output.vEye = normalize(mul(output.vEye, matTangent));
	//	output.col = input.col;
	//}
	//else
	{
		float fDot = lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.2f) + 0.3f;
		output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;
	}

	//// ȯ��
	//if (cb_useEnviMap)
	//{
	//	// camera/eye -> V?
	//	float3 incident = output.eye = normalize(WorldPos.xyz - cb_EyePos.xyz);
	//	// R = I - 2 * N * (I.N)	?
	//	//output.ref = normalize(incident - 2.0f * output.nor * dot(incident, output.nor));
	//	output.ref = normalize(reflect(incident, output.nor.xyz));
	//	output.fre = normalize(refract(incident, output.nor.xyz, 1.0f / refAtNormal_Incidence));
	//}

	// ������ �ؽ�ó ��ǥ
	if (cb_useShadow)
	{
		output.TexShadow = mul(float4(input.pos, 1.0f), g_matShadow);
	}
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PBUFFER_OUTPUT PS(VS_OUTPUT_Mix input) : SV_Target
{
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color1	= input.nor;
	output.color0	= g_txDiffuse.Sample(samLinear, input.tex) * input.col;
	//output.color1.w		= vIn.col.w;
	//output.color0.w		= 1.0f;


	//// ȯ��
	//if (cb_useEnviMap)
	//{
	//	uint type = cb_useEnviMap;
	//	switch (type)
	//	{
	//	case 1:
	//	{
	//		float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	//		float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);
	//
	//		output.color0 = lerp(output.color0, reflectColor, 0.5f);
	//	} break;
	//	case 2:
	//	{
	//		float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	//		float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// �ݻ� �÷�
	//
	//		float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
	//		float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);
	//
	//		// ��ǻ��ʰ� �ݻ�� ����
	//		output.color0 = lerp(output.color0, reflectColor, fresnel + 0.3f);
	//	} break;
	//	case 3:
	//	{
	//		float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// �ݻ� �÷�
	//		float4 refractColor = g_txEnviMap.Sample(samLinear, input.fre);		// ���� �÷�
	//
	//		float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
	//		float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);
	//
	//		// ����, �ݻ� ����
	//		output.color0 *= lerp(refractColor, reflectColor, fresnel * 7.0f)/* * input.col*/ * 1.15f;
	//		//color = lerp(refractColor, color, fresnel * 0.1f);
	//		//color.a = 1.0f;
	//	} break;
	//	}
	//}
	//// �븻
	//if (cb_useNormalMap)
	//{
	//	// ��ǻ�� ����(�븻��)
	//	float4 normal = g_txNormalMap.Sample(samLinear, input.tex);
	//	normal = normalize((normal - 0.5f) * 2.0f);
	//
	//	// saturate == clamp
	//	float fDot = saturate(dot(normal.xyz, input.vLightDir));// +0.15f;
	//	float3 LightColor = cb_DiffuseLightColor.rgb * fDot;
	//
	//	// ����ŧ�� ����
	//	float3 R = reflect(-input.vLightDir, normal.xyz);
	//	float3 SpecularColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);
	//	//float3 SpecularColor	= cb_SpecularLightColor.rgb * pow(saturate(dot(input.vHalf, normal.xyz )), cb_SpecularPower);
	//
	//	// ���� �÷� ����
	//	//return vDiffuseColor * float4(LightColor + SpecularColor, 1.0f) * input.col;
	//	output.color0 *= float4(LightColor + SpecularColor + 0.2f, 1.0f)/* * input.col*/;
	//}
	// ����
	//if (cb_useLight)
	//{
	//	output.color0 *= input.col * cb_useLight;
	//}
	// ������
	if (cb_useShadow)
	{
		static const float	iNumKernel = 3;
		float fLightAmount = 0.0f;
		float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;

		const float fdelta = 1.0f / SMapSize;
		int iHalf = (iNumKernel - 1) / 2;
		for (int v = -iHalf; v <= iHalf; v++)
		{
			for (int u = -iHalf; u <= iHalf; u++)
			{
				float2 vOffset = float2(u * fdelta, v * fdelta);
				fLightAmount += g_txDepthMap.SampleCmpLevelZero(samComShadowMap,
					ShadowTexColor.xy + vOffset, ShadowTexColor.z);
			}
		}
		fLightAmount /= iNumKernel * iNumKernel;
		///float fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
		//output.color0 *= max(0.5f, fLightAmount);

		output.color0 *= max(cb_useShadow, fLightAmount);

		//output.color0 = cb_useShadow > fLightAmount ? output.color0 * cb_useShadow : output.color0;
	}

	output.color0.w = 1.0f;
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