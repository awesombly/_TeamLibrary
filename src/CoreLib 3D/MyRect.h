#pragma once
#include "header.h"

class MyRect;

class MyVector2 {
public:
	MyVector2() noexcept = default;
	MyVector2(const int& _x, const int& _y) noexcept : x((float)_x), y((float)_y) {};
	MyVector2(const float& _x, const float& _y) noexcept : x(_x), y(_y) {};
	MyVector2(const POINT& _point) noexcept :x((float)_point.x), y((float)_point.y) {};
	MyVector2(const MyVector2& _point)	noexcept = default;
	MyVector2(MyVector2&& _point)		noexcept = default;
	~MyVector2()						noexcept = default;
public:
	float x, y;
public:
	// + 내적, 외적

	// 정규화
	MyVector2 Normalize() const noexcept
	{
		float length = sqrt(x * x + y * y);
		return { x / length, y / length };
	}
	// 목표 방향 단위벡터 반환
	MyVector2 toPointNormalize(const MyVector2& _point) const noexcept
	{
		return (_point - *this).Normalize();
	}
	// 길이(크기) 얻기
	float getLength() const noexcept
	{
		return sqrt(myStd::PowT(x, 2) + myStd::PowT(y, 2));
	}
	// 목표까지 루트 안벗긴 거리
	float getSquareDistance(const MyVector2& _point) const noexcept
	{
		return myStd::PowT(myStd::AbsT(x - _point.x), 2) + myStd::PowT(myStd::AbsT(y - _point.y), 2);
	}
	// 목표까지 거리
	float getDistance(const MyVector2& _point) const noexcept
	{
		return sqrt(myStd::PowT(myStd::AbsT(x - _point.x), 2) + myStd::PowT(myStd::AbsT(y - _point.y), 2));
	}
	MyVector2 & operator =(const MyVector2& _point)  noexcept = default;
	MyVector2 & operator =(MyVector2&& _point) noexcept = default;
	MyVector2 & operator =(const POINT& _point)  noexcept
	{
		x = (float)_point.x;
		y = (float)_point.y;
		return *this;
	}
	MyVector2 & operator =(const int& value)  noexcept
	{
		x = (float)value;
		y = (float)value;
		return *this;
	}
	MyVector2 & operator =(const float& value)  noexcept
	{
		x = value;
		y = value;
		return *this;
	}
	MyVector2& operator +=(const MyVector2& _point)	noexcept
	{
		x += _point.x;
		y += _point.y;
		return *this;
	}
	MyVector2& operator -=(const MyVector2& _point)	noexcept
	{
		x -= _point.x;
		y -= _point.y;
		return *this;
	}
	MyVector2& operator +=(const int& _value)		noexcept
	{
		x += _value;
		y += _value;
		return *this;
	}
	MyVector2& operator +=(const float& _value)		noexcept
	{
		x += _value;
		y += _value;
		return *this;
	}
	MyVector2& operator -=(const int& _value)		noexcept
	{
		x -= _value;
		y -= _value;
		return *this;
	}
	MyVector2& operator -=(const float& _value)		noexcept
	{
		x -= _value;
		y -= _value;
		return *this;
	}
	MyVector2& operator *=(const int& _value)		noexcept
	{
		x *= _value;
		y *= _value;
		return *this;
	}
	MyVector2& operator *=(const float& _value)		noexcept
	{
		x *= _value;
		y *= _value;
		return *this;
	}
	MyVector2& operator /=(const int& _value) noexcept
	{
		x /= _value;
		y /= _value;
		return *this;
	}
	MyVector2& operator /=(const float& _value)		noexcept
	{
		x /= _value;
		y /= _value;
		return *this;
	}
	MyVector2 operator +(const MyVector2& _point) const noexcept
	{
		return { x + _point.x, y + _point.y };
	}
	MyVector2 operator +(const int& _value) const noexcept
	{
		return { x + _value, y + _value };
	}
	MyVector2 operator +(const float& _value) const noexcept
	{
		return { x + _value, y + _value };
	}
	MyVector2 operator -(const MyVector2& _point) const noexcept
	{
		return { x - _point.x, y - _point.y };
	}
	MyVector2 operator -(const int& _value) const noexcept
	{
		return { x - _value, y - _value };
	}
	MyVector2 operator -(const float& _value) const noexcept
	{
		return { x - _value, y - _value };
	}
	MyVector2 operator *(const int& _value) const noexcept
	{
		return { x * _value, y * _value };
	}
	MyVector2 operator *(const float& _value) const noexcept
	{
		return { x * _value, y * _value };
	}
	MyVector2 operator /(const int& _value)	const noexcept
	{
		return { x / _value, y / _value };
	}
	MyVector2 operator /(const float& _value) const noexcept
	{
		return { x / _value, y / _value };
	}
	friend MyVector2 operator +(const POINT& _point, const MyVector2& _vector2) noexcept
	{
		return { _point.x + _vector2.x, _point.y + _vector2.y };
	}
	friend MyVector2 operator -(const POINT& _point, const MyVector2& _vector2) noexcept
	{
		return { _point.x - _vector2.x, _point.y - _vector2.y };
	}
};


