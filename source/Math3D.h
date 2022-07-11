/*
FORCE_INLINE float SinF(float angle)
{
	int32 s = sinLerp((short)(angle * DEGREES_IN_CIRCLE / 360));

	return f32tofloat(s);
}

FORCE_INLINE float CosF(float angle)
{
	int32 c = cosLerp((short)(angle * DEGREES_IN_CIRCLE / 360));

	return f32tofloat(c);
}

FORCE_INLINE float SqrtfF(float val)
{
	return f32tofloat(sqrtf32(floattof32(val)));
}

FORCE_INLINE float MulF(float a, float b)
{
	return f32tofloat(mulf32(floattof32(a), floattof32(b)));
}

FORCE_INLINE float DivF(float a, float b)
{
	return f32tofloat(divf32(floattof32(a), floattof32(b)));
}
*/
FORCE_INLINE f32 sqrtf32(f32 Val)
{
	return sqrtf32(Val.Fixed);
}

FORCE_INLINE f32 sinf32(f32 Angle)
{
	return (s32)sinLerp((short)(Angle.Float() * DEGREES_IN_CIRCLE / 360));
}

FORCE_INLINE f32 cosf32(f32 Angle)
{
	return (s32)cosLerp((short)(Angle.Float() * DEGREES_IN_CIRCLE / 360));
}

//vector
typedef struct
{
	f32 x;
	f32 y;
} vec2;

typedef struct
{
	f32 x;
	f32 y;
	f32 z;
} vec3;

typedef struct
{
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} vec4;

f32 FastInverseSqrt(f32 number)
{
	long i;
	float x2;
	float y;
	const float threehalfs = 1.5f;
	const float Num = number.Float();

	x2 = Num * 0.5f;
	y = Num;
	i = *(long*)&y;              // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);   // what the fuck? 
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed - it's for more accuracy

	return y;
}

vec2 Normalize2(vec2 v)
{
	register f32 length = FastInverseSqrt(v.x * v.x + v.y * v.y);
	v.x *= length;
	v.y *= length;
	return v;
}

vec3 Normalize3(vec3 v)
{
	register f32 length = FastInverseSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x *= length;
	v.y *= length;
	v.z *= length;
	return v;
}

vec4 Normalize4(vec4 v)
{
	register f32 length = FastInverseSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	v.x *= length;
	v.y *= length;
	v.z *= length;
	v.w *= length;
	return v;
}

void Normalize2P(vec2* v)
{
	register f32 length = FastInverseSqrt(v->x * v->x + v->y * v->y);
	v->x *= length;
	v->y *= length;
}

void Normalize3P(vec3* v)
{
	register f32 length = FastInverseSqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x *= length;
	v->y *= length;
	v->z *= length;
}

void Normalize4P(vec4* v)
{
	register f32 length = FastInverseSqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
	v->x *= length;
	v->y *= length;
	v->z *= length;
	v->w *= length;
}

FORCE_INLINE vec3 Cross3P(vec3* a, vec3* b)
{
	vec3 r;
	r.x = a->y * b->z - a->z * b->y;
	r.y = a->z * b->x - a->x * b->z;
	r.z = a->x * b->y - a->y * b->x;
	return r;
}

FORCE_INLINE f32 Dot2P(vec2* a, vec2* b)
{
	return a->x * b->x + a->y * b->y;
}

FORCE_INLINE f32 Dot3P(vec3* a, vec3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

FORCE_INLINE vec2 Add2P(vec2* a, vec2* b)
{
	vec2 r;
	r.x = a->x + b->x;
	r.y = a->y + b->y;
	return r;
}

FORCE_INLINE vec2 Sub2P(vec2* a, vec2* b)
{
	vec2 r;
	r.x = a->x - b->x;
	r.y = a->y - b->y;
	return r;
}

FORCE_INLINE vec2 Mul2P(vec2* a, vec2* b)
{
	vec2 r;
	r.x = a->x * b->x;
	r.y = a->y * b->y;
	return r;
}

FORCE_INLINE vec2 Div2P(vec2* a, vec2* b)
{
	vec2 r;
	r.x = a->x / b->x;
	r.y = a->y / b->y;
	return r;
}

FORCE_INLINE vec3 Add3P(vec3* a, vec3* b)
{
	vec3 r;
	r.x = a->x + b->x;
	r.y = a->y + b->y;
	r.z = a->z + b->z;
	return r;
}

FORCE_INLINE vec3 Sub3P(vec3* a, vec3* b)
{
	vec3 r;
	r.x = a->x - b->x;
	r.y = a->y - b->y;
	r.z = a->z - b->z;
	return r;
}

FORCE_INLINE vec3 Mul3P(vec3* a, vec3* b)
{
	vec3 r;
	r.x = a->x * b->x;
	r.y = a->y * b->y;
	r.z = a->z * b->z;
	return r;
}

FORCE_INLINE vec3 Div3P(vec3* a, vec3* b)
{
	vec3 r;
	r.x = a->x / b->x;
	r.y = a->y / b->y;
	r.z = a->z / b->z;
	return r;
}

FORCE_INLINE vec4 Add4P(vec4* a, vec4* b)
{
	vec4 r;
	r.x = a->x + b->x;
	r.y = a->y + b->y;
	r.z = a->z + b->z;
	r.w = a->w + b->w;
	return r;
}

FORCE_INLINE vec4 Sub4P(vec4* a, vec4* b)
{
	vec4 r;
	r.x = a->x - b->x;
	r.y = a->y - b->y;
	r.z = a->z - b->z;
	r.w = a->w - b->w;
	return r;
}

FORCE_INLINE vec4 Mul4P(vec4* a, vec4* b)
{
	vec4 r;
	r.x = a->x * b->x;
	r.y = a->y * b->y;
	r.z = a->z * b->z;
	r.w = a->w * b->w;
	return r;
}

FORCE_INLINE vec4 Div4P(vec4* a, vec4* b)
{
	vec4 r;
	r.x = a->x / b->x;
	r.y = a->y / b->y;
	r.z = a->z / b->z;
	r.w = a->w / b->w;
	return r;
}


FORCE_INLINE vec3 Cross3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;
	return r;
}

