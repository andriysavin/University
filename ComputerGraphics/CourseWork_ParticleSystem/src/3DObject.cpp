//////////////////////////////////////////////////////////////////////////
#include "3dobject.h"
//////////////////////////////////////////////////////////////////////////
C3DObject::C3DObject(LPDIRECT3DDEVICE9 const pDevice):
	m_pDevice(pDevice),
	m_vPos	 (0.0f, 0.0f, 0.0f),
	m_vRot	 (0.0f, 0.0f, 0.0f),
	m_vScale (1.0f, 1.0f, 1.0f),
	m_blIsVisible(true)
{
	D3DXMatrixIdentity(&m_mWorld);
}
//////////////////////////////////////////////////////////////////////////
C3DObject::~C3DObject(void)
{
}
//////////////////////////////////////////////////////////////////////////
void C3DObject::UpdateTransformMatrix()
{
	D3DXMATRIX mTrans, mRot, mScale;
	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationYawPitchRoll(&mRot, m_vRot.y, m_vRot.x, m_vRot.z);
	D3DXMatrixScaling(&mScale, m_vScale.y, m_vScale.x, m_vScale.z);
	m_mWorld = mScale * mRot * mTrans;
}
//////////////////////////////////////////////////////////////////////////
