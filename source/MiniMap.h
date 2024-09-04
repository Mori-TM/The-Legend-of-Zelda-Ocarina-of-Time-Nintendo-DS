#define MINI_MAP_RES floattot16(256.0f)

const u32 MiniMap[] =
{
	21,
	FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_COLOR, FIFO_TEX_COORD, FIFO_VERTEX16),
	GL_QUAD,
	
	RGB15(31, 31, 31),
	TEXTURE_PACK(MINI_MAP_RES, MINI_MAP_RES),
	VERTEX_PACK(floattov16(0.0), floattov16(0.0)), VERTEX_PACK(0, 0),

	FIFO_COMMAND_PACK(FIFO_COLOR, FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_COLOR),
	RGB15(31, 31, 31),	
	TEXTURE_PACK(floattot16(0.0), MINI_MAP_RES),
	VERTEX_PACK(floattov16(10.0), floattov16(0.0)), VERTEX_PACK(0, 0),
	RGB15(31, 31, 31),

	FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_COLOR, FIFO_TEX_COORD),
	TEXTURE_PACK(floattot16(0.0), floattot16(0.0)),
	VERTEX_PACK(floattov16(10.0), floattov16(10.0)), VERTEX_PACK(0, 0),
	RGB15(31, 31, 31),
	TEXTURE_PACK(MINI_MAP_RES, floattot16(0.0)),

	FIFO_COMMAND_PACK(FIFO_VERTEX16, FIFO_END, FIFO_NOP, FIFO_NOP),
	VERTEX_PACK(floattov16(0.0), floattov16(10.0)), VERTEX_PACK(0, 0),
};

#define MINIMAP_COUNT 1
s32 MiniMaps[MINIMAP_COUNT];

FORCE_INLINE void MiniMapInit()
{
//	MiniMaps[0] = LoadTexture(TEXTURE_SIZE_64, (u8*)Texkokiri_forest_minimap_pcx);
}

FORCE_INLINE void MiniMapRender()
{
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthofx(F0, F255, F191, F0, -F10, F10);
	glMatrixMode(GL_MODELVIEW);
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT);
	gluLookAtfx(F0, F0, F1,
				F0, F0, F0,
				F0, F1, F0);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
//	glScalefx(9.6f, 6.2f, 2.0f);
	glScalefx(F20, F20, F2);
//	glBindTexture(GL_TEXTURE_2D, MiniMaps[0]);
	glCallList((u32*)MiniMap);
	glPopMatrix(1);
}