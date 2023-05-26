/*
* TO-Do
* (dont) use hardware normalize functions (its slower)
* use hardware cross functions
*/
float fabsff(float x)
{
	if (x < 0.0)
		return -x;

	return x;
}

FORCE_INLINE f32 fabsf32(f32 x)
{
	if (x < 0)
		return -x;

	return x;
}

#define DEGREES_IN_CIRCLEF32 (s32)134217728
#define	FULL_360F32 (s32)1474560
#define DEG_IN_CIR_DIVF32 (s32)372827

FORCE_INLINE f32 sinf32(f32 Ane)
{
	return sinLerp((short)(Div(Mul(Ane, DEGREES_IN_CIRCLEF32), FULL_360F32)));
//	return (s32)sinLer((short)(Ane * f32((float)DEGREES_IN_CIRCLE) / f32(360.0f)));
//	return (s32)sinLer((short)(Ane.Float() * DEGREES_IN_CIRCLE / 360));
}

FORCE_INLINE f32 cosf32(f32 Ane)
{
	return cosLerp((short)(Div(Mul(Ane, DEGREES_IN_CIRCLEF32), FULL_360F32)));
//	return (s32)cosLer((short)(Ane * f32((float)DEGREES_IN_CIRCLE) / f32(360.0f)));
//	return (s32)cosLer((short)(Ane.Float() * DEGREES_IN_CIRCLE / 360));
}

FORCE_INLINE f32 asinf32(f32 Ane)
{
	return asinLerp((short)(Div(Mul(Ane, DEGREES_IN_CIRCLEF32), FULL_360F32)));
}

FORCE_INLINE f32 acosf32(f32 Ane)
{
	
	return Mul(acosLerp((short)Ane), DEGREES_IN_CIRCLEF32);
}

#define TO_DEGREES 234684

FORCE_INLINE f32 atan2f32(f32 x, f32 y)
{
	f32 v = Div(asinf32(x), acosf32(y));
	return Mul(v, TO_DEGREES);
}

/*
f32 atan2f32( float y, float x )
{
    static const uint32_t sign_mask = 0x80000000;
    static const float b = 0.596227f;

    // Extract the sign bits
    uint32_t ux_s  = sign_mask & (uint32_t &)x;
    uint32_t uy_s  = sign_mask & (uint32_t &)y;

    // Determine the quadrant offset
    float q = (float)( ( ~ux_s & uy_s ) >> 29 | ux_s >> 30 ); 

    // Calculate the arctangent in the first quadrant
    float bxy_a = fabsff( b * x * y );
    float num = bxy_a + y * y;
    float atan_1q =  num / ( x * x + bxy_a + num );

    // Translate it to the proper quadrant
    uint32_t uatan_2q = (ux_s ^ uy_s) | (uint32_t &)atan_1q;
    return q + (float &)uatan_2q;
} 
*/

typedef f32 vec2[2];
typedef f32 vec3[3];
typedef f32 vec4[4];

f32 FastInverseSqrt(f32 number)
{
	long i;
	float x2;
	float y;
	const float threehalfs = 1.5f;
	const float Num = ToFloat(number);

	x2 = Num * 0.5f;
	y = Num;
	i = *(long*)&y;              // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);   // what the fuck? 
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed - it's for more accuracy
//	y = y * (threehalfs - (x2 * y * y));

	return ToF32(y);
}

void Normalize2(vec2 v)
{
	register f32 length = FastInverseSqrt(Mul(v[0], v[0]) + Mul(v[1], v[1]));
	v[0] = Mul(v[0], length);
	v[1] = Mul(v[1], length);
}

void Normalize3(vec3 v)
{
	
	register f32 length = Div(4096, sqrtf32(Mul(v[0], v[0]) + Mul(v[1], v[1]) + Mul(v[2], v[2])));
	v[0] = Mul(v[0], length);
	v[1] = Mul(v[1], length);
	v[2] = Mul(v[2], length);
	
	/*
	register f32 length = FastInverseSqrt(Mul(v[0], v[0]) + Mul(v[1], v[1]) + Mul(v[2], v[2]));
	v[0] = Mul(v[0], length);
	v[1] = Mul(v[1], length);
	v[2] = Mul(v[2], length);
	*/
}

