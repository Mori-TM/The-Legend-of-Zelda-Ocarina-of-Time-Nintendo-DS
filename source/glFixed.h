FORCE_INLINE void glTranslatefx(f32 x, f32 y, f32 z)
{
	glTranslatef32(x, y, z);
}

FORCE_INLINE void glScalefx(f32 x, f32 y, f32 z)
{
	glScalef32(x, y, z);
}

FORCE_INLINE void glRotatefx(f32 a, f32 x, f32 y, f32 z)
{
	glRotatef32i(a, x, y, z);
}

FORCE_INLINE void gluPerspectivefx(f32 Fov, f32 Aspect, f32 NearZ, f32 FarZ)
{ 
	gluPerspectivef32(Fov, Aspect, NearZ, FarZ);
}

FORCE_INLINE void glOrthofx(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 NearZ, f32 FarZ)
{
	glOrthof32(Left, Right, Bottom, Top, NearZ, FarZ);
}

FORCE_INLINE void gluLookAtfx(f32 Ex, f32 Ey, f32 Ez,
	f32 Lx, f32 Ly, f32 Lz,
	f32 Ux, f32 Uy, f32 Uz)
{
	gluLookAtf32(Ex, Ey, Ez,
				 Lx, Ly, Lz,
				 Ux, Uy, Uz);
}

FORCE_INLINE void gluLookAtV(vec3 Pos, vec3 LookAt, vec3 Up)
{
	gluLookAtfx(Pos[0], Pos[1], Pos[2],
				LookAt[0], LookAt[1], LookAt[2],
				Up[0], Up[1], Up[2]);
}

FORCE_INLINE void glTranslateV(vec3 Pos)
{
	glTranslatefx(Pos[0], Pos[1], Pos[2]);
}

FORCE_INLINE void glScaleV(vec3 Scale)
{
	glScalefx(Scale[0], Scale[1], Scale[2]);
}

FORCE_INLINE void glTexCoordfx(f32 x, f32 y)
{
	glTexCoord2t16(f32tot16(x), f32tot16(y));
}

//Not working!
FORCE_INLINE void glNormalfx(f32 x, f32 y, f32 z)
{
	glNormal(NORMAL_PACK(f32tov10(x), f32tov10(y), f32tov10(z)));
}

//Not working!
FORCE_INLINE void glVertexfx(f32 x, f32 y, f32 z)
{
	GFX_VERTEX16 = VERTEX_PACK((v16)x, (v16)y);
	GFX_VERTEX16 = VERTEX_PACK((v16)z, 0);
}