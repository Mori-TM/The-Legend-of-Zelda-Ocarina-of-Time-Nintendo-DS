#define v10tofloat(n)   (((float)(n)) * 0.001953125f)
//#define v10tofloat(n) (n == 0x1FF ? 1.0 : (((float)(n)) / (float)(1 << 9)))
#define v10tof32(n) (v10tofloat(n) * 4096.0f)

vec3 GetNormalFromList(u32* List, u32 Index)
{
	Index++;
	Index *= 5;
	Index--;

	v10 x = (List[Index] & 0x3FF);
	v10 y = (List[Index] >> 10) & 0x3FF;
	v10 z = (List[Index] >> 20) & 0x3FF;
	if (x & 0x200) x |= 0xFC00;
	if (y & 0x200) y |= 0xFC00;
	if (z & 0x200) z |= 0xFC00;


	vec3 Norm;
	Norm.x = (s32)v10tof32(x);
	Norm.y = (s32)v10tof32(y);
	Norm.z = (s32)v10tof32(z);

	return Norm;
}

void SetNormalToList(u32* List, u32 Index, vec3 Norm)
{
	Index++;
	Index *= 5;
	Index--;

	List[Index] = NORMAL_PACK(f32tov10(Norm.x.Fixed), f32tov10(Norm.y.Fixed), f32tov10(Norm.z.Fixed));
}

#define t16tof32(n) ((float)t16toint(n) * 4096.0f)

vec2 GetTexCoordFromList(u32* List, u32 Index)
{
	Index++;
	Index *= 5;
	Index -= 2;

	t16 x = (List[Index] & 0xFFFF);
	t16 y = (List[Index] >> 16) & 0xFFFF; // & 0xFFFF opt

	vec2 TexCoord;
	TexCoord.x = (s32)t16tof32(x);
	TexCoord.y = (s32)t16tof32(y);

	return TexCoord;
}

void SetTexCoordToList(u32* List, u32 Index, vec2 TexCoord)
{
	Index++;
	Index *= 5;
	Index -= 2;

	List[Index] = TEXTURE_PACK(f32tot16(TexCoord.x.Fixed), f32tot16(TexCoord.y.Fixed));
}

vec3 GetVertexFromList(u32* List, u32 Index)
{
	Index++;
	Index *= 5;

	v16 x = (List[Index] & 0xFFFF);
	v16 y = (List[Index] >> 16) & 0xFFFF; // & 0xFFFF opt
	v16 z = (List[Index + 1] & 0xFFFF);

	vec3 Pos;
	Pos.x = (s32)x;
	Pos.y = (s32)y;
	Pos.z = (s32)z;

	return Pos;
}

void SetVertexToList(u32* List, u32 Index, vec3 Pos)
{
	Index++;
	Index *= 5;

	List[Index] = VERTEX_PACK(f32tov16(Pos.x.Fixed), f32tov16(Pos.y.Fixed));
	List[Index + 1] = VERTEX_PACK(f32tov16(Pos.z.Fixed), 0);
}