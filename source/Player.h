int YoungLinkTextures[1];

void YoungLinkLoadTextures()
{
	LoadDirect = true;
	YoungLinkTextures[0] = LoadTextureFromFile("LinkTex.pcx");
}

void YoungLinkDeleteTextures()
{
	glDeleteTextures(1, YoungLinkTextures);
}

Md2ModelData PlayerModel;
u32* PlayerDisplayList;

void YoungLinkInit()
{
	Md2LoadFromFile("Link.bin", 512, 512, &PlayerModel);
	PlayerDisplayList = (u32*)malloc((((PlayerModel.NumTriangles * 3) - 2) * 5 + 12) * sizeof(u32));
	PlayerDisplayList[0] = ((PlayerModel.NumTriangles * 3) - 2) * 5 + 11;
	PlayerDisplayList[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16);
	PlayerDisplayList[2] = GL_TRIANGLE;
}

void YoungLinkDestroy()
{
	free(PlayerDisplayList);
	Md2Destroy(&PlayerModel);
	YoungLinkDeleteTextures();
}