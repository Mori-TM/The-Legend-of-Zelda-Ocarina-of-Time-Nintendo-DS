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
		Fixed += n.Fixed;
	}

	FORCE_INLINE void operator+=(float const& n)
	{
		Fixed += f32(n).Fixed;
	}

	FORCE_INLINE void operator-=(f32 const& n)
	{
		Fixed -= n.Fixed;
	}

	FORCE_INLINE void operator-=(float const& n)
	{
		Fixed -= f32(n).Fixed;
	}


	FORCE_INLINE f32 operator+(f32 const& n)
	{
		return Fixed + n.Fixed;
	}

	FORCE_INLINE f32 operator+(float const& n)
	{
		return Fixed + f32(n).Fixed;
	}

	FORCE_INLINE f32 operator-(f32 const& n)
	{
		return Fixed - n.Fixed;
	}

	FORCE_INLINE f32 operator-(float const& n)
	{
		return Fixed - f32(n).Fixed;
	}

	//for the ds use hardware assisted math func like mulf32() or divf32()
	FORCE_INLINE void operator*=(f32 const& n)
	{
		Fixed = mulf32(Fixed, n.Fixed);
	//	Fixed = (s32)((s64(Fixed) * s64(n.Fixed)) >> 12);
	}

	FORCE_INLINE void operator*=(float const& n)
	{
		Fixed = mulf32(Fixed, f32(n).Fixed);
	//	Fixed = (s32)((s64(Fixed) * s64(f32(n).Fixed)) >> 12);
	}

	FORCE_INLINE void operator/=(f32 const& n)
	{
		Fixed = divf32(Fixed, n.Fixed);
	//	Fixed = (s32)((s64(Fixed) << 12) / s64(n.Fixed));
	}

	FORCE_INLINE void operator/=(float const& n)
	{
		Fixed = divf32(Fixed, f32(n).Fixed);
	//	Fixed = (s32)((s64(Fixed) << 12) / s64(f32(n).Fixed));
	}


	FORCE_INLINE f32 operator*(f32 const& n)
	{
		return mulf32(Fixed, n.Fixed);
	//	return (s32)((s64(Fixed) * s64(n.Fixed)) >> 12);
	}

	FORCE_INLINE f32 operator*(float const& n)
	{
		return mulf32(Fixed, f32(n).Fixed);
	//	return (s32)((s64(Fixed) * s64(f32(n).Fixed)) >> 12);
	}

	FORCE_INLINE f32 operator/(f32 const& n)
	{
		return divf32(Fixed, n.Fixed);
	//	return (s32)((s64(Fixed) << 12) / s64(n.Fixed));
	}

	FORCE_INLINE f32 operator/(float const& n)
	{
		return divf32(Fixed, f32(n).Fixed);
	//	return (s32)((s64(Fixed) << 12) / s64(f32(n).Fixed));
	}

	FORCE_INLINE f32 operator-()
	{
		return -Fixed;
	}

	FORCE_INLINE bool operator<(f32 const& n)
	{
		return Fixed < n.Fixed;
	}

	FORCE_INLINE bool operator<=(f32 const& n)
	{
		return Fixed <= n.Fixed;
	}

	FORCE_INLINE bool operator>(f32 const& n)
	{
		return Fixed > n.Fixed;
	}

	FORCE_INLINE bool operator>=(f32 const& n)
	{
		return Fixed >= n.Fixed;
	}

	FORCE_INLINE bool operator==(f32 const& n)
	{
		return Fixed == n.Fixed;
	}

	FORCE_INLINE bool operator!=(f32 const& n)
	{
		return Fixed != n.Fixed;
	}
};