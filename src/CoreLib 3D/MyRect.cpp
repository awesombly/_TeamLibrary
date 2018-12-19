#include "MyRect.h"


MyVector4::MyVector4(const MyRect& _rect) noexcept 
	: x(_rect.m_point.x), y(_rect.m_point.y), z((float)_rect.m_width), w((float)_rect.m_height) 
{};


MyRect::MyRect(const int& x, const int& y, const int& width, const int& height) noexcept
	: m_point((float)x, (float)y), m_width(width), m_height(height) 
{}

MyRect::MyRect(const float& x, const float& y, const int& width, const int& height) noexcept
	: m_point(x, y), m_width(width), m_height(height)
{}

MyRect::~MyRect() {}


void MyRect::Move(const int& x, const int& y) noexcept
{
	m_point.x += (int)x;
	m_point.y += (int)y;
}
void MyRect::Move(const float& x, const float& y) noexcept
{
	m_point.x += x;
	m_point.y += y;
}
void MyRect::Move(const MyVector2& point) noexcept
{
	m_point.x += point.x;
	m_point.y += point.y;
}
void MyRect::Move(const POINT& point) noexcept
{
	m_point.x += (float)point.x;
	m_point.y += (float)point.y;
}

bool MyRect::CollisionCheck(const MyVector2& point) const noexcept
{
	if (point.x > m_point.x && point.x < m_point.x + m_width &&
		point.y > m_point.y && point.y < m_point.y + m_height)
	{
		return true;
	}
	return false;
}
bool MyRect::CollisionCheck(const POINT& point) const noexcept
{
	if (point.x > m_point.x && point.x < m_point.x + m_width &&
		point.y > m_point.y && point.y < m_point.y + m_height)
	{
		return true;
	}
	return false;
}

bool MyRect::CollisionCheck(const MyRect& rect) const noexcept
{
	if (m_point.x <= rect.m_point.x + rect.m_width &&
		m_point.x + m_width >= rect.m_point.x &&
		m_point.y <= rect.m_point.y + rect.m_height &&
		m_point.y + m_height >= rect.m_point.y)
		return true;
	return false;
}
bool MyRect::CollisionCheck(const RECT& rect) const noexcept
{
	if (m_point.x <= rect.left + rect.right &&
		m_point.x + m_width >= rect.left &&
		m_point.y <= rect.top + rect.bottom &&
		m_point.y + m_height >= rect.top)
		return true;
	return false;
}
// 반지름(원)으로 체크
bool MyRect::CollisionCheck(const float& distance, const float& targetRadius) const noexcept
{
	if(distance <= m_width / 2.0f + targetRadius)
		return true;
	return false;
}

//MyRect MyRect::Intersection(const MyRect& rect) const noexcept
//{
//	// 충돌 체크
//	if (!CollisionCheck(rect)) return MyRect();
//
//	MyRect pRect;
//	// 큰 left, 큰 top, 작은 right, 작은 bottom
//	pRect.m_point.x = std::fmax(m_point.x, rect.m_point.x);
//	pRect.m_point.y = std::fmax(m_point.y, rect.m_point.y);
//	pRect.m_width   = (int)(std::fmin(m_point.x + m_width, rect.m_point.x + rect.m_width) - pRect.m_point.x);
//	pRect.m_height  = (int)(std::fmin(m_point.y + m_height, rect.m_point.y + rect.m_height) - pRect.m_point.y);
//
//	return pRect;
//}
//
//MyRect MyRect::Union(const MyRect& rect) const noexcept
//{
//	//if (!CollisionCheck(rect)) return nullptr;
//	MyRect pRect;
//
//	// 작은 left, 작은 top, 큰 right, 큰 bottom
//	pRect.m_point.x = std::fmin(m_point.x, rect.m_point.x);
//	pRect.m_point.y = std::fmin(m_point.y, rect.m_point.y);
//	pRect.m_width   = (int)(std::fmax(m_point.x + m_width, rect.m_point.x + rect.m_width) - pRect.m_point.x);
//	pRect.m_height  = (int)(std::fmax(m_point.y + m_height, rect.m_point.y + rect.m_height) - pRect.m_point.y);
//
//	return pRect;
//}

//void MyRect::DrawRect()  const noexcept
//{
//	DrawRect(*this);
//}
//
//void MyRect::DrawRect(const MyRect& rect)  const noexcept
//{
//	try
//	{
//		std::cout << "Left : " << rect.getLeft() << ",\tTop : " << rect.getTop() << ",\tRight : " << rect.getRight() << ",\tBottom : " << rect.getBottom() << std::endl;
//	}
//	catch (...)
//	{
//		std::cout << "null" << std::endl;
//	}
//
//	int right = rect.getRight();
//	int bottom = rect.getBottom();
//	for (int h = 0; h < right; h++)
//	{
//		for (int w = 0; w < bottom; w++)
//		{
//			if (h < rect.m_point.y || w < rect.m_point.x)
//			{
//				std::cout << "·";
//				continue;
//			}
//			std::cout << "■";
//		}
//		std::cout << std::endl;
//	}
//}

MyVector2 MyRect::getPoint() const noexcept
{
	return m_point;
}
int MyRect::getWidth() const noexcept
{
	return m_width;
}
int MyRect::getHeight() const noexcept
{
	return m_height;
}
MyVector2 MyRect::getCenter() const noexcept
{
	return {m_point.x + m_width / 2, m_point.y + m_height / 2 };
}
int MyRect::getCenterX() const noexcept
{
	return (int)(m_point.x + m_width / 2);
}
int MyRect::getCenterY() const noexcept
{
	return (int)(m_point.y + m_height / 2);
}
MyVector2 MyRect::getWH()	const noexcept
{
	return MyVector2(m_width, m_height);
}
int	MyRect::getX() const noexcept
{
	return (int)m_point.x;
}
int MyRect::getY() const noexcept
{
	return (int)m_point.y;
}
int MyRect::getLeft() const noexcept
{
	return (int)m_point.x;
}
int MyRect::getRight() const noexcept
{
	return (int)m_point.x + m_width;
}
int MyRect::getTop() const noexcept
{
	return (int)m_point.y;
}
int MyRect::getBottom() const noexcept
{
	return (int)m_point.y + m_height;
}
void MyRect::setX(const int& x) noexcept
{
	m_point.x = (float)x;
}
void MyRect::setX(const float& x) noexcept
{
	m_point.x = x;
}
void MyRect::setY(const int& y) noexcept
{
	m_point.y = (float)y;
}
void MyRect::setY(const float& y) noexcept
{
	m_point.y = y;
}
void MyRect::setPoint(const int& x, const int& y) noexcept
{
	m_point = { (float)x, (float)y };
}
void MyRect::setPoint(const float& x, const float& y) noexcept
{
	m_point = { x, y };
}
void MyRect::setPoint(const MyVector2& point) noexcept
{
	m_point = point;
}
void MyRect::setWidth(const int& width) noexcept
{
	m_width = width;
}
void MyRect::setHeight(const int& height) noexcept
{
	m_height = height;
}
void MyRect::setWH(const int& width, const int& height)	noexcept
{
	m_width = width;
	m_height = height;
}
void MyRect::setWH(const float& width, const float& height)	noexcept
{
	m_width = (int)width;
	m_height = (int)height;
}
void MyRect::setWH(const MyVector2& width_height)			noexcept
{
	m_width = (int)width_height.x;
	m_height = (int)width_height.y;
}
void MyRect::setLeft(const int& value) noexcept
{
	m_point.x = (float)value;
}
void MyRect::setTop(const int& value) noexcept
{
	m_point.y = (float)value;
}
void MyRect::setRight(const int& value) noexcept
{
	m_point.x = (float)(value - m_width);
}
void MyRect::setBottom(const int& value) noexcept
{
	m_point.y = (float)(value - m_height);
}

void MyRect::setRect(const RECT& rect) noexcept
{
	setRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}
void MyRect::setRect(const MyRect& rect) noexcept
{
	*this = rect;
}
void MyRect::setRect(const int& x, const int& y, const int& width, const int& height) noexcept
{
	m_point = { (float)x, (float)y };
	m_width = width;
	m_height = height;
}
void MyRect::setRect(const float& x, const float& y, const int& width, const int& height) noexcept
{
	m_point = { x, y };
	m_width = width;
	m_height = height;
}
void MyRect::setRect(const float& x, const float& y, const float& width, const float& height) noexcept
{
	m_point = { x, y };
	m_width = (int)width;
	m_height = (int)height;
}
void MyRect::setCenter(const int& x, const int& y) noexcept
{
	m_point.x = (float)(x - m_width / 2);
	m_point.y = (float)(y - m_height / 2);
}
void MyRect::setCenter(const float& x, const float& y) noexcept
{
	m_point.x = x - m_width / 2;
	m_point.y = y - m_height / 2;
}
void MyRect::setCenter(const MyVector2& point) noexcept
{
	m_point.x = point.x - m_width / 2;
	m_point.y = point.y - m_height / 2;
}
void MyRect::setCenter(const POINT& point) noexcept
{
	m_point.x = (float)(point.x - m_width / 2);
	m_point.y = (float)(point.y - m_height / 2);
}
void MyRect::setCenterX(const int& _x) noexcept
{
	m_point.x = (float)(_x - m_width / 2);
}
void MyRect::setCenterX(const float& _x) noexcept
{
	m_point.x = (_x - m_width / 2);
}
void MyRect::setCenterY(const int& _y) noexcept
{
	m_point.y = (float)(_y - m_height / 2);
}
void MyRect::setCenterY(const float& _y) noexcept
{
	m_point.y = (_y - m_height / 2);
}

void MyRect::RECTset(RECT* rect) const noexcept
{
	rect->left = (long)m_point.x;
	rect->top = (long)m_point.y;
	rect->right = m_width;
	rect->bottom = m_height;
}

MyRect& MyRect::operator =(const RECT& rect) noexcept
{
	setRect(rect);
	return *this;
}

//MyRect& MyRect::operator +(const MyRect& rect) noexcept
//{
//	return *this + rect;
//}

//MyRect& MyRect::operator *(const MyRect& rect) noexcept
//{
//	return Intersection(rect);
//}

//MyRect MyRect::operator +(const int& iValue) noexcept
//{
//	return MyRect(*this) + iValue;
//}
//MyRect MyRect::operator -(const int& iValue) noexcept
//{
//	return MyRect(*this) - iValue;
//}
//MyRect& operator +(const int& iValue, const MyRect& rect) noexcept
//{
//	MyRect* tempRect = new MyRect(rect);
//	tempRect->Move(iValue, iValue);
//
//	return *tempRect;
//}
//MyRect& operator -(const int& iValue, const MyRect& rect) noexcept
//{
//	MyRect* tempRect = new MyRect(rect);
//	tempRect->Move(-iValue, -iValue);
//
//	return *tempRect;
//}


float MyRect::getAngle(const int& sx, const int& sy, const int& dx, const int& dy) noexcept
{
	int&& vx = dx - sx;
	int&& vy = dy - sy;
	float&& radian = (float)atan2(vy, vx);
	float&& degree = (radian * 57.3f);
	if (degree < 0)
		degree += 360;
	return degree;
}

float MyRect::getAngle(const MyVector2& sP, const MyVector2& dP) noexcept
{
	float&& vx = dP.x - sP.x;
	float&& vy = dP.y - sP.y;
	float&& radian = atan2(vy, vx);
	float&& degree = (radian * 57.3f);
	if (degree < 0)
		degree += 360;
	return degree;
}
