int KokiriForestTextures[35];
void KokiriForestLoadTextures()
{
	KokiriForestTextures[0] = LoadTextureFromFile("Texgrass.pcx");
	KokiriForestTextures[1] = LoadTextureFromFile("Texwall.pcx");
	ChangePalette = ARGB16_256(1,145,117,77);//ARGB16_256(1, 99, 63, 34);//ARGB16_256(1, 161,148,119);//
	KokiriForestTextures[2] = LoadTextureFromFile("Textrunks.pcx");
	LoadAlpha = true;
	ChangePalette = ARGB16_256(1, 38, 80, 30);
	KokiriForestTextures[3] = LoadTextureFromFile("Textrees.pcx");
	LoadMirrored = true;
	KokiriForestTextures[4] = LoadTextureFromFile("Texcut.pcx");
	KokiriForestTextures[5] = LoadTextureFromFile("Texhouse.pcx");
	KokiriForestTextures[6] = LoadTextureFromFile("Texbush.pcx");
	KokiriForestTextures[7] = LoadTextureFromFile("Texbushside.pcx");
	KokiriForestTextures[8] = LoadTextureFromFile("Texwalls.pcx");
	KokiriForestTextures[9] = LoadTextureFromFile("Texledge.pcx");
	KokiriForestTextures[10] = LoadTextureFromFile("Texbark.pcx");
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[11] = LoadTextureFromFile("Textreetop.pcx");
	KokiriForestTextures[12] = LoadTextureFromFile("Texexit.pcx");
	LoadMirrored = true;
	KokiriForestTextures[13] = LoadTextureFromFile("Texsteptop.pcx");
	KokiriForestTextures[14] = LoadTextureFromFile("Texstep.pcx");
	LoadMirrored = true;
	KokiriForestTextures[15] = LoadTextureFromFile("Textower.pcx");
	KokiriForestTextures[16] = LoadTextureFromFile("Texenter.pcx");
	LoadAlpha = true;
	KokiriForestTextures[17] = LoadTextureFromFile("Texcurtain.pcx");
	LoadAlpha = true;
	KokiriForestTextures[18] = LoadTextureFromFile("Texvine.pcx");
	LoadAlpha = true;
	KokiriForestTextures[19] = LoadTextureFromFile("Texedge.pcx");
	LoadMirrored = true;
	KokiriForestTextures[20] = LoadTextureFromFile("Texporch.pcx");
	LoadAlpha = true;
	KokiriForestTextures[21] = LoadTextureFromFile("Texrail.pcx");
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[22] = LoadTextureFromFile("Texladder.pcx");
	LoadAlpha = true;
	KokiriForestTextures[23] = LoadTextureFromFile("Texshroom.pcx");
	KokiriForestTextures[24] = LoadTextureFromFile("Texmido.pcx");
	KokiriForestTextures[25] = LoadTextureFromFile("Texsaria.pcx");
	KokiriForestTextures[26] = LoadTextureFromFile("Texshop.pcx");
	KokiriForestTextures[27] = LoadTextureFromFile("Texawning.pcx");
	LoadAlpha = true;
	KokiriForestTextures[28] = LoadTextureFromFile("Texfence.pcx");
	LoadAlpha = true;
	KokiriForestTextures[29] = LoadTextureFromFile("Texfencew.pcx");
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[30] = LoadTextureFromFile("Texbridge.pcx");
	LoadAlpha = true;
	KokiriForestTextures[31] = LoadTextureFromFile("Texleaf.pcx");
	LoadAlpha = true;
	KokiriForestTextures[32] = LoadTextureFromFile("Texvineh.pcx");
	KokiriForestTextures[33] = LoadTextureFromFile("Texwater.pcx");
	LoadAlpha = true;
	KokiriForestTextures[34] = LoadTextureFromFile("Texvinew.pcx");
}
u32* KokiriForestCollisionModel;
u32* KokiriForestModels[35];
void KokiriForestLoadModels()
{
	KokiriForestCollisionModel = LoadFile("KokiriForestCollision.bin", NULL);
	KokiriForestModels[0] = LoadFile("KokiriForestgrass.bin", NULL);
	KokiriForestModels[1] = LoadFile("KokiriForestwall.bin", NULL);
	KokiriForestModels[2] = LoadFile("KokiriForesttrunks.bin", NULL);
	KokiriForestModels[3] = LoadFile("KokiriForesttrees.bin", NULL);
	KokiriForestModels[4] = LoadFile("KokiriForestcut.bin", NULL);
	KokiriForestModels[5] = LoadFile("KokiriForesthouse.bin", NULL);
	KokiriForestModels[6] = LoadFile("KokiriForestbush.bin", NULL);
	KokiriForestModels[7] = LoadFile("KokiriForestbushside.bin", NULL);
	KokiriForestModels[8] = LoadFile("KokiriForestwalls.bin", NULL);
	KokiriForestModels[9] = LoadFile("KokiriForestledge.bin", NULL);
	KokiriForestModels[10] = LoadFile("KokiriForestbark.bin", NULL);
	KokiriForestModels[11] = LoadFile("KokiriForesttreetop.bin", NULL);
	KokiriForestModels[12] = LoadFile("KokiriForestexit.bin", NULL);
	KokiriForestModels[13] = LoadFile("KokiriForeststeptop.bin", NULL);
	KokiriForestModels[14] = LoadFile("KokiriForeststep.bin", NULL);
	KokiriForestModels[15] = LoadFile("KokiriForesttower.bin", NULL);
	KokiriForestModels[16] = LoadFile("KokiriForestenter.bin", NULL);
	KokiriForestModels[17] = LoadFile("KokiriForestcurtain.bin", NULL);
	KokiriForestModels[18] = LoadFile("KokiriForestvine.bin", NULL);
	KokiriForestModels[19] = LoadFile("KokiriForestedge.bin", NULL);
	KokiriForestModels[20] = LoadFile("KokiriForestporch.bin", NULL);
	KokiriForestModels[21] = LoadFile("KokiriForestrail.bin", NULL);
	KokiriForestModels[22] = LoadFile("KokiriForestladder.bin", NULL);
	KokiriForestModels[23] = LoadFile("KokiriForestshroom.bin", NULL);
	KokiriForestModels[24] = LoadFile("KokiriForestmido.bin", NULL);
	KokiriForestModels[25] = LoadFile("KokiriForestsaria.bin", NULL);
	KokiriForestModels[26] = LoadFile("KokiriForestshop.bin", NULL);
	KokiriForestModels[27] = LoadFile("KokiriForestawning.bin", NULL);
	KokiriForestModels[28] = LoadFile("KokiriForestfence.bin", NULL);
	KokiriForestModels[29] = LoadFile("KokiriForestfencew.bin", NULL);
	KokiriForestModels[30] = LoadFile("KokiriForestbridge.bin", NULL);
	KokiriForestModels[31] = LoadFile("KokiriForestleaf.bin", NULL);
	KokiriForestModels[32] = LoadFile("KokiriForestvineh.bin", NULL);
	KokiriForestModels[33] = LoadFile("KokiriForestwater.bin", NULL);
	KokiriForestModels[34] = LoadFile("KokiriForestvinew.bin", NULL);
}
void KokiriForestDraw()
{
	glPolyFmt(2031745| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[0]);
	glCallList(KokiriForestModels[0]);
	
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[1]);
	glCallList(KokiriForestModels[1]);
	//trunks
	glMaterialf(GL_AMBIENT, RGB15(28, 28, 28));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[2]);
	glCallList(KokiriForestModels[2]);	
	glMaterialf(GL_AMBIENT, RGB15(1, 1, 1));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[3]);
	glCallList(KokiriForestModels[3]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[4]);
	glCallList(KokiriForestModels[4]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[5]);
	glCallList(KokiriForestModels[5]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[6]);
	glCallList(KokiriForestModels[6]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[7]);
	glCallList(KokiriForestModels[7]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[8]);
	glCallList(KokiriForestModels[8]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[9]);
	glCallList(KokiriForestModels[9]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[10]);
	glCallList(KokiriForestModels[10]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[11]);
	glCallList(KokiriForestModels[11]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[12]);
	glCallList(KokiriForestModels[12]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[13]);
	glCallList(KokiriForestModels[13]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[14]);
	glCallList(KokiriForestModels[14]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[15]);
	glCallList(KokiriForestModels[15]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[16]);
	glCallList(KokiriForestModels[16]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[17]);
	glCallList(KokiriForestModels[17]);
	glPolyFmt(2031809| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[18]);
	glCallList(KokiriForestModels[18]);
	glPolyFmt(2031745| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[19]);
	glCallList(KokiriForestModels[19]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[20]);
	glCallList(KokiriForestModels[20]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[21]);
	glCallList(KokiriForestModels[21]);
	glPolyFmt(2031809| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[22]);
	glCallList(KokiriForestModels[22]);
	glPolyFmt(2031745| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[23]);
	glCallList(KokiriForestModels[23]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[24]);
	glCallList(KokiriForestModels[24]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[25]);
	glCallList(KokiriForestModels[25]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[26]);
	glCallList(KokiriForestModels[26]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[27]);
	glCallList(KokiriForestModels[27]);
	glPolyFmt(2031809| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[28]);
	glCallList(KokiriForestModels[28]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[29]);
	glCallList(KokiriForestModels[29]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[30]);
	glCallList(KokiriForestModels[30]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[31]);
	glCallList(KokiriForestModels[31]);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[32]);
	glCallList(KokiriForestModels[32]);
	
	glMatrixMode(GL_TEXTURE);	
	glStoreMatrix(0);
	glTranslatef32(-Ticksf32, -Ticksf32, 0);
	glMatrixMode(GL_MODELVIEW);	
	
	{
		glPolyFmt(983233 | POLY_FOG);
		glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[33]);
		glCallList(KokiriForestModels[33]);
	}

	glMatrixMode(GL_TEXTURE);	
	glRestoreMatrix(0);
	glMatrixMode(GL_MODELVIEW);
	
//	glPolyFmt(983233);
//	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[33]);
//	glCallList(KokiriForestModels[33]);
	glPolyFmt(2031809| POLY_FOG);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[34]);
	glCallList(KokiriForestModels[34]);
	
}
void KokiriForestDeleteTextures()
{
	glDeleteTextures(35, KokiriForestTextures);
}
void KokiriForestDeleteModels()
{
	free(KokiriForestCollisionModel);
	free(KokiriForestModels[0]);
	free(KokiriForestModels[1]);
	free(KokiriForestModels[2]);
	free(KokiriForestModels[3]);
	free(KokiriForestModels[4]);
	free(KokiriForestModels[5]);
	free(KokiriForestModels[6]);
	free(KokiriForestModels[7]);
	free(KokiriForestModels[8]);
	free(KokiriForestModels[9]);
	free(KokiriForestModels[10]);
	free(KokiriForestModels[11]);
	free(KokiriForestModels[12]);
	free(KokiriForestModels[13]);
	free(KokiriForestModels[14]);
	free(KokiriForestModels[15]);
	free(KokiriForestModels[16]);
	free(KokiriForestModels[17]);
	free(KokiriForestModels[18]);
	free(KokiriForestModels[19]);
	free(KokiriForestModels[20]);
	free(KokiriForestModels[21]);
	free(KokiriForestModels[22]);
	free(KokiriForestModels[23]);
	free(KokiriForestModels[24]);
	free(KokiriForestModels[25]);
	free(KokiriForestModels[26]);
	free(KokiriForestModels[27]);
	free(KokiriForestModels[28]);
	free(KokiriForestModels[29]);
	free(KokiriForestModels[30]);
	free(KokiriForestModels[31]);
	free(KokiriForestModels[32]);
	free(KokiriForestModels[33]);
	free(KokiriForestModels[34]);
}