void Normalize4(vec4 v)
{
	register f32 length = FastInverseSqrt(Mul(v[0], v[0]) + Mul(v[1], v[1]) + Mul(v[2], v[2]) + Mul(v[3], v[3]));
	v[0] = Mul(v[0], length);
	v[1] = Mul(v[1], length);
	v[2] = Mul(v[2], length);
	v[3] = Mul(v[3], length);
}

FORCE_INLINE f32* Cross3(f32* restrict a, f32* restrict b, f32* restrict r)
{
	r[0] = Mul(a[1], b[2]) - Mul(a[2], b[1]);
	r[1] = Mul(a[2], b[0]) - Mul(a[0], b[2]);
	r[2] = Mul(a[0], b[1]) - Mul(a[1], b[0]);
	return r;
}

FORCE_INLINE f32 Dot2(vec2 a, vec2 b)
{
	return Mul(a[0], b[0]) + Mul(a[1], b[1]);
}

FORCE_INLINE f32 Dot3(vec3 a, vec3 b)
{
	return Mul(a[0], b[0]) + Mul(a[1], b[1]) + Mul(a[2], b[2]);
}

FORCE_INLINE f32* Add2(vec2 a, vec2 b, vec2 r)
{
	r[0] = a[0] + b[0];
	r[1] = a[1] + b[1];
	return r;
}

FORCE_INLINE f32* Sub2(vec2 a, vec2 b, vec2 r)
{
	r[0] = a[0] - b[0];
	r[1] = a[1] - b[1];
	return r;
}

FORCE_INLINE f32* Mul2(vec2 a, vec2 b, vec2 r)
{
	r[0] = Mul(a[0], b[0]);
	r[1] = Mul(a[1], b[1]);
	return r;
}

FORCE_INLINE f32* Div2(vec2 a, vec2 b, vec2 r)
{
	r[0] = Div(a[0], b[0]);
	r[1] = Div(a[1], b[1]);
	return r;
}

f32* Add3(vec3 a, vec3 b, vec3 r)
{
	r[0] = a[0] + b[0];
	r[1] = a[1] + b[1];
	r[2] = a[2] + b[2];
	return r;
}

f32* Sub3(vec3 a, vec3 b, vec3 r)
{
	r[0] = a[0] - b[0];
	r[1] = a[1] - b[1];
	r[2] = a[2] - b[2];
	return r;
}

f32* Mul3(vec3 a, vec3 b, vec3 r)
{
	r[0] = Mul(a[0], b[0]);
	r[1] = Mul(a[1], b[1]);
	r[2] = Mul(a[2], b[2]);
	return r;
}

f32* Div3(vec3 a, vec3 b, vec3 r)
{
	r[0] = Div(a[0], b[0]);
	r[1] = Div(a[1], b[1]);
	r[2] = Div(a[2], b[2]);
	return r;
}

FORCE_INLINE f32* Add4(vec4 a, vec4 b, vec4 r)
{
	r[0] = a[0] + b[0];
	r[1] = a[1] + b[1];
	r[2] = a[2] + b[2];
	r[3] = a[3] + b[3];
	return r;
}

FORCE_INLINE f32* Sub4(vec4 a, vec4 b, vec4 r)
{
	r[0] = a[0] - b[0];
	r[1] = a[1] - b[1];
	r[2] = a[2] - b[2];
	r[3] = a[3] - b[3];
	return r;
}

FORCE_INLINE f32* Mul4(vec4 a, vec4 b, vec4 r)
{
	r[0] = Mul(a[0], b[0]);
	r[1] = Mul(a[1], b[1]);
	r[2] = Mul(a[2], b[2]);
	r[3] = Mul(a[3], b[3]);
	return r;
}

