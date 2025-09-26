//////////////////////////////////////////////////////////////////////////
#include "Camera.h"
//////////////////////////////////////////////////////////////////////////
Camera::Camera(LPDIRECT3DDEVICE9 pDevice) : 
	m_pDevice (	 pDevice),
	m_fFOV    (D3DX_PI/4),
	m_fAspect (4.0f/3.0f),
	m_fZNear  (     1.0f),
	m_fZFar   ( 10000.0f),
	m_vPos	  (0.0f, 0.0f, 0.0f),
	m_vLookAt (0.0f, 0.0f, 1.0f),
	m_vUp     (0.0f, 1.0f, 0.0f),
	m_vCamRot (0.0f, 0.0f, 0.0f),
	m_blViewCh(true),
	m_blProjCh(true)

{
	UpdateProjMatrix();
	UpdateViewMatrix();
}
//////////////////////////////////////////////////////////////////////////
Camera::~Camera(void)
{
}
//////////////////////////////////////////////////////////////////////////
void Camera::UpdateProjMatrix()
{
	D3DXMatrixPerspectiveFovLH  ( &m_mProj, m_fFOV, m_fAspect, m_fZNear, m_fZFar );
	m_blProjCh = true;
}
//////////////////////////////////////////////////////////////////////////
void Camera::UpdateViewMatrix()
{
	D3DXMATRIX mTrans,mRotX,mRotY,mRotZ;
	D3DXMatrixTranslation(&mTrans,-m_vPos.x,-m_vPos.y,-m_vPos.z);
	D3DXMatrixRotationX(&mRotX,-m_vCamRot.x);
	D3DXMatrixRotationY(&mRotY,-m_vCamRot.y);
	D3DXMatrixRotationZ(&mRotZ,-m_vCamRot.z);

	m_mView = mTrans * mRotY * mRotX * mRotZ;
	m_blViewCh = true ;
}
//////////////////////////////////////////////////////////////////////////
void	Camera::ChangePosCamSpace(const D3DXVECTOR3& vDeltaPos) 
{
	D3DXVECTOR3 vTransDeltaPos;
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll(&mRot,m_vCamRot.y,m_vCamRot.x,m_vCamRot.z);
		
	D3DXVec3TransformCoord(&vTransDeltaPos,&vDeltaPos,&mRot );

	m_vPos += vTransDeltaPos; 

	UpdateViewMatrix();
}
//////////////////////////////////////////////////////////////////////////
void Camera::SetViewProjMatrices(bool blForce /*= false*/)
{
	if(m_blViewCh || blForce )	
		m_pDevice->SetTransform( D3DTS_VIEW,	   &m_mView ),m_blViewCh = false;
	if(m_blProjCh || blForce )	
		m_pDevice->SetTransform( D3DTS_PROJECTION, &m_mProj ),m_blProjCh = false;
}
//////////////////////////////////////////////////////////////////////////
