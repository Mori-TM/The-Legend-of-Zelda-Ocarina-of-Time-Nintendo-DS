typedef int32_t f32;

u32 KeysDown;
u32 KeysHeld;
touchPosition Touch;

FORCE_INLINE void PenDelta(int *dx, int *dy) 
{
	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };

	if (KeysHeld & KEY_TOUCH) 
	{
		touchRead(&Touch);
		if (prev_pen[0] != 0x7FFFFFFF) 
		{
			*dx = (prev_pen[0] - Touch.rawx);
			*dy = (prev_pen[1] - Touch.rawy);
		} 
		else 
		{
			*dx = *dy = 0;
		}

		prev_pen[0] = Touch.rawx;
		prev_pen[1] = Touch.rawy;
	} 
	else 
	{
		prev_pen[0] = prev_pen[1] = 0x7FFFFFFF;
		*dx = *dy = 0;
	}
}

FORCE_INLINE void gluLookAtV(vec3* Pos, vec3* LookAt, vec3* Up)
{
	gluLookAt(Pos->x, Pos->y, Pos->z,
			  LookAt->x, LookAt->y, LookAt->z,
			  Up->x, Up->y, Up->z);
}

FORCE_INLINE void glTranslateV(vec3* Pos)
{
	glTranslatef(Pos->x, Pos->y, Pos->z);
}

FORCE_INLINE void glScaleV(vec3* Scale)
{
	glTranslatef(Scale->x, Scale->y, Scale->z);
}

int LoadTexture(int Size, u8* Image)
{
	int Texture;
	
	sImage pcx;
	loadPCX((u8*)Image, &pcx);
	image8to16(&pcx);
	
	glGenTextures(1, &Texture);
	glBindTexture(0, Texture);

	glTexImage2D(0, 0, GL_RGB, Size, Size, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, pcx.image.data8);
	imageDestroy(&pcx);

	return Texture;
}