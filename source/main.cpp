//For syntax highlighting
#define ARM9
#define FORCE_INLINE __attribute__((always_inline)) inline
#define PI 3.14159265359f
//#define NO_HARDWARE_ACCELERATION
#define NDEBUG
#define USE_INSTUCTION_CHACHE
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

#include "Texkokiri_forest_minimap_pcx.h"
#include "Link_bin.h"

#include "Fixed.h"
#include "Math3D.h"
#include "Globals.h"
#include "Helper.h"
#include "glFixed.h"
#include "DisplayList.h"
#include "Renderer.h"
#include "Audio.h"
#include "MiniMap.h"
#include "Md2Loader.h"
#include "Collision.h"

#include "Models/KokiriForest.h"
#include "Models/TempleOfTime.h"
#include "Models/Sphere.h"
//#include "Models/YoungLink.h"

#include "LinkTex_pcx.h"

#include "Camera.h"

int YoungLinkTextures[1];
void YoungLinkLoadTextures()
{
	YoungLinkTextures[0] = LoadTexture(TEXTURE_SIZE_128, (u8*)LinkTex_pcx);
}

void YoungLinkDeleteTextures()
{
	glDeleteTextures(1, YoungLinkTextures);
}

Md2::ModelData Model;
u32* List;

FORCE_INLINE void CreatePack(u32 Index, u32* List, u32* Off, u32* CmdOff, Md2::Proccess* Model)
{
	(*Off)++;
	*CmdOff += 5;
	List[*CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	SetTexCoordToList(List, *Off, Model->TexCoord[Index]);
	SetNormalToList(List, *Off, Model->Normal[Index]);
	SetVertexToList(List, *Off, Model->Vertex[Index]);
}

void DrawMd2(u32* List, Md2::Proccess* Model)
{
	register u32 Off = 0;
	register u32 CmdOff = 7;
	register u32 i = 0;
	Model->Update(i);

	SetTexCoordToList(List, Off, Model->TexCoord[0]);
	SetNormalToList(List, Off, Model->Normal[0]);
	SetVertexToList(List, Off, Model->Vertex[0]);
	List[CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	CreatePack(2, List, &Off, &CmdOff, Model);
	CreatePack(1, List, &Off, &CmdOff, Model);

	for (i = 1; i < Model->Model->NumTriangles; i++)
	{
		Model->Update(i);
		
		CreatePack(0, List, &Off, &CmdOff, Model);
		CreatePack(2, List, &Off, &CmdOff, Model);
		CreatePack(1, List, &Off, &CmdOff, Model);
	}

//	Model->Update(i);
	
//	CreatePack(0, List, &Off, &CmdOff, Model);
//	CreatePack(2, List, &Off, &CmdOff, Model);
//	CreatePack(1, List, &Off, &CmdOff, Model);
}

f32 AnimationSpeed = 3.74;

FORCE_INLINE void Display()
{	
	glEnable(GL_TEXTURE_2D);
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);

	glPushMatrix();
		glTranslateV(&PlayerPos);
		glRotatefx(FN90, F1, F0, F0);

		Md2::Proccess ProcModel;
		ProcModel.Model = &Model;
		ProcModel.Start(0, 20);
	//	ProcModel.Start(0, Model.NumFrames);
		DrawMd2(List, &ProcModel);
		ProcModel.End((DeltaTime * AnimationSpeed));
		glBindTexture(GL_TEXTURE_2D, YoungLinkTextures[0]);
		glCallList((u32*)List);
	glPopMatrix(1);

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 );

	glPushMatrix();
		glScalefx(F100, F100, F100);

		if (SceneToRender == 0)
			TempleofTimeDraw();
		else if (SceneToRender == 1)
			KokiriForestDraw();
	glPopMatrix(1);
}

void PollKeyEvents()
{
	scanKeys();
	touchRead(&Touch);
	
	KeysHeld = keysHeld();
	KeysDown = keysDown();

	if (KeysDown & KEY_A) 
	{
		SceneToRender++;
		if (SceneToRender >= 2)
			SceneToRender = 0;

		if (SceneToRender == 0)
		{
			KokiriForestDeleteTextures();
			TempleofTimeLoadTextures();
		}
		else if (SceneToRender == 1)
		{
			TempleofTimeDeleteTextures();
			KokiriForestLoadTextures();			
		}		
	}
	if (KeysDown & KEY_B) UpdateTimer = !UpdateTimer;
	if (KeysDown & KEY_X)
	{
		CameraPos = { 0.0, 3.0, 8.0 };
		CameraDir = { 0.0, 0.0,-1.0 };
		CameraUp = { 0.0, 1.0, 0.0 };
		PlayerPos = { 5.0, 20.0, 0.0 };
		PlayerDir = { 0.0, 0.0,-1.0 };
		PlayerNor = { 0.0, 0.0,-1.0 };
		PlayerCameraDist = 0.0;
		PlayerFallSpeed = -0.1;
		PlayerVelocity = 0.0;
	}

	//if (KeysDown & KEY_X) LinkDrawCount++;
	//if (KeysDown & KEY_Y && LinkDrawCount > 0) LinkDrawCount--;
	if (KeysDown & KEY_SELECT) RenderMap = !RenderMap;

	if (KeysHeld & KEY_START) exit(0);
}

int main() 
{	
	RendererInit();
	
	MiniMapInit();
	YoungLinkLoadTextures();
//	TempleofTimeLoadTextures();
	KokiriForestLoadTextures();

	AudioInit();
	AudioLoad(MOD_ZELDA);
	AudioPlay(MOD_ZELDA);
	/*
	#define VERTICES 2088
	vec3 OGPoses[VERTICES];
	for (int i = 0; i < VERTICES; i += 3)
	{
		OGPoses[i] = GetVertexFromList(YoungLink, i);
		OGPoses[i + 1] = GetVertexFromList(YoungLink, i + 1);
		OGPoses[i + 2] = GetVertexFromList(YoungLink, i + 2);
	}
	*/
	Md2::Load((u8*)Link_bin, (u32)Link_bin_size, 128, 128, &Model);
	List = (u32*)malloc((((Model.NumTriangles * 3) - 2) * 5 + 12) * sizeof(u32));
	List[0] = ((Model.NumTriangles * 3) - 2) * 5 + 11;
	List[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16);
	List[2] = GL_TRIANGLE;

	timerStart(0, ClockDivider_1024, 0, NULL);

	f32 Time = 0.0f;
	while (1) 
	{		
		PollKeyEvents();		

		if (UpdateTimer)
			Time += DeltaTime * LightDelay;

	//	if (Time >= DEGREES_IN_CIRCLEF32)
	//		Time = 0.0f;

		glLoadIdentity();

		LightPos = { -sinf32(Time) * F8, FN8, -cosf32(Time) * F8 };	
		Normalize3P(&LightPos);

	//	glLight(0, RGB15(floattov10(LightColor.x.Float()), floattov10(LightColor.y.Float()), floattov10(LightColor.z.Float())), floattov10(LightPos.x.Float()), floattov10(LightPos.y.Float()), floattov10(LightPos.z.Float()));
	//	LightPos =
		RendererSetLight(0, LightColor, LightPos);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspectivefx(FOV, Aspect, NearZ, FarZ);
	//	UpdateCamera();
	//	UpdateLink();
		UpdateCamera();
		glMatrixMode(GL_MODELVIEW);	
		

			/*	
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);
		glPushMatrix();
		glTranslateV(&Pos);
		glScalefx(1.0, 1.0, 1.0);
		if (Collision == -1)//no collision
			glBindTexture(0, 0);
		else if (Normal.y < 0.1)
		{
			glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[Collision]);
			f32 V = Radius - Dist;
			vec3 O = { Normal.x * V, Normal.y * V, Normal.z * V };
			PlayerPos = Add3P(&PlayerPos, &O);
		//	PlayerPos = Add3P(&PlayerPos, &P);
		}
			
		glCallList((u32*)Sphere);
		glPopMatrix(1);
*/
		Display();

		if (RenderMap)
			MiniMapRender();
		
	//	RendererDebugInfos();
	//	printf("\x1b[13;2H Player Col: %.1f %.1f %.1f", Normal.x.Float(), Normal.y.Float(), Normal.z.Float());
	//	printf("\x1b[13;2H Player Rot: %.1f %.1f", PlayerRot.x.Float(), PlayerRot.y.Float());
	//	printf("\x1b[14;2H Camera Dir: %.1f %.1f %.1f", CameraDir.x.Float(), CameraDir.y.Float(), CameraDir.z.Float());
		glFlush(0);		
	//	swiWaitForVBlank();
	//	consoleClear();		

		Ticks += timerElapsed(0);
		if (Ticks >= INT32_MAX)
			Ticks = 0;
		Ticksf32 = (float)Ticks;
		GetDeltaTime();
		Frame = (Frame + 1) % MaxFrames;
	//	printf("\x1b[19;2H Ticks: %d", Ticks);
	//	printf("\x1b[19;2H Dt int: %u", DeltaTimeI);
	}

	TempleofTimeDeleteTextures();
	KokiriForestDeleteTextures();
	YoungLinkDeleteTextures();

	return 0;
}