FORCE_INLINE f32* Div4(vec4 a, vec4 b, vec4 r)
{
	r[0] = Div(a[0], b[0]);
	r[1] = Div(a[1], b[1]);
	r[2] = Div(a[2], b[2]);
	r[3] = Div(a[3], b[3]);
	return r;
}

FORCE_INLINE f32 Length3(vec3 v)
{
	return sqrtf32(Squ(v[0]) + Squ(v[1]) + Squ(v[2]));
}

FORCE_INLINE f32* Reflect2(vec2 v, vec2 n, vec2 r)
{
	r[0] = v[0] - Mul(Mul(Dot2(v, n), n[0]), 8192);
	r[1] = v[1] - Mul(Mul(Dot2(v, n), n[1]), 8192);
	return r;
}

FORCE_INLINE f32* Reflect3(vec3 v, vec3 n, vec3 r)
{
	r[0] = v[0] - Mul(Mul(Dot3(v, n), n[0]), 8192);
	r[1] = v[1] - Mul(Mul(Dot3(v, n), n[1]), 8192);
	r[2] = v[2] - Mul(Mul(Dot3(v, n), n[2]), 8192);
	return r;
}

f32 GetDistanceVec2(vec2 a, vec2 b)
{
	register f32 DX = a[0] - b[0];
	register f32 DY = a[1] - b[1];

	DX = Squ(DX);
	DY = Squ(DY);

	return sqrtf32(DX + DY);
}

f32 GetDistanceVec3(vec3 a, vec3 b)
{
	register f32 DX = a[0] - b[0];
	register f32 DY = a[1] - b[1];
	register f32 DZ = a[2] - b[2];

	DX = Squ(DX);
	DY = Squ(DY);
	DZ = Squ(DZ);

	return sqrtf32(DX + DY + DZ);
}

FORCE_INLINE f32* Copy2(vec2 a, vec2 r)
{
	register f32 x = a[0];
	register f32 y = a[1];
	r[0] = x;
	r[1] = y;
	return r;
}

FORCE_INLINE f32* Copy3(vec3 a, vec3 r)
{
	register f32 x = a[0];
	register f32 y = a[1];
	register f32 z = a[2];
	r[0] = x;
	r[1] = y;
	r[2] = z;

	return r;
}

FORCE_INLINE f32* Copy4(vec4 a, vec4 r)
{
	register f32 x = a[0];
	register f32 y = a[1];
	register f32 z = a[2];
	register f32 w = a[3];
	r[0] = x;
	r[1] = y;
	r[2] = z;
	r[3] = w;
	return r;
}

FORCE_INLINE f32* Vec2(f32 x, f32 y, vec2 r)
{
	r[0] = x;
	r[1] = y;
	return r;
}

FORCE_INLINE f32* Vec3(f32 x, f32 y, f32 z, vec3 r)
{
	r[0] = x;
	r[1] = y;
	r[2] = z;
	return r;
}

FORCE_INLINE f32* Vec4(f32 x, f32 y, f32 z, f32 w, vec4 r)
{
	r[0] = x;
	r[1] = y;
	r[2] = z;
	r[3] = w;
	return r;
}

FORCE_INLINE f32* Vec2f(f32 x, vec2 r)
{
	register f32 a = x;
	r[0] = a;
	r[1] = a;
	return r;
}

FORCE_INLINE f32* Vec3f(f32 x, vec3 r)
{
	register f32 a = x;
	r[0] = a;
	r[1] = a;
	r[2] = a;
	return r;
}

FORCE_INLINE f32* Vec4f(f32 x, vec4 r)
{
	register f32 a = x;
	r[0] = a;
	r[1] = a;
	r[2] = a;
	r[3] = a;
	return r;
}

const f32 Radians = 0.01745329251f;
//helper
FORCE_INLINE f32 ToRadians(f32 Ane)
{
	return Mul(Ane, Radians);
}