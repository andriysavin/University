//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "Matrix4.h"
#include "Vector4.h"
//////////////////////////////////////////////////////////////////////////
class C3DEngine
{
public:

	enum PRIMITIVETYPE
	{
		PT_TRIANGLELIST,
		PT_TRIANGLESTRIP,
		PT_TRIANGLEFAN,
		PT_LINELIST,
		PT_LINESTRIP
	};

	enum TRANSFORMTYPE
	{
		TT_WORLD,
		TT_VIEW,
		TT_PROJECTION
	};

	typedef VECTOR4	VERTEX;
	typedef VECTOR4* PVERTEX;
	typedef DWORD	INDEX;
	typedef DWORD*	PINDEX;

	void		DrawIndexedPrimitive(PRIMITIVETYPE PrimType, DWORD dwStartIndex, DWORD dwPrimitiveCount);
	void		DrawPrimitive(PRIMITIVETYPE PrimType, DWORD dwStartVertex, DWORD dwPrimitiveCount);
	void		SetTransform(TRANSFORMTYPE TransformType, const MATRIX4* pMatrix);
	void		SetVertexBuffer(const PVERTEX pVertexBuffer);
	void		SetIndexBuffer(const PINDEX pIndexBuffer);
	void		SetDC(HDC hDC);
private:
	void		DrawTriangleList(DWORD dwStartVertex, DWORD dwPrimitiveCount);
	void		DrawTriangleStrip(DWORD dwStartVertex, DWORD dwPrimitiveCount);
	void		DrawTriangleFan(DWORD dwStartVertex, DWORD dwPrimitiveCount);
	void		DrawLineStrip(DWORD dwStartVertex, DWORD dwPrimitiveCount);
	void		DrawLineList(DWORD dwStartVertex, DWORD dwPrimitiveCount);

	void		DrawIndexedTriangleList(DWORD dwStartIndex, DWORD dwPrimitiveCount);
	void		DrawIndexedTriangleStrip(DWORD dwStartIndex, DWORD dwPrimitiveCount);
	void		DrawIndexedTriangleFan(DWORD dwStartIndex, DWORD dwPrimitiveCount);
	void		DrawIndexedLineStrip(DWORD dwStartIndex, DWORD dwPrimitiveCount);

	PVERTEX		m_pVertexBuffer;
	PINDEX      m_pIndexBuffer;

	MATRIX4		m_mWorldMatrix;
	MATRIX4		m_mViewMatrix;
	MATRIX4		m_mProjectionMatrix;
	MATRIX4  	m_mTransformMatrix;