class MyVector3 {
public:
	MyVector3() noexcept = default;
	MyVector3(const int& _x, const int& _y, const int& _z) noexcept : x((float)_x), y((float)_y), z((float)_z) {};
	MyVector3(const float& _x, const float& _y, const float& _z) noexcept : x(_x), y(_y), z(_z) {};
	MyVector3(const MyVector3& _vector3) noexcept = default;
	MyVector3(MyVector3&& _vector3) noexcept = default;
	~MyVector3() noexcept = default;
public:
	float x, y, z;
public:
	MyVector3 & operator =(const MyVector3& _vector3)  noexcept = default;
	MyVector3 & operator =(MyVector3&& _vector3)  noexcept = default;
	MyVector3 & operator =(const int& value)  noexcept
	{
		x = (float)value;
		y = (float)value;
		z = (float)value;
		return *this;
	}
	MyVector3 & operator =(const float& value)  noexcept
	{
		x = value;
		y = value;
		y = value;
		return *this;
	}
	MyVector3& operator +=(const MyVector3& _point)  noexcept
	{
		x += _point.x;
		y += _point.y;
		z += _point.z;
		return *this;
	}
	MyVector3& operator +=(const int& _value)  noexcept
	{
		x += _value;
		y += _value;
		z += _value;
		return *this;
	}
	MyVector3& operator +=(const float& _value)  noexcept
	{
		x += _value;
		y += _value;
		z += _value;
		return *this;
	}
	MyVector3& operator -=(const MyVector3& _point)  noexcept
	{
		x -= _point.x;
		y -= _point.y;
		z -= _point.z;
		return *this;
	}
	MyVector3& operator -=(const int& _value)  noexcept
	{
		x -= _value;
		y -= _value;
		z -= _value;
		return *this;
	}
	MyVector3& operator -=(const float& _value)  noexcept
	{
		x -= _value;
		y -= _value;
		z -= _value;
		return *this;
	}
	MyVector3& operator *=(const int& _value)  noexcept
	{
		x *= _value;
		y *= _value;
		z *= _value;
		return *this;
	}
	MyVector3& operator *=(const float& _value)  noexcept
	{
		x *= _value;
		y *= _value;
		z *= _value;
		return *this;
	}
	MyVector3& operator /=(const int& _value)  noexcept
	{
		x /= _value;
		y /= _value;
		z /= _value;
		return *this;
	}
	MyVector3& operator /=(const float& _value)  noexcept
	{
		x /= _value;
		y /= _value;
		z /= _value;
		return *this;
	}
	MyVector3 operator +(const MyVector3& _point) const noexcept
	{
		return { x + _point.x, y + _point.y, z + _point.z };
	}
	MyVector3 operator +(const int& _value) const noexcept
	{
		return { x + _value, y + _value, z + _value };
	}
	MyVector3 operator +(const float& _value) const noexcept
	{
		return { x + _value, y + _value, z + _value };
	}
	MyVector3 operator -(const MyVector3& _point) const noexcept
	{
		return { x - _point.x, y - _point.y, z - _point.z };
	}
	MyVector3 operator -(const int& _value) const noexcept
	{
		return { x - _value, y - _value, z - _value };
	}
	MyVector3 operator -(const float& _value) const noexcept
	{
		return { x - _value, y - _value, z - _value };
	}
	MyVector3 operator *(const int& _value) const noexcept
	{
		return { x * _value, y * _value, z * _value };
	}
	MyVector3 operator *(const float& _value) const noexcept
	{
		return { x * _value, y * _value, z * _value };
	}
	MyVector3 operator /(const int& _value) const noexcept
	{
		return { x / _value, y / _value, z / _value };
	}
	MyVector3 operator /(const float& _value) const noexcept
	{
		return { x / _value, y / _value, z / _value };
	}
};

