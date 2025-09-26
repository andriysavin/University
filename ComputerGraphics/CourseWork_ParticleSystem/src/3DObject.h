//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
//////////////////////////////////////////////////////////////////////////
class C3DObject
{
public:
					 C3DObject(LPDIRECT3DDEVICE9 const pDevice);
	virtual			~C3DObject(void);

	virtual	void	Update() = 0;
	virtual	void	Render() = 0;

	void		SetPos   (const D3DXVECTOR3& vPos     ) {m_vPos = vPos;		  UpdateTransformMatrix();}
	void		ChangePos(const D3DXVECTOR3& vDeltaPos) {m_vPos += vDeltaPos; UpdateTransformMatrix();}

	void		SetScale(const D3DXVECTOR3& vScale) {m_vScale = vScale; UpdateTransformMatrix();}
	

	void		SetRotX(float fAng) {m_vRot.x = fAng; UpdateTransformMatrix();}
	void		SetRotY(float fAng) {m_vRot.y = fAng; UpdateTransformMatrix();}
	void		SetRotZ(float fAng) {m_vRot.z = fAng; UpdateTransformMatrix();}

	void		ChangeRot(const D3DXVECTOR3& vDeltaAngles)
				{ m_vRot += vDeltaAngles; UpdateTransformMatrix();}
	void		SetIsVisible(bool blIsVisible) { m_blIsVisible = blIsVisible; }
	bool		IsVisible() const { return m_blIsVisible; }
protected:
	D3DXMATRIX					m_mWorld;
	LPDIRECT3DDEVICE9 const		m_pDevice;
private:
	void		UpdateTransformMatrix();

	D3DXVECTOR3					m_vPos;
	D3DXVECTOR3					m_vRot;
	D3DXVECTOR3					m_vScale;
	bool						m_blIsVisible;
};
//////////////////////////////////////////////////////////////////////////
