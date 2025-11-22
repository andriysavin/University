//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <math.h>
//////////////////////////////////////////////////////////////////////////
struct MATRIX4
{
public:
	MATRIX4(const MATRIX4& refMatr)
	{
		memcpy(&m, &refMatr.m, sizeof(refMatr.m));
	}

	MATRIX4(void)
	{
	}

	MATRIX4& operator = (const MATRIX4& refMatr)
	{
		memcpy(&m, &refMatr.m, sizeof(refMatr.m));
		return *this;
	}

	MATRIX4& operator *= (const MATRIX4& refMatr);
	MATRIX4  operator *	(const MATRIX4& refMatr) const;

	void	Identity();
	void	Translation(float fDX, float fDY, float fDZ);
	void	Scale(float fSX, float fSY, float fSZ);
	void	RotationX(float fAngle);
	void	RotationY(float fAngle);
	void	RotationZ(float fAngle);
	void    Projection(float fFov, float fViewWidth, float fViewHeight);

	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
//////////////////////////////////////////////////////////////////////////
MATRIX4 MATRIX4::operator * (const MATRIX4& refMatr) const
{
	MATRIX4 mRes;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			mRes.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++)  mRes.m[i][j] += m[i][k] * refMatr.m[k][j];
		}

	return mRes;
}
//////////////////////////////////////////////////////////////////////////
MATRIX4& MATRIX4::operator *= (const MATRIX4& refMatr)
{
	MATRIX4 mRes;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			mRes.m[i][j] = 0.0f;
			for (int k = 0; k < 4; k++)  mRes.m[i][j] += m[i][k] * refMatr.m[k][j];
		}

	return (*this = mRes);
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::Identity()
{
	memset(&m, 0, sizeof(m));
	_11 = _22 = _33 = _44 = 1.0f;
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::Translation(float fDX, float fDY, float fDZ)
{
	Identity();
	_41 = fDX; _42 = fDY; _43 = fDZ;
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::Scale(float fSX, float fSY, float fSZ)
{
	memset(&m, 0, sizeof(m));
	_11 = fSX; _22 = fSY; _33 = fSZ; _44 = 1.0f;
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::RotationX(float fAngle)
{
	Identity();

	_22 = _33 = cosf(fAngle);
	_32 = -(_23 = sinf(fAngle));
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::RotationY(float fAngle)
{
	Identity();

	_11 = _33 = cosf(fAngle);
	_13 = -(_31 = sinf(fAngle));
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::RotationZ(float fAngle)
{
	Identity();

	_11 = _22 = cosf(fAngle);
	_21 = -(_12 = sinf(fAngle));
}
//////////////////////////////////////////////////////////////////////////
inline void MATRIX4::Projection(float fFov, float fViewWidth, float fViewHeight)
{
	Identity();

	_22 = -1;
	_33 = 0; _34 = tanf(fFov / 2) / min(fViewWidth, fViewHeight);
	_41 = fViewWidth / 2; _42 = fViewHeight / 2;
	_31 = _34 * _41; _32 = _34 * _42;
}
//////////////////////////////////////////////////////////////////////////
