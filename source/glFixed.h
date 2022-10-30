FORCE_INLINE void glTranslatefx(f32 x, f32 y, f32 z)
{
	glTranslatef32(x.Fixed, y.Fixed, z.Fixed);
}

FORCE_INLINE void glScalefx(f32 x, f32 y, f32 z)
{
	glScalef32(x.Fixed, y.Fixed, z.Fixed);
}

FORCE_INLINE void glRotatefx(f32 a, f32 x, f32 y, f32 z)
{
	glRotatef32i(a.Fixed, x.Fixed, y.Fixed, z.Fixed);
}

FORCE_INLINE void gluPerspectivefx(f32 Fov, f32 Aspect, f32 NearZ, f32 FarZ)
{ 
	gluPerspectivef32(Fov.Fixed, Aspect.Fixed, NearZ.Fixed, FarZ.Fixed);
}

FORCE_INLINE void glOrthofx(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 NearZ, f32 FarZ)
{
	glOrthof32(Left.Fixed, Right.Fixed, Bottom.Fixed, Top.Fixed, NearZ.Fixed, FarZ.Fixed);
}

FORCE_INLINE void gluLookAtfx(f32 Ex, f32 Ey, f32 Ez,
	f32 Lx, f32 Ly, f32 Lz,
	f32 Ux, f32 Uy, f32 Uz)
{
	gluLookAtf32(Ex.Fixed, Ey.Fixed, Ez.Fixed,
				 Lx.Fixed, Ly.Fixed, Lz.Fixed,
				 Ux.Fixed, Uy.Fixed, Uz.Fixed);
}

FORCE_INLINE void gluLookAtV(vec3* Pos, vec3* LookAt, vec3* Up)
{
	gluLookAtfx(Pos->x, Pos->y, Pos->z,
				LookAt->x, LookAt->y, LookAt->z,
				Up->x, Up->y, Up->z);
}

FORCE_INLINE void glTranslateV(vec3* Pos)
{
	glTranslatefx(Pos->x, Pos->y, Pos->z);
}

FORCE_INLINE void glScaleV(vec3* Scale)
{
	glScalefx(Scale->x, Scale->y, Scale->z);
}

FORCE_INLINE void glTexCoordfx(f32 x, f32 y)
{
	glTexCoord2t16(f32tot16(x.Fixed), f32tot16(y.Fixed));
}

//Not working!
FORCE_INLINE void glNormalfx(f32 x, f32 y, f32 z)
{
	glNormal(NORMAL_PACK(f32tov10(x.Fixed), f32tov10(y.Fixed), f32tov10(z.Fixed)));
}

//Not working!
FORCE_INLINE void glVertexfx(f32 x, f32 y, f32 z)
{
	GFX_VERTEX16 = VERTEX_PACK((v16)x.Fixed, (v16)y.Fixed);
	GFX_VERTEX16 = VERTEX_PACK((v16)z.Fixed, 0);
}