class MyVector4 {
public:
	MyVector4() noexcept = default;
	MyVector4(const int& _x, const int& _y, const int& _z, const int& _w) noexcept : x((float)_x), y((float)_y), z((float)_z), w((float)_w) {};
	MyVector4(const float& _x, const float& _y, const float& _z, const float& _w) noexcept : x(_x), y(_y), z(_z), w(_w) {};
	MyVector4(const MyRect& _rect) noexcept;
	MyVector4(const MyVector4& _vector4) noexcept = default;
	MyVector4(MyVector4&& _vector4) noexcept = default;
	~MyVector4() noexcept = default;
public:
	float x, y, z, w;
public:
	MyVector4 getPosVector(const int& width, const int& height) noexcept
	{
		return { x / width, y / height, (x + z) / width, (y + w) / height };
	}
	MyVector4 & operator =(const MyVector4& _vector4)  noexcept = default;
	MyVector4 & operator =(MyVector4&& _vector4)  noexcept = default;
	MyVector4 & operator =(const int& value)  noexcept
	{
		x = (float)value;
		y = (float)value;
		z = (float)value;
		w = (float)value;
		return *this;
	}
	MyVector4 & operator =(const float& value)  noexcept
	{
		x = value;
		y = value;
		y = value;
		w = value;
		return *this;
	}
	MyVector4& operator +=(const MyVector4& _point)  noexcept
	{
		x += _point.x;
		y += _point.y;
		y += _point.z;
		w += _point.w;
		return *this;
	}
	MyVector4& operator +=(const int& _value)  noexcept
	{
		x += _value;
		y += _value;
		y += _value;
		w += _value;
		return *this;
	}
	MyVector4& operator +=(const float& _value)  noexcept
	{
		x += _value;
		y += _value;
		y += _value;
		w += _value;
		return *this;
	}
	MyVector4& operator -=(const MyVector4& _point)  noexcept
	{
		x -= _point.x;
		y -= _point.y;
		y -= _point.z;
		w -= _point.w;
		return *this;
	}
	MyVector4& operator -=(const int& _value)  noexcept
	{
		x -= _value;
		y -= _value;
		y -= _value;
		w -= _value;
		return *this;
	}
	MyVector4& operator -=(const float& _value)  noexcept
	{
		x -= _value;
		y -= _value;
		y -= _value;
		w -= _value;
		return *this;
	}
	MyVector4& operator *=(const int& _value)  noexcept
	{
		x *= _value;
		y *= _value;
		y *= _value;
		w *= _value;
		return *this;
	}
	MyVector4& operator *=(const float& _value)  noexcept
	{
		x *= _value;
		y *= _value;
		y *= _value;
		w *= _value;
		return *this;
	}
	MyVector4& operator /=(const int& _value)  noexcept
	{
		x /= _value;
		y /= _value;
		y /= _value;
		w /= _value;
		return *this;
	}
	MyVector4& operator /=(const float& _value)  noexcept
	{
		x /= _value;
		y /= _value;
		y /= _value;
		w /= _value;
		return *this;
	}
	MyVector4 operator +(const MyVector4& _point) const noexcept
	{
		return { x + _point.x, y + _point.y, z + _point.z, w + _point.w };
	}
	MyVector4 operator +(const int& _value) const noexcept
	{
		return { x + _value, y + _value, z + _value, w + _value};
	}
	MyVector4 operator +(const float& _value) const noexcept
	{
		return { x + _value, y + _value, z + _value, w + _value };
	}
	MyVector4 operator -(const MyVector4& _point) const noexcept
	{
		return { x - _point.x, y - _point.y, z - _point.z, w - _point.w };
	}
	MyVector4 operator -(const int& _value) const noexcept
	{
		return { x - _value, y - _value, z - _value, w - _value };
	}
	MyVector4 operator -(const float& _value) const noexcept
	{
		return { x - _value, y - _value, z - _value, w - _value };
	}
	MyVector4 operator *(const int& _value) const noexcept
	{
		return { x * _value, y * _value, z * _value, w * _value };
	}
	MyVector4 operator *(const float& _value) const noexcept
	{
		return { x * _value, y * _value, z * _value, w * _value };
	}
	MyVector4 operator /(const int& _value) const noexcept
	{
		return { x / _value, y / _value, z / _value, w / _value };
	}
	MyVector4 operator /(const float& _value) const noexcept
	{
		return { x / _value, y / _value, z / _value, w / _value };
	}
};


class MyRect
{
public:
	MyRect()				   noexcept	= default;
	MyRect(const MyRect& rect) noexcept = default;
	MyRect(MyRect&& rect)	   noexcept = default;
	MyRect(const int& x, const int& y, const int& width, const int& height) noexcept;
	MyRect(const float& x, const float& y, const int& width, const int& height) noexcept;
	~MyRect() noexcept;

	virtual void Move(const int& x, const int& y)		noexcept;
	virtual void Move(const float& x, const float& y)	noexcept;
	virtual void Move(const MyVector2& point)			noexcept;
	virtual void Move(const POINT& point)				noexcept;

	bool CollisionCheck(const MyRect& rect)		const noexcept;
	bool CollisionCheck(const RECT& rect)		const noexcept;
	bool CollisionCheck(const MyVector2& point) const noexcept;
	bool CollisionCheck(const POINT& point)		const noexcept;
	bool CollisionCheck(const float& distance,	const float& targetRadius) const noexcept;
	//MyRect Intersection(const MyRect& rect)		const noexcept;
	//MyRect Union(const MyRect& rect)			const noexcept;
	//void DrawRect() const noexcept;
	//void DrawRect(const MyRect& rect)			const noexcept;