	HDC			m_hDC;
};
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawIndexedPrimitive(PRIMITIVETYPE PrimType, DWORD dwStartIndex, DWORD dwPrimitiveCount)
{
	switch (PrimType)
	{
	case PT_TRIANGLELIST:
		DrawIndexedTriangleList(dwStartIndex, dwPrimitiveCount);
		break;
	case PT_TRIANGLESTRIP:
		DrawIndexedTriangleStrip(dwStartIndex, dwPrimitiveCount);
		break;
	case PT_TRIANGLEFAN:
		DrawIndexedTriangleFan(dwStartIndex, dwPrimitiveCount);
		break;
	case PT_LINESTRIP:
		DrawIndexedLineStrip(dwStartIndex, dwPrimitiveCount);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
inline void C3DEngine::DrawIndexedTriangleList(DWORD dwStartIndex, DWORD dwPrimitiveCount)
{
	VERTEX v1, v2, v3;

	for (DWORD i = dwStartIndex; i < dwPrimitiveCount * 3 + dwStartIndex; i += 3)
	{
		v1 = m_pVertexBuffer[m_pIndexBuffer[i + 0]] * m_mTransformMatrix; v1 /= v1.w;
		v2 = m_pVertexBuffer[m_pIndexBuffer[i + 1]] * m_mTransformMatrix; v2 /= v2.w;
		v3 = m_pVertexBuffer[m_pIndexBuffer[i + 2]] * m_mTransformMatrix; v3 /= v3.w;

		MoveToEx(m_hDC, (int)v1.x, (int)v1.y, NULL);
		LineTo(m_hDC, (int)v2.x, (int)v2.y);
		LineTo(m_hDC, (int)v3.x, (int)v3.y);
		LineTo(m_hDC, (int)v1.x, (int)v1.y);
	}
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawIndexedTriangleStrip(DWORD dwStartIndex, DWORD dwPrimitiveCount)
{
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawIndexedTriangleFan(DWORD dwStartIndex, DWORD dwPrimitiveCount)
{
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawIndexedLineStrip(DWORD dwStartIndex, DWORD dwPrimitiveCount)
{
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawPrimitive(PRIMITIVETYPE PrimType, DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
	switch (PrimType)
	{
	case PT_TRIANGLELIST:
		DrawTriangleList(dwStartVertex, dwPrimitiveCount);
		break;
	case PT_TRIANGLESTRIP:
		DrawTriangleStrip(dwStartVertex, dwPrimitiveCount);
		break;
	case PT_TRIANGLEFAN:
		DrawTriangleFan(dwStartVertex, dwPrimitiveCount);
		break;
	case PT_LINELIST:
		DrawLineList(dwStartVertex, dwPrimitiveCount);
		break;
	case PT_LINESTRIP:
		DrawLineStrip(dwStartVertex, dwPrimitiveCount);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawTriangleList(DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawLineList(DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
	VERTEX v1, v2;

	for (DWORD i = dwStartVertex; i < 2 * dwPrimitiveCount + dwStartVertex; i += 2)
	{
		v1 = m_pVertexBuffer[i + 0] * m_mTransformMatrix; v1 /= v1.w;
		v2 = m_pVertexBuffer[i + 1] * m_mTransformMatrix; v2 /= v2.w;
		MoveToEx(m_hDC, (int)v1.x, (int)v1.y, NULL);
		LineTo(m_hDC, (int)v2.x, (int)v2.y);
	}
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawTriangleStrip(DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
	VERTEX v1, v2, v3;

	v1 = m_pVertexBuffer[dwStartVertex + 0] * m_mTransformMatrix; v1 /= v1.w;
	v2 = m_pVertexBuffer[dwStartVertex + 1] * m_mTransformMatrix; v2 /= v2.w;
	v3 = m_pVertexBuffer[dwStartVertex + 2] * m_mTransformMatrix; v3 /= v3.w;

	MoveToEx(m_hDC, (int)v1.x, (int)v1.y, NULL);
	LineTo(m_hDC, (int)v2.x, (int)v2.y);
	LineTo(m_hDC, (int)v3.x, (int)v3.y);
	LineTo(m_hDC, (int)v1.x, (int)v1.y);

	for (DWORD i = dwStartVertex + 1; i < dwPrimitiveCount + dwStartVertex; i++)
	{
		v1 = m_pVertexBuffer[i + 0] * m_mTransformMatrix; v1 /= v1.w;
		v2 = m_pVertexBuffer[i + 1] * m_mTransformMatrix; v2 /= v2.w;
		v3 = m_pVertexBuffer[i + 2] * m_mTransformMatrix; v3 /= v3.w;

		MoveToEx(m_hDC, (int)v2.x, (int)v2.y, NULL);
		LineTo(m_hDC, (int)v3.x, (int)v3.y);
		LineTo(m_hDC, (int)v1.x, (int)v1.y);
	}
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawTriangleFan(DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
	VERTEX v1, v2, v3;

	v1 = m_pVertexBuffer[dwStartVertex + 0] * m_mTransformMatrix; v1 /= v1.w;

	for (DWORD i = dwStartVertex + 1; i < dwPrimitiveCount + dwStartVertex + 1; i += 1)
	{
		v2 = m_pVertexBuffer[i + 0] * m_mTransformMatrix; v2 /= v2.w;
		v3 = m_pVertexBuffer[i + 1] * m_mTransformMatrix; v3 /= v3.w;

		MoveToEx(m_hDC, (int)v2.x, (int)v2.y, NULL);
		LineTo(m_hDC, (int)v3.x, (int)v3.y);
		LineTo(m_hDC, (int)v1.x, (int)v1.y);
	}
}
//////////////////////////////////////////////////////////////////////////
void C3DEngine::DrawLineStrip(DWORD dwStartVertex, DWORD dwPrimitiveCount)
{
}
//////////////////////////////////////////////////////////////////////////
inline void C3DEngine::SetVertexBuffer(const PVERTEX pVertexBuffer)
{
	m_pVertexBuffer = pVertexBuffer;
}
//////////////////////////////////////////////////////////////////////////
inline void C3DEngine::SetIndexBuffer(const PINDEX pIndexBuffer)
{
	m_pIndexBuffer = pIndexBuffer;
}
//////////////////////////////////////////////////////////////////////////
inline void C3DEngine::SetTransform(TRANSFORMTYPE TransformType, const MATRIX4* pMatrix)
{
	switch (TransformType)
	{
	case TT_WORLD:
		m_mWorldMatrix = *pMatrix;
		break;
	case TT_VIEW:
		m_mViewMatrix = *pMatrix;
		break;
	case TT_PROJECTION:
		m_mProjectionMatrix = *pMatrix;
		break;
	}
	m_mTransformMatrix = (m_mWorldMatrix * m_mViewMatrix) * m_mProjectionMatrix;
}
//////////////////////////////////////////////////////////////////////////
inline void C3DEngine::SetDC(HDC hDC)
{
	m_hDC = hDC;
}
//////////////////////////////////////////////////////////////////////////