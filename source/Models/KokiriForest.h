#include "KokiriForestCollision_bin.h"
#include "Texgrass_pcx.h"
#include "KokiriForestgrass_bin.h"
#include "Texwall_pcx.h"
#include "KokiriForestwall_bin.h"
#include "Textrunks_pcx.h"
#include "KokiriForesttrunks_bin.h"
#include "Textrees_pcx.h"
#include "KokiriForesttrees_bin.h"
#include "Texcut_pcx.h"
#include "KokiriForestcut_bin.h"
#include "Texhouse_pcx.h"
#include "KokiriForesthouse_bin.h"
#include "Texbush_pcx.h"
#include "KokiriForestbush_bin.h"
#include "Texbushside_pcx.h"
#include "KokiriForestbushside_bin.h"
#include "Texwalls_pcx.h"
#include "KokiriForestwalls_bin.h"
#include "Texledge_pcx.h"
#include "KokiriForestledge_bin.h"
#include "Texbark_pcx.h"
#include "KokiriForestbark_bin.h"
#include "Textreetop_pcx.h"
#include "KokiriForesttreetop_bin.h"
#include "Texexit_pcx.h"
#include "KokiriForestexit_bin.h"
#include "Texsteptop_pcx.h"
#include "KokiriForeststeptop_bin.h"
#include "Texstep_pcx.h"
#include "KokiriForeststep_bin.h"
#include "Textower_pcx.h"
#include "KokiriForesttower_bin.h"
#include "Texenter_pcx.h"
#include "KokiriForestenter_bin.h"
#include "Texcurtain_pcx.h"
#include "KokiriForestcurtain_bin.h"
#include "Texvine_pcx.h"
#include "KokiriForestvine_bin.h"
#include "Texedge_pcx.h"
#include "KokiriForestedge_bin.h"
#include "Texporch_pcx.h"
#include "KokiriForestporch_bin.h"
#include "Texrail_pcx.h"
#include "KokiriForestrail_bin.h"
#include "Texladder_pcx.h"
#include "KokiriForestladder_bin.h"
#include "Texshroom_pcx.h"
#include "KokiriForestshroom_bin.h"
#include "Texmido_pcx.h"
#include "KokiriForestmido_bin.h"
#include "Texsaria_pcx.h"
#include "KokiriForestsaria_bin.h"
#include "Texshop_pcx.h"
#include "KokiriForestshop_bin.h"
#include "Texawning_pcx.h"
#include "KokiriForestawning_bin.h"
#include "Texfence_pcx.h"
#include "KokiriForestfence_bin.h"
#include "Texfencew_pcx.h"
#include "KokiriForestfencew_bin.h"
#include "Texbridge_pcx.h"
#include "KokiriForestbridge_bin.h"
#include "Texleaf_pcx.h"
#include "KokiriForestleaf_bin.h"
#include "Texvineh_pcx.h"
#include "KokiriForestvineh_bin.h"
#include "Texwater_pcx.h"
#include "KokiriForestwater_bin.h"
#include "Texfade_pcx.h"
#include "KokiriForestfade_bin.h"
#include "Texvinew_pcx.h"
#include "KokiriForestvinew_bin.h"
int KokiriForestTextures[36];
void KokiriForestLoadTextures()
{
	KokiriForestTextures[0] = LoadTexture((u8*)Texgrass_pcx);
	KokiriForestTextures[1] = LoadTexture((u8*)Texwall_pcx);
	KokiriForestTextures[2] = LoadTexture((u8*)Textrunks_pcx);
	LoadAlpha = true;
	KokiriForestTextures[3] = LoadTexture((u8*)Textrees_pcx);
	LoadMirrored = true;
	KokiriForestTextures[4] = LoadTexture((u8*)Texcut_pcx);
	KokiriForestTextures[5] = LoadTexture((u8*)Texhouse_pcx);
	KokiriForestTextures[6] = LoadTexture((u8*)Texbush_pcx);
	KokiriForestTextures[7] = LoadTexture((u8*)Texbushside_pcx);
	KokiriForestTextures[8] = LoadTexture((u8*)Texwalls_pcx);
	KokiriForestTextures[9] = LoadTexture((u8*)Texledge_pcx);
	KokiriForestTextures[10] = LoadTexture((u8*)Texbark_pcx);
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[11] = LoadTexture((u8*)Textreetop_pcx);
	KokiriForestTextures[12] = LoadTexture((u8*)Texexit_pcx);
	LoadMirrored = true;
	KokiriForestTextures[13] = LoadTexture((u8*)Texsteptop_pcx);
	KokiriForestTextures[14] = LoadTexture((u8*)Texstep_pcx);
	KokiriForestTextures[15] = LoadTexture((u8*)Textower_pcx);
	KokiriForestTextures[16] = LoadTexture((u8*)Texenter_pcx);
	LoadAlpha = true;
	KokiriForestTextures[17] = LoadTexture((u8*)Texcurtain_pcx);
	LoadAlpha = true;
	KokiriForestTextures[18] = LoadTexture((u8*)Texvine_pcx);
	LoadAlpha = true;
	KokiriForestTextures[19] = LoadTexture((u8*)Texedge_pcx);
	LoadMirrored = true;
	KokiriForestTextures[20] = LoadTexture((u8*)Texporch_pcx);
	LoadAlpha = true;
	KokiriForestTextures[21] = LoadTexture((u8*)Texrail_pcx);
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[22] = LoadTexture((u8*)Texladder_pcx);
	LoadAlpha = true;
	KokiriForestTextures[23] = LoadTexture((u8*)Texshroom_pcx);
	KokiriForestTextures[24] = LoadTexture((u8*)Texmido_pcx);
	KokiriForestTextures[25] = LoadTexture((u8*)Texsaria_pcx);
	KokiriForestTextures[26] = LoadTexture((u8*)Texshop_pcx);
	KokiriForestTextures[27] = LoadTexture((u8*)Texawning_pcx);
	LoadAlpha = true;
	KokiriForestTextures[28] = LoadTexture((u8*)Texfence_pcx);
	LoadAlpha = true;
	KokiriForestTextures[29] = LoadTexture((u8*)Texfencew_pcx);
	LoadMirrored = true;
	LoadAlpha = true;
	KokiriForestTextures[30] = LoadTexture((u8*)Texbridge_pcx);
	LoadAlpha = true;
	KokiriForestTextures[31] = LoadTexture((u8*)Texleaf_pcx);
	LoadAlpha = true;
	KokiriForestTextures[32] = LoadTexture((u8*)Texvineh_pcx);
	KokiriForestTextures[33] = LoadTexture((u8*)Texwater_pcx);
	KokiriForestTextures[34] = LoadTexture((u8*)Texfade_pcx);
	LoadAlpha = true;
	KokiriForestTextures[35] = LoadTexture((u8*)Texvinew_pcx);
}
void KokiriForestDraw()
{
	glPolyFmt(2031745);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[0]);
	glCallList((u32*)KokiriForestgrass_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[1]);
	glCallList((u32*)KokiriForestwall_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[2]);
	glCallList((u32*)KokiriForesttrunks_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[3]);
	glCallList((u32*)KokiriForesttrees_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[4]);
	glCallList((u32*)KokiriForestcut_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[5]);
	glCallList((u32*)KokiriForesthouse_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[6]);
	glCallList((u32*)KokiriForestbush_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[7]);
	glCallList((u32*)KokiriForestbushside_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[8]);
	glCallList((u32*)KokiriForestwalls_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[9]);
	glCallList((u32*)KokiriForestledge_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[10]);
	glCallList((u32*)KokiriForestbark_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[11]);
	glCallList((u32*)KokiriForesttreetop_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[12]);
	glCallList((u32*)KokiriForestexit_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[13]);
	glCallList((u32*)KokiriForeststeptop_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[14]);
	glCallList((u32*)KokiriForeststep_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[15]);
	glCallList((u32*)KokiriForesttower_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[16]);
	glCallList((u32*)KokiriForestenter_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[17]);
	glCallList((u32*)KokiriForestcurtain_bin);
	glPolyFmt(2031809);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[18]);
	glCallList((u32*)KokiriForestvine_bin);
	glPolyFmt(2031745);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[19]);
	glCallList((u32*)KokiriForestedge_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[20]);
	glCallList((u32*)KokiriForestporch_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[21]);
	glCallList((u32*)KokiriForestrail_bin);
	glPolyFmt(2031809);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[22]);
	glCallList((u32*)KokiriForestladder_bin);
	glPolyFmt(2031745);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[23]);
	glCallList((u32*)KokiriForestshroom_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[24]);
	glCallList((u32*)KokiriForestmido_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[25]);
	glCallList((u32*)KokiriForestsaria_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[26]);
	glCallList((u32*)KokiriForestshop_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[27]);
	glCallList((u32*)KokiriForestawning_bin);
	glPolyFmt(2031809);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[28]);
	glCallList((u32*)KokiriForestfence_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[29]);
	glCallList((u32*)KokiriForestfencew_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[30]);
	glCallList((u32*)KokiriForestbridge_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[31]);
	glCallList((u32*)KokiriForestleaf_bin);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[32]);
	glCallList((u32*)KokiriForestvineh_bin);
	glPolyFmt(983233);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[33]);
	glCallList((u32*)KokiriForestwater_bin);
	glPolyFmt(2031745);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[34]);
	glCallList((u32*)KokiriForestfade_bin);
	glPolyFmt(2031809);
	glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[35]);
	glCallList((u32*)KokiriForestvinew_bin);
}
void KokiriForestDeleteTextures()
{
	glDeleteTextures(36, KokiriForestTextures);
}
