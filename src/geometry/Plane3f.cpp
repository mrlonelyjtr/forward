//--------------------------------------------------------------------------------
// This file is a portion of the Hieroglyph 3 Rendering Engine.  It is distributed
// under the MIT License, available in the root of this distribution and 
// at the following URL:
//
// http://www.opensource.org/licenses/mit-license.php
//
// Copyright (c) Jason Zink 
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
#include "Plane3f.h"
#include <math.h>
//--------------------------------------------------------------------------------
using namespace forward;
//--------------------------------------------------------------------------------
Plane3f::Plane3f()
{
	m_fComponents[0] = 0.0f;
	m_fComponents[1] = 0.0f;
	m_fComponents[2] = 0.0f;
	m_fComponents[3] = 0.0f;
}
//--------------------------------------------------------------------------------
Plane3f::Plane3f(f32 a, f32 b, f32 c, f32 d)
{
	m_fComponents[0] = a;
	m_fComponents[1] = b;
	m_fComponents[2] = c;
	m_fComponents[3] = d;
}
//--------------------------------------------------------------------------------
Plane3f::Plane3f(const Vector3f& normal, const Vector3f& v)
{
	auto d = -normal.Dot(v);
	m_fComponents[0] = normal.x;
	m_fComponents[1] = normal.y;
	m_fComponents[2] = normal.z;
	m_fComponents[3] = d;
}
//--------------------------------------------------------------------------------
Plane3f::~Plane3f()
{
}
//--------------------------------------------------------------------------------
void Plane3f::Normalize()
{
	f32 fMagnitude = static_cast<f32>(sqrt(a()*a() + b()*b() + c()*c()));

	for (i32 i = 0; i < 4; i++)
	{
        m_fComponents[i] /= fMagnitude;
	}
}
//--------------------------------------------------------------------------------
f32 Plane3f::DistanceToPoint( const Vector3f& pt ) const
{
	return (a() * pt.x +
			b() * pt.y +
			c() * pt.z +
			d());
}
//--------------------------------------------------------------------------------
f32 Plane3f::a() const
{
	return(m_fComponents[0]);
}
//--------------------------------------------------------------------------------
f32& Plane3f::a()
{
	return(m_fComponents[0]);
}
//--------------------------------------------------------------------------------
f32 Plane3f::b() const
{
	return(m_fComponents[1]);
}
//--------------------------------------------------------------------------------
f32& Plane3f::b()
{
	return(m_fComponents[1]);
}
//--------------------------------------------------------------------------------
f32 Plane3f::c() const
{
	return(m_fComponents[2]);
}
//--------------------------------------------------------------------------------
f32& Plane3f::c()
{
	return(m_fComponents[2]);
}
//--------------------------------------------------------------------------------
f32 Plane3f::d() const
{
	return(m_fComponents[3]);
}
//--------------------------------------------------------------------------------
f32& Plane3f::d()
{
	return(m_fComponents[3]);
}
//--------------------------------------------------------------------------------
eSHAPE Plane3f::GetShapeType( ) const
{
	return( PLANE );
}
//--------------------------------------------------------------------------------