FORCE_INLINE f32 Dot2(vec2 a, vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

FORCE_INLINE f32 Dot3(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

FORCE_INLINE vec2 Add2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
}

FORCE_INLINE vec2 Sub2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	return r;
}

FORCE_INLINE vec2 Mul2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	return r;
}

FORCE_INLINE vec2 Div2(vec2 a, vec2 b)
{
	vec2 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	return r;
}

FORCE_INLINE vec3 Add3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	return r;
}

FORCE_INLINE vec3 Sub3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	return r;
}

FORCE_INLINE vec3 Mul3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	return r;
}

FORCE_INLINE vec3 Div3(vec3 a, vec3 b)
{
	vec3 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;
	return r;
}

FORCE_INLINE vec4 Add4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	r.w = a.w + b.w;
	return r;
}

FORCE_INLINE vec4 Sub4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	r.w = a.w - b.w;
	return r;
}

FORCE_INLINE vec4 Mul4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	r.w = a.w * b.w;
	return r;
}

FORCE_INLINE vec4 Div4(vec4 a, vec4 b)
{
	vec4 r;
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;
	r.w = a.w / b.w;
	return r;
}

FORCE_INLINE f32 Length3(vec3 v)
{
	return sqrtf32(v.x * v.x + v.y * v.y + v.z * v.z);
}

FORCE_INLINE f32 Length3P(vec3* v)
{
	return sqrtf32((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

FORCE_INLINE vec2 Reflect2(vec2* v, vec2* n)
{
	vec2 r;
	r.x = v->x - Dot2P(v, n) * n->x * 2.0f;
	r.y = v->y - Dot2P(v, n) * n->y * 2.0f;
	return r;
}

FORCE_INLINE vec3 Reflect3(vec3* v, vec3* n)
{
	vec3 r;
	r.x = v->x - Dot3P(v, n) * n->x * 2.0f;
	r.y = v->y - Dot3P(v, n) * n->y * 2.0f;
	r.z = v->z - Dot3P(v, n) * n->z * 2.0f;
	return r;
}

f32 GetDistanceVec2(vec2* a, vec2* b)
{
	register f32 DX = a->x - b->x;
	register f32 DY = a->y - b->y;

	DX *= DX;
	DY *= DY;

	return sqrtf32(DX + DY);
}

f32 GetDistanceVec3(vec3 a, vec3 b)
{
	register f32 DX = a.x - b.x;
	register f32 DY = a.y - b.y;
	register f32 DZ = a.z - b.z;

	DX *= DX;
	DY *= DY;
	DZ *= DZ;

	return sqrtf32(DX + DY + DZ);
}

f32 GetDistanceVec3P(vec3* a, vec3* b)
{
	register f32 DX = a->x - b->x;
	register f32 DY = a->y - b->y;
	register f32 DZ = a->z - b->z;

	DX *= DX;
	DY *= DY;
	DZ *= DZ;

	return sqrtf32(DX + DY + DZ);
}

FORCE_INLINE vec2 Vec2(f32 x, f32 y)
{
	vec2 r = { x, y };
	return r;
}

FORCE_INLINE vec3 Vec3(f32 x, f32 y, f32 z)
{
	vec3 r = { x, y, z };
	return r;
}

FORCE_INLINE vec4 Vec4(f32 x, f32 y, f32 z, f32 w)
{
	vec4 r = { x, y, z, w };
	return r;
}

FORCE_INLINE vec2 Vec2f(f32 x)
{
	vec2 r = { x, x };
	return r;
}

FORCE_INLINE vec3 Vec3f(f32 x)
{
	vec3 r = { x, x, x };
	return r;
}

FORCE_INLINE vec4 Vec4f(f32 x)
{
	vec4 r = { x, x, x, x };
	return r;
}

//helper
FORCE_INLINE f32 ToRadians(f32 Angle)
{
	return Angle * 0.01745329251f;
}