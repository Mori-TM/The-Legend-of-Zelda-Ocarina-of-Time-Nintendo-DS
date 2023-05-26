#define v10tofloat(n)   (((float)(n)) * 0.001953125f)
//#define v10tofloat(n) (n == 0x1FF ? 1.0 : (((float)(n)) / (float)(1 << 9)))
#define v10tof32(n) (v10tofloat(n) * 4096.0f)

f32* GetNormalFromList(u32* List, u32 Index, vec3 Norm)
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

	Norm[0] = (s32)v10tof32(x);
	Norm[1] = (s32)v10tof32(y);
	Norm[2] = (s32)v10tof32(z);
	return Norm;
}

void SetNormalToList(u32* List, u32 Index, vec3 Norm)
{
	Index++;
	Index *= 5;
	Index--;

	List[Index] = NORMAL_PACK(f32tov10(Norm[0]), f32tov10(Norm[1]), f32tov10(Norm[2]));
}

#define t16tof32(n) ((float)t16toint(n) * 4096.0f)

f32* GetTexCoordFromList(u32* List, u32 Index, vec2 TexCoord)
{
	Index++;
	Index *= 5;
	Index -= 2;

	t16 x = (List[Index] & 0xFFFF);
	t16 y = (List[Index] >> 16) & 0xFFFF; // & 0xFFFF opt

	//I guess is a cheaper alternitive, but inecurate
	TexCoord[0] = t16toint(x) * 4096;
	TexCoord[1] = t16toint(y) * 4096;
//	TexCoord[0] = (s32)t16tof32(x);
//	TexCoord[1] = (s32)t16tof32(y);
	return TexCoord;
}

void SetTexCoordToList(u32* List, u32 Index, vec2 TexCoord)
{
	Index++;
	Index *= 5;
	Index -= 2;

	List[Index] = TEXTURE_PACK(f32tot16(TexCoord[0]), f32tot16(TexCoord[1]));
//	List+=Index;

//	u32 Src = TEXTURE_PACK(f32tot16(TexCoord[0]), f32tot16(TexCoord[1]));
//	DC_FlushRange(&List[Index], sizeof(Src));
//	dmaCopy(&Src, &List[Index], sizeof(Src));
}

f32* GetVertexFromList(u32* List, u32 Index, vec3 Pos)
{
	Index++;
	Index *= 5;

	register v16 x = (List[Index] & 0xFFFF);
	register v16 y = (List[Index] >> 16) & 0xFFFF; // & 0xFFFF opt
	register v16 z = (List[Index + 1] & 0xFFFF);

	Pos[0] = (s32)x;
	Pos[1] = (s32)y;
	Pos[2] = (s32)z;

	return Pos;
}

void SetVertexToList(u32* List, u32 Index, vec3 Pos)
{
	Index++;
	Index *= 5;

	List[Index] = VERTEX_PACK(f32tov16(Pos[0]), f32tov16(Pos[1]));
	List[Index + 1] = VERTEX_PACK(f32tov16(Pos[2]), 0);
}