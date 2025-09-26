//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
//////////////////////////////////////////////////////////////////////////
class Camera
{
public:
	Camera(LPDIRECT3DDEVICE9 pDevice);
	~Camera(void);

	void		SetViewProjMatrices(bool blForse = false);
	void		SetPos   (const D3DXVECTOR3& vPos     ) {m_vPos = vPos;		  UpdateViewMatrix();}
	void		ChangePos(const D3DXVECTOR3& vDeltaPos) {m_vPos += vDeltaPos; UpdateViewMatrix();}
	void		ChangePosCamSpace(const D3DXVECTOR3& vDeltaPos) ;

	void		SetRot(const D3DXVECTOR3& vAngles) { m_vCamRot = vAngles; UpdateViewMatrix(); }
	void		SetRotX(float fAng) {m_vCamRot.x = fAng; UpdateViewMatrix();}
	void		SetRotY(float fAng) {m_vCamRot.y = fAng; UpdateViewMatrix();}
	void		SetRotZ(float fAng) {m_vCamRot.z = fAng; UpdateViewMatrix();}

	void		ChangeRot(const D3DXVECTOR3& vDeltaAngles)
				{ m_vCamRot += vDeltaAngles; UpdateViewMatrix();}
private:
	void		UpdateViewMatrix();
	void		UpdateProjMatrix();

	LPDIRECT3DDEVICE9  const m_pDevice;

	float	m_fFOV;
	float	m_fAspect;
	float	m_fZNear;
	float	m_fZFar ;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vLookAt;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vCamRot;

	D3DXMATRIX  m_mView;
	D3DXMATRIX  m_mProj;
	bool		m_blViewCh;
	bool		m_blProjCh;
};
//////////////////////////////////////////////////////////////////////////
