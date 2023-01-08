FORCE_INLINE void RendererInit()
{	
	videoSetMode(MODE_0_3D);
	glInit();

	consoleDebugInit(DebugDevice_NOCASH);
	consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 23, 2, false, true);
	consoleDemoInit();

	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
//	vramSetBankC(VRAM_C_TEXTURE);
	vramSetBankD(VRAM_D_TEXTURE);

	glEnable(GL_ANTIALIAS);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POLY_OVERFLOW);
	glEnable(GL_BLEND);

	GFX_CLEAR_COLOR = glGlob->clearColor = ( glGlob->clearColor & 0xFFE08000) | (0x7FFF & 0x3A9C) | ((31 & 0x1F) << 16);
//	glClearColor(, 31);
	glClearPolyID(63);
	glClearDepth(0x7FFF);

	glViewport(0, 0, 255, 191);
	glLight(0, RGB15(31, 0, 0), 0, floattov10(-1.0f), 0);
	glLight(1, RGB15(0, 31, 0), floattov10(1.0f), 0, 0);
	glLight(2, RGB15(0, 0, 31), floattov10(-1.0f), 0, 0);

	glMaterialf(GL_AMBIENT, RGB15(1, 1, 1));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialf(GL_EMISSION, RGB15(16, 16, 16));
	glMaterialShinyness();

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);
}

FORCE_INLINE void RendererDebugInfos()
{
	int VertexCount;
	int PolyCount;

	glGetInt(GL_GET_VERTEX_RAM_COUNT, &VertexCount);
	glGetInt(GL_GET_POLYGON_RAM_COUNT, &PolyCount);

	printf("\x1b[1;2H Vertices in Ram: %i/6144", VertexCount);
	printf("\x1b[2;2H Triangles in Ram: %i/2048", PolyCount);
	printf("\x1b[3;2H Light Dir: %.1f %.1f %.1f", LightPos.x.Float(), LightPos.y.Float(), LightPos.z.Float());
	printf("\x1b[4;1H Light Color: %.2f %.2f %.2f", LightColor.x.Float(), LightColor.y.Float(), LightColor.z.Float());
//	printf("\x1b[5;3H Battery: %ld%%", getBatteryLevel());
	
	switch (SceneToRender)
	{
	case 0:
		printf("\x1b[6;3H Scene: Temple of Time");
		break;

	case 1:
		printf("\x1b[6;3H Scene: Kokiri Forest");
		break;
	}

	printf("\x1b[8;1H A = chnage scene");
	printf("\x1b[9;1H B = rotate light");
	printf("\x1b[10;1H X/Y = add/sub Links");
	printf("\x1b[11;1H select = map");

	printf("\x1b[12;1H Delta Time: %f", DeltaTime.Float());
}

FORCE_INLINE void RendererSetLight(int Light, vec3 Color, vec3 Dir)
{
	glLight(Light, 
			RGB15((int)(LightColor.x * 31.0f).Float(), (int)(LightColor.y * 31.0f).Float(), (int)(LightColor.z * 31.0f).Float()),
			f32tov10(LightPos.x.Fixed), f32tov10(LightPos.y.Fixed), f32tov10(LightPos.z.Fixed));
}