	MyVector2 getPoint()	const noexcept;
	MyVector2 getCenter()	const noexcept;
	int getCenterX()		const noexcept;
	int getCenterY()		const noexcept;
	MyVector2 getWH()		const noexcept;
	int getWidth()			const noexcept;
	int getHeight()			const noexcept;
	int getX()				const noexcept;
	int getY()				const noexcept;
	int getLeft()			const noexcept;
	int getRight()			const noexcept;
	int getTop()			const noexcept;
	int getBottom()			const noexcept;

	virtual void setWidth(const int& width)						noexcept;
	virtual void setHeight(const int& height)					noexcept;
	virtual void setWH(const int& width, const int& height)		noexcept;
	virtual void setWH(const float& width, const float& height)	noexcept;
	virtual void setWH(const MyVector2& width_height)			noexcept;

	virtual void setX(const int& x)							noexcept;
	virtual void setX(const float& x)						noexcept;
	virtual void setY(const int& y)							noexcept;
	virtual void setY(const float& y)						noexcept;
	virtual void setPoint(const int& x, const int& y)		noexcept;
	virtual void setPoint(const float& x, const float& y)	noexcept;
	virtual void setPoint(const MyVector2& point)			noexcept;
	virtual void setCenter(const int& x, const int& y)		noexcept;
	virtual void setCenter(const float& x, const float& y)	noexcept;
	virtual void setCenterX(const int& x)					noexcept;
	virtual void setCenterX(const float& x)					noexcept;
	virtual void setCenterY(const int& y)					noexcept;
	virtual void setCenterY(const float& y)					noexcept;
	virtual void setCenter(const MyVector2& point)			noexcept;
	virtual void setCenter(const POINT& point)				noexcept;
	virtual void setTop(const int& value)					noexcept;
	virtual void setLeft(const int& value)					noexcept;
	virtual void setRight(const int& value)					noexcept;
	virtual void setBottom(const int& value)				noexcept;
	
	virtual void setRect(const MyRect& rect)				noexcept;
	virtual void setRect(const RECT& rect)					noexcept;
	virtual void setRect(const int& x, const int& y, const int& width, const int& height) noexcept;
	virtual void setRect(const float& x, const float& y, const int& width, const int& height) noexcept;
	virtual void setRect(const float& x, const float& y, const float& width, const float& height) noexcept;
	virtual void RECTset(RECT* rect)	const noexcept;

	virtual MyRect& operator =(const MyRect&) noexcept = default;
	virtual MyRect& operator =(MyRect&&)	  noexcept = default;
	MyRect& operator =(const RECT& rect)	  noexcept;
	//MyRect& operator +(const MyRect&)		  noexcept;
	//MyRect& operator *(const MyRect&)		  noexcept;
	//MyRect operator +(const int&)			  noexcept;
	//MyRect operator -(const int&)			  noexcept;
	//friend MyRect& operator +(const int&, const MyRect& rect) noexcept;
	//friend MyRect& operator -(const int&, const MyRect& rect) noexcept;
	MyRect operator *(const int& _value)  noexcept
	{
		return { m_point.x * _value, m_point.y * _value,  (int)(m_width * _value), (int)(m_height * _value) };
	}
	MyRect operator *(const float& _value)  noexcept
	{
		return { m_point.x * _value, m_point.y * _value,  (int)(m_width * _value),(int)(m_height * _value) };
	}
	MyRect operator /(const int& _value)  noexcept
	{
		return { m_point.x / _value, m_point.y / _value,  (int)(m_width / _value), (int)(m_height / _value) };
	}
	MyRect operator /(const float& _value)  noexcept
	{
		return { m_point.x / _value, m_point.y / _value,  (int)(m_width / _value), (int)(m_height / _value) };
	}
	MyRect operator /(const MyRect& _rect)  noexcept
	{
		return { m_point.x / _rect.m_point.x, m_point.y / _rect.m_point.y,  (int)(m_width / _rect.m_width), (int)(m_height / _rect.m_height) };
	}
	MyRect operator *(const MyRect& _rect)  noexcept
	{
		return { m_point.x * _rect.m_point.x, m_point.y * _rect.m_point.y,  (int)(m_width * _rect.m_width), (int)(m_height * _rect.m_height) };
	}

	static float getAngle(const int& sx, const int& sy, const int& dx, const int& dy) noexcept;
	static float getAngle(const MyVector2& sP, const MyVector2& dP) noexcept;
public:
	MyVector2 m_point;
	int m_width, m_height;
};