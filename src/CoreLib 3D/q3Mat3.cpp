//#include <cassert>
//#include "q3Mat3.h"
//
////--------------------------------------------------------------------------------------------------
//// q3Mat3
////--------------------------------------------------------------------------------------------------
//q3Mat3::q3Mat3()
//{
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3::q3Mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
//	: ex(a, b, c)
//	, ey(d, e, f)
//	, ez(g, h, i)
//{
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3::q3Mat3(const D3DXVECTOR3& _x, const D3DXVECTOR3& _y, const D3DXVECTOR3& _z)
//	: ex(_x)
//	, ey(_y)
//	, ez(_z)
//{
//}
//
////--------------------------------------------------------------------------------------------------
//void q3Mat3::Set(float a, float b, float c, float d, float e, float f, float g, float h, float i)
//{
//	ex = {a, b, c};
//	ey = {d, e, f};
//	ez = {g, h, i};
//}
//
////--------------------------------------------------------------------------------------------------
//void q3Mat3::Set(const D3DXVECTOR3& axis, float angle)
//{
//	float s = std::sin(angle);
//	float c = std::cos(angle);
//	float x = axis.x;
//	float y = axis.y;
//	float z = axis.z;
//	float xy = x * y;
//	float yz = y * z;
//	float zx = z * x;
//	float t = float(1.0) - c;
//
//	Set(
//		x * x * t + c, xy * t + z * s, zx * t - y * s,
//		xy * t - z * s, y * y * t + c, yz * t + x * s,
//		zx * t + y * s, yz * t - x * s, z * z * t + c
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//void q3Mat3::SetRows(const D3DXVECTOR3& x, const D3DXVECTOR3& y, const D3DXVECTOR3& z)
//{
//	ex = x;
//	ey = y;
//	ez = z;
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3& q3Mat3::operator=(const q3Mat3& rhs)
//{
//	ex = rhs.ex;
//	ey = rhs.ey;
//	ez = rhs.ez;
//
//	return *this;
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3& q3Mat3::operator*=(const q3Mat3& rhs)
//{
//	*this = *this * rhs;
//
//	return *this;
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3& q3Mat3::operator*=(float f)
//{
//	ex *= f;
//	ey *= f;
//	ez *= f;
//
//	return *this;
//}
//
//
////--------------------------------------------------------------------------------------------------
//q3Mat3& q3Mat3::operator+=(const q3Mat3& rhs)
//{
//	ex += rhs.ex;
//	ey += rhs.ey;
//	ez += rhs.ez;
//
//	return *this;
//}
//
////--------------------------------------------------------------------------------------------------
//q3Mat3& q3Mat3::operator-=(const q3Mat3& rhs)
//{
//	ex -= rhs.ex;
//	ey -= rhs.ey;
//	ez -= rhs.ez;
//
//	return *this;
//}
//
////--------------------------------------------------------------------------------------------------
//D3DXVECTOR3& q3Mat3::operator[](UINT index)
//{
//	switch (index)
//	{
//	case 0:
//		return ex;
//	case 1:
//		return ey;
//	case 2:
//		return ez;
//	default:
//		assert(false);
//		return ex;
//	}
//}
//
////--------------------------------------------------------------------------------------------------
//const D3DXVECTOR3& q3Mat3::operator[](UINT index) const
//{
//	switch (index)
//	{
//	case 0:
//		return ex;
//	case 1:
//		return ey;
//	case 2:
//		return ez;
//	default:
//		assert(false);
//		return ex;
//	}
//}
//
////--------------------------------------------------------------------------------------------------
//const D3DXVECTOR3 q3Mat3::Column0() const
//{
//	return D3DXVECTOR3(ex.x, ey.x, ez.x);
//}
//
////--------------------------------------------------------------------------------------------------
//const D3DXVECTOR3 q3Mat3::Column1() const
//{
//	return D3DXVECTOR3(ex.y, ey.y, ez.y);
//}
//
////--------------------------------------------------------------------------------------------------
//const D3DXVECTOR3 q3Mat3::Column2() const
//{
//	return D3DXVECTOR3(ex.z, ey.z, ez.z);
//}
//
////--------------------------------------------------------------------------------------------------
//const D3DXVECTOR3 q3Mat3::operator*(const D3DXVECTOR3& rhs) const
//{
//	return D3DXVECTOR3(
//		ex.x * rhs.x + ey.x * rhs.y + ez.x * rhs.z,
//		ex.y * rhs.x + ey.y * rhs.y + ez.y * rhs.z,
//		ex.z * rhs.x + ey.z * rhs.y + ez.z * rhs.z
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//const q3Mat3 q3Mat3::operator*(const q3Mat3& rhs) const
//{
//	return q3Mat3(
//		(*this * rhs.ex),
//		(*this * rhs.ey),
//		(*this * rhs.ez)
//	);
//}
//
////--------------------------------------------------------------------------------------------------
//const q3Mat3 q3Mat3::operator*(float f) const
//{
//	return q3Mat3(ex * f, ey * f, ez * f);
//}
//
////--------------------------------------------------------------------------------------------------
//const q3Mat3 q3Mat3::operator+(const q3Mat3& rhs) const
//{
//	return q3Mat3(ex + rhs.ex, ey + rhs.ey, ez + rhs.ez);
//}
//
////--------------------------------------------------------------------------------------------------
//const q3Mat3 q3Mat3::operator-(const q3Mat3& rhs) const
//{
//	return q3Mat3(ex - rhs.ex, ey - rhs.ey, ez - rhs.ez);
//}
