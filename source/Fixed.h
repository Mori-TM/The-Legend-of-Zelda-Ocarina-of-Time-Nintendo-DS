/*
class f32
{
public:
	s32 Fixed;

	FORCE_INLINE f32()
	{
		Fixed = 0;
	}

	FORCE_INLINE f32(s32 n)
	{
		Fixed = n;
	}

	FORCE_INLINE f32(float n)
	{
		Fixed = (s32)(n * 4096.0f);
	}

	FORCE_INLINE f32(double n)
	{
		Fixed = (s32)((float)n * 4096.0f);
	}

	FORCE_INLINE float Float()
	{
		return (float(Fixed) * 0.000244140625f);
	}

	FORCE_INLINE void operator+=(f32 const& n)
	{
		Fixed += n;
	}

	FORCE_INLINE void operator+=(float const& n)
	{
		Fixed += f32(n);
	}

	FORCE_INLINE void operator-=(f32 const& n)
	{
		Fixed -= n;
	}

	FORCE_INLINE void operator-=(float const& n)
	{
		Fixed -= f32(n);
	}


	FORCE_INLINE f32 operator+(f32 const& n)
	{
		return Fixed + n;
	}

	FORCE_INLINE f32 operator+(float const& n)
	{
		return Fixed + f32(n);
	}

	FORCE_INLINE f32 operator-(f32 const& n)
	{
		return Fixed - n;
	}

	FORCE_INLINE f32 operator-(float const& n)
	{
		return Fixed - f32(n);
	}

	//for the ds use hardware assisted math func like mulf32() or divf32()

#ifdef NO_HARDWARE_ACCELERATION
	FORCE_INLINE void operator*=(f32 const& n)
	{
		Fixed = (s32)((s64(Fixed) * s64(n)) >> 12);
	}

	FORCE_INLINE void operator*=(float const& n)
	{
		Fixed = (s32)((s64(Fixed) * s64(f32(n))) >> 12);
	}

	FORCE_INLINE void operator/=(f32 const& n)
	{
		Fixed = (s32)((s64(Fixed) << 12) / s64(n));
	}

	FORCE_INLINE void operator/=(float const& n)
	{
		Fixed = (s32)((s64(Fixed) << 12) / s64(f32(n)));
	}


	FORCE_INLINE f32 operator*(f32 const& n)
	{
		return (s32)((s64(Fixed) * s64(n)) >> 12);
	}

	FORCE_INLINE f32 operator*(float const& n)
	{
		return (s32)((s64(Fixed) * s64(f32(n))) >> 12);
	}

	FORCE_INLINE f32 operator/(f32 const& n)
	{
		return (s32)((s64(Fixed) << 12) / s64(n));
	}

	FORCE_INLINE f32 operator/(float const& n)
	{
		return (s32)((s64(Fixed) << 12) / s64(f32(n)));
	}
#else
	FORCE_INLINE void operator*=(f32 const& n)
	{
		Fixed = mulf32(Fixed, n);
	}

	FORCE_INLINE void operator*=(float const& n)
	{
		Fixed = mulf32(Fixed, f32(n));
	}

	FORCE_INLINE void operator/=(f32 const& n)
	{
		Fixed = divf32(Fixed, n);
	}

	FORCE_INLINE void operator/=(float const& n)
	{
		Fixed = divf32(Fixed, f32(n));
	}


	FORCE_INLINE f32 operator*(f32 const& n)
	{
		return mulf32(Fixed, n);
	}

	FORCE_INLINE f32 operator*(float const& n)
	{
		return mulf32(Fixed, f32(n));
	}

	FORCE_INLINE f32 operator/(f32 const& n)
	{
		return divf32(Fixed, n);
	}

	FORCE_INLINE f32 operator/(float const& n)
	{
		return divf32(Fixed, f32(n));
	}
#endif	

	FORCE_INLINE f32 operator-()
	{
		return -Fixed;
	}

	FORCE_INLINE bool operator<(f32 const& n)
	{
		return Fixed < n;
	}

	FORCE_INLINE bool operator<=(f32 const& n)
	{
		return Fixed <= n;
	}

	FORCE_INLINE bool operator>(f32 const& n)
	{
		return Fixed > n;
	}

	FORCE_INLINE bool operator>=(f32 const& n)
	{
		return Fixed >= n;
	}

	FORCE_INLINE bool operator==(f32 const& n)
	{
		return Fixed == n;
	}

	FORCE_INLINE bool operator!=(f32 const& n)
	{
		return Fixed != n;
	}
};
*/

typedef s32 f32;

FORCE_INLINE f32 ToF32(float n)
{
	return (f32)(n * 4096.0f);
}

FORCE_INLINE float ToFloat(f32 a)
{
	return ((float)a * 0.000244140625f);
}

FORCE_INLINE f32 Add(f32 a, f32 b)
{
	return a + b;
}

FORCE_INLINE f32 Sub(f32 a, f32 b)
{
	return a - b;
}

FORCE_INLINE f32 Mul(f32 a, f32 b)
{
	/*
	register f32 r;
	asm volatile ("mov %1, %0\n\t"
    "add $1, %0"
    : "=r" (dst) 
    : "r" (src));
	return r;
	*/
	return mulf32(a, b);
}

FORCE_INLINE f32 Div(f32 a, f32 b)
{
	return divf32(a, b);
}

FORCE_INLINE f32 Squ(f32 a)
{
	return mulf32(a, a);
}