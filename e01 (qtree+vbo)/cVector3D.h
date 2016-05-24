#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

//#define M_PI       3.14159265358979323846
#define M_PIdiv180		0.0174532925199432957692
#define M_180divPI		57.2957795130823208767


class cVector3D
{
public:
    
	float   x,y,z;
       
	cVector3D() {}
	cVector3D(float r, float s, float t)					{ x = r; y = s; z = t; }
	cVector3D(const cVector3D& v)							{ x = v.x; y = v.y; z = v.z; }
	inline cVector3D& Set(float r, float s, float t)		{ x = r; y = s; z = t; return (*this); }
	inline void Get(float *r, float *s, float *t)			{ *r = x; *s = y; *t = z; }
	inline cVector3D& operator =(const cVector3D& v)		{ x = v.x; y = v.y; z = v.z; return (*this); }
	inline cVector3D& operator +=(const cVector3D& v)		{ x += v.x; y += v.y; z += v.z; return (*this); }
	inline cVector3D& operator -=(const cVector3D& v)		{ x -= v.x; y -= v.y; z -= v.z; return (*this); }
	inline cVector3D& operator *=(float t)					{ x *= t; y *= t; z *= t; return (*this); }
	inline cVector3D& operator /=(float t)					{ float f = 1.0F / t; x *= f; y *= f; z *= f; return (*this); }
	inline cVector3D& operator %=(const cVector3D& v)
	{ 
		float r,s;
		r = y * v.z - z * v.y;
		s = z * v.x - x * v.z;
		z = x * v.y - y * v.x;
		x = r;
		y = s;
		return (*this);
	}
	inline cVector3D& operator &=(const cVector3D& v)		{ x *= v.x; y *= v.y; z *= v.z; return (*this); }
	inline cVector3D operator -(void) const					{ return (cVector3D(-x, -y, -z)); }
	inline cVector3D operator +(const cVector3D& v) const	{ return (cVector3D(x + v.x, y + v.y, z + v.z)); }
	inline cVector3D operator -(const cVector3D& v) const	{ return (cVector3D(x - v.x, y - v.y, z - v.z)); }
	inline cVector3D operator *(float t) const				{ return (cVector3D(x * t, y * t, z * t)); }
	inline cVector3D operator /(float t) const				{ float f = 1.0F / t; return (cVector3D(x * f, y * f, z * f)); }
	inline float operator *(const cVector3D& v) const		{ return (x * v.x + y * v.y + z * v.z); }
	inline cVector3D operator %(const cVector3D& v) const	{ return (cVector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x)); }
	inline cVector3D operator &(const cVector3D& v) const	{ return (cVector3D(x * v.x, y * v.y, z * v.z)); }
	inline bool operator ==(const cVector3D& v) const		{ return ((x == v.x) && (y == v.y) && (z == v.z)); }
	inline bool operator !=(const cVector3D& v) const		{ return ((x != v.x) || (y != v.y) || (z != v.z)); }
	inline cVector3D& Normalize(void)						{ return (*this /= sqrtf(x * x + y * y + z * z)); }
	cVector3D& RotateAboutX(float angle);
	cVector3D& RotateAboutY(float angle);
	cVector3D& RotateAboutZ(float angle);
	cVector3D& RotateAboutAxis(float angle, const cVector3D& axis);

	inline cVector3D& RoundUp(void)						{ x = floor(x + 0.5f); y = floor(y + 0.5f); z = floor(z + 0.5f); return (*this); }
};

inline cVector3D operator *(float t, const cVector3D& v)
{
    return (cVector3D(t * v.x, t * v.y, t * v.z));
}

inline float Dot(const cVector3D& v1, const cVector3D& v2)
{
    return (v1 * v2);
}

inline cVector3D Cross(const cVector3D& v1, const cVector3D& v2)
{
    return (v1 % v2);
}

inline float Magnitude(const cVector3D& v)
{
    return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float InverseMag(const cVector3D& v)
{
    return (1.0F / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float SquaredMag(const cVector3D& v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

// Calculates angle between 2 vectors (rad)
inline float Angle(const cVector3D& v1, const cVector3D& v2)
{
	if( Magnitude(v1 + v2) < 0.0001 )  return (float)M_PI;

	return acos( (v1*v2)/(Magnitude(v1)*Magnitude(v2)) );
}