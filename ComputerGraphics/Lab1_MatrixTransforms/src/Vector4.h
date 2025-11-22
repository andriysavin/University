//////////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <math.h>
#include "Matrix4.h"

struct VECTOR4
{
	VECTOR4(void)
	{
	}

	VECTOR4(float fX, float fY, float fZ, float fW = 1)
	{
		x = fX; y = fY; z = fZ; w = fW;
	}

	VECTOR4 operator * (MATRIX4& refMatr)
	{
		VECTOR4 vRes;
		for (int j = 0; j < 4; j++)
		{
			vRes.v[j] = 0.0f;
			for (int k = 0; k < 4; k++)  vRes.v[j] += v[k] * refMatr.m[k][j];
		}
		return vRes;
	}

	VECTOR4 operator - ()
	{
		return VECTOR4(-x, -y, -z, -w);
	}

	VECTOR4& operator -= (VECTOR4& refVect)
	{
		x -= refVect.x;
		y -= refVect.y;
		z -= refVect.z;
		w -= refVect.w;
		return *this;
	}

	VECTOR4& operator += (VECTOR4& refVect)
	{
		x += refVect.x;
		y += refVect.y;
		z += refVect.z;
		w += refVect.w;
		return *this;
	}

	VECTOR4& operator *= (MATRIX4& refMatr)
	{
		VECTOR4 vRes;
		
		for (int j = 0; j < 4; j++)
		{
			vRes.v[j] = 0.0f;
			for (int k = 0; k < 4; k++)  vRes.v[j] += v[k] * refMatr.m[k][j];
		}

		return (*this = vRes);
	}

	VECTOR4& operator /= (float fValue)
	{
		x /= fValue, y /= fValue, z /= fValue, w /= fValue;
		return *this;
	}

	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};
};
//////////////////////////////////////////////////////////////////////////