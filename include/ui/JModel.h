#pragma once
#include "stdui.h"
#include "JDxHelper.h"
namespace UI
{
	class JModel
	{	
	public:		
		ID3D11Buffer*			 m_pVertexBuffer;
		ID3D11Buffer*			 m_pIndexBuffer;
		ID3D11Buffer*			 m_pConstantBuffer;
	protected:

		ID3D11VertexShader*		 m_pVS;
		ID3D11GeometryShader*    m_pGS;
		ID3D11InputLayout*		 m_pIL;
		ID3D11PixelShader*		 m_pPS;
		ID3DBlob*				 m_BlobPS;
		ID3DBlob*				 m_BlobVS;
		ID3DBlob*				 m_BlobGS;
		UINT					 m_iVertexNum;
		UINT					 m_iIndexNum;
		UINT					 m_iVertexSize;
		UINT					 m_iIndexSize;
	public:
		bool PreRender(ID3D11DeviceContext* pContext);
		bool Render(ID3D11DeviceContext* pContext);
		bool PostRender(ID3D11DeviceContext* pContext);
		bool Release();
	public:
		JModel();
		virtual ~JModel();
	};
}