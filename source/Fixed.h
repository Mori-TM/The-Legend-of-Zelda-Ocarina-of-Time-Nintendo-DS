#define F0		0
#define F1		4096
#define F2		8192
#define F3		12288
#define F4		16384
#define F5		20480
#define F6		24576
#define F8		32768
#define F9		36864
#define F10		40960
#define F20		81920
#define F31		126976
#define F50		204800
#define F90		368640
#define F100	409600
#define F191	782336
#define F200	819200
#define F255	1044480

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