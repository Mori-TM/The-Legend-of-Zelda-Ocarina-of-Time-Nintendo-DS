u32 MiniMap[] =
{
	21,
	FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_COLOR, FIFO_TEX_COORD, FIFO_VERTEX16),
	GL_QUAD,
	
	RGB15(31, 31, 31),
	TEXTURE_PACK(floattot16(64.0), floattot16(64.0)),
	VERTEX_PACK(floattov16(0.0), floattov16(0.0)), VERTEX_PACK(0, 0),

	FIFO_COMMAND_PACK(FIFO_COLOR, FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_COLOR),
	RGB15(31, 31, 31),	
	TEXTURE_PACK(floattot16(0.0), floattot16(64.0)),
	VERTEX_PACK(floattov16(10.0), floattov16(0.0)), VERTEX_PACK(0, 0),
	RGB15(31, 31, 31),

	FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_COLOR, FIFO_TEX_COORD),
	TEXTURE_PACK(floattot16(0.0), floattot16(0.0)),
	VERTEX_PACK(floattov16(10.0), floattov16(10.0)), VERTEX_PACK(0, 0),
	RGB15(31, 31, 31),
	TEXTURE_PACK(floattot16(64.0), floattot16(0.0)),

	FIFO_COMMAND_PACK(FIFO_VERTEX16, FIFO_END, FIFO_NOP, FIFO_NOP),
	VERTEX_PACK(floattov16(0.0), floattov16(10.0)), VERTEX_PACK(0, 0),
};

#define MINIMAP_COUNT 1
s32 MiniMaps[MINIMAP_COUNT];

FORCE_INLINE void MiniMapInit()
{
	MiniMaps[0] = LoadTexture(TEXTURE_SIZE_64, (u8*)Texkokiri_forest_minimap_pcx);
}

FORCE_INLINE void MiniMapRender()
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthofx(0.0f, 255.0f, 191.0f, 0.0f, -10.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT);
	gluLookAtfx(0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
	glPushMatrix();

	glScalefx(9.6f, 6.2f, 2.0f);
	glBindTexture(GL_TEXTURE_2D, MiniMaps[0]);
	glCallList((u32*)MiniMap);
	glPopMatrix(1);
}