//#pragma once
//#include "header.h"
//
//struct q3Mat3
//{
//	D3DXVECTOR3 ex;
//	D3DXVECTOR3 ey;
//	D3DXVECTOR3 ez;
//
//	q3Mat3();
//	q3Mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i);
//	q3Mat3(const D3DXVECTOR3& _x, const D3DXVECTOR3& _y, const D3DXVECTOR3& _z);
//
//	void Set(float a, float b, float c, float d, float e, float f, float g, float h, float i);
//	void Set(const D3DXVECTOR3& axis, float angle);
//	void SetRows(const D3DXVECTOR3& x, const D3DXVECTOR3& y, const D3DXVECTOR3& z);
//
//	q3Mat3& operator=(const q3Mat3& rhs);
//	q3Mat3& operator*=(const q3Mat3& rhs);
//	q3Mat3& operator*=(float f);
//	q3Mat3& operator+=(const q3Mat3& rhs);
//	q3Mat3& operator-=(const q3Mat3& rhs);
//
//	D3DXVECTOR3& operator[](UINT index);
//	const D3DXVECTOR3& operator[](UINT index) const;
//	const D3DXVECTOR3 Column0() const;
//	const D3DXVECTOR3 Column1() const;
//	const D3DXVECTOR3 Column2() const;
//
//	const D3DXVECTOR3 operator*(const D3DXVECTOR3& rhs) const;
//	const q3Mat3 operator*(const q3Mat3& rhs) const;
//	const q3Mat3 operator*(float f) const;
//	const q3Mat3 operator+(const q3Mat3& rhs) const;
//	const q3Mat3 operator-(const q3Mat3& rhs) const;
//};
//
//
//
//
//
////--------------------------------------------------------------------------------------------------
//// q3Mat3
////--------------------------------------------------------------------------------------------------
//inline void q3Identity(q3Mat3& m)
//{
//	m.Set(
//		float(1.0), float(0.0), float(0.0),
//		float(0.0), float(1.0), float(0.0),
//		float(0.0), float(0.0), float(1.0)
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3Rotate(const D3DXVECTOR3& x, const D3DXVECTOR3& y, const D3DXVECTOR3& z)
//{
//	return q3Mat3(x, y, z);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3Transpose(const q3Mat3& m)
//{
//	return q3Mat3(
//		m.ex.x, m.ey.x, m.ez.x,
//		m.ex.y, m.ey.y, m.ez.y,
//		m.ex.z, m.ey.z, m.ez.z
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//inline void q3Zero(q3Mat3& m)
//{
//	memset(&m, 0, sizeof(float) * 9);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3Diagonal(float a)
//{
//	return q3Mat3(
//		float(a), float(0.0), float(0.0),
//		float(0.0), float(a), float(0.0),
//		float(0.0), float(0.0), float(a)
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3Diagonal(float a, float b, float c)
//{
//	return q3Mat3(
//		float(a), float(0.0), float(0.0),
//		float(0.0), float(b), float(0.0),
//		float(0.0), float(0.0), float(c)
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3OuterProduct(const D3DXVECTOR3& u, const D3DXVECTOR3& v)
//{
//	D3DXVECTOR3 a = v * u.x;
//	D3DXVECTOR3 b = v * u.y;
//	D3DXVECTOR3 c = v * u.z;
//
//	return q3Mat3(
//		a.x, a.y, a.z,
//		b.x, b.y, b.z,
//		c.x, c.y, c.z
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//inline const q3Mat3 q3Covariance(D3DXVECTOR3 *points, UINT numPoints)
//{
//	float invNumPoints = float(1.0) / float(numPoints);
//	D3DXVECTOR3 c = D3DXVECTOR3(float(0.0), float(0.0), float(0.0));
//
//	for (UINT i = 0; i < numPoints; ++i)
//		c += points[i];
//
//	c /= float(numPoints);
//
//	float m00, m11, m22, m01, m02, m12;
//	m00 = m11 = m22 = m01 = m02 = m12 = float(0.0);
//
//	for (UINT i = 0; i < numPoints; ++i)
//	{
//		D3DXVECTOR3 p = points[i] - c;
//
//		m00 += p.x * p.x;
//		m11 += p.y * p.y;
//		m22 += p.z * p.z;
//		m01 += p.x * p.y;
//		m02 += p.x * p.z;
//		m12 += p.y * p.z;
//	}
//
//	float m01inv = m01 * invNumPoints;
//	float m02inv = m02 * invNumPoints;
//	float m12inv = m12 * invNumPoints;
//
//	return q3Mat3(
//		m00 * invNumPoints, m01inv, m02inv,
//		m01inv, m11 * invNumPoints, m12inv,
//		m02inv, m12inv, m22 * invNumPoints
//	);
//};
//
////--------------------------------------------------------------------------------------------------
////inline const q3Mat3 q3Inverse(const q3Mat3& m)
////{
////	D3DXVECTOR3 tmp0, tmp1, tmp2;
////	float detinv;
////
////	tmp0 = q3Cross(m.ey, m.ez);
////	tmp1 = q3Cross(m.ez, m.ex);
////	tmp2 = q3Cross(m.ex, m.ey);
////
////	detinv = float(1.0) / q3Dot(m.ez, tmp2);
////
////	return q3Mat3(
////		tmp0.x * detinv, tmp1.x * detinv, tmp2.x * detinv,
////		tmp0.y * detinv, tmp1.y * detinv, tmp2.y * detinv,
////		tmp0.z * detinv, tmp1.z * detinv, tmp2.z * detinv
////	);
////}
