//For syntax highlighting
#define ARM9
#define FORCE_INLINE __attribute__((always_inline)) inline
#define PI 12868
//#define NO_HARDWARE_ACCELERATION
//#define NDEBUG
#define USE_INSTUCTION_CHACHE
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

//#include "Texkokiri_forest_minimap_pcx.h"
#include "LinkTex_pcx.h"
#include "Link_bin.h"

#include "Fixed.h"
#include "Math3D.h"
#include "Globals.h"
#include "Helper.h"
#include "glFixed.h"
#include "DisplayList.h"

#include "Models/KokiriForest.h"
//#include "Models/Sphere.h"
vec3 PlayerPosLast = { F5, F0, 0 };
#include "Renderer.h"
#include "Audio.h"
//#include "MiniMap.h"
#include "Md2Loader.h"
#include "Collision.h"

/*
To-Do
-subdived ramp at deku tree
-split forest mesh more?
-implement wall collision
-clean up collision mesh
-remove overlapping triangles in render mesh
-fix wierd alpha texture issue on ds hardware

Infos for me
-If ramp triangle collision doesn't work 100% just subdived it
-collision model needs to be half the size of the render model, 
 because collision only uses 16 bit fixed for data storing
*/

//#include "Models/YoungLink.h"

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

Md2ModelData Model;
u32* List;

FORCE_INLINE void CreatePack(u32 Index, u32* List, u32* Off, u32* CmdOff)
{
	(*Off)++;
	*CmdOff += 5;
	List[*CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	SetTexCoordToList(List, *Off, Md2Proceess.TexCoord[Index]);
	SetNormalToList(List, *Off, Md2Proceess.Normal[Index]);
	SetVertexToList(List, *Off, Md2Proceess.Vertex[Index]);
}

void DrawMd2(u32* List)
{
	u32 Off = 0;
	u32 CmdOff = 7;
	register u32 i = 0;
	Md2Update(i);

	SetTexCoordToList(List, Off, Md2Proceess.TexCoord[0]);
	SetNormalToList(List, Off, Md2Proceess.Normal[0]);
	SetVertexToList(List, Off, Md2Proceess.Vertex[0]);
	List[CmdOff] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
	CreatePack(2, List, &Off, &CmdOff);
	CreatePack(1, List, &Off, &CmdOff);

	for (i = 1; i < Model.NumTriangles; i++)
	{
		Md2Update(i);
		
		CreatePack(0, List, &Off, &CmdOff);
		CreatePack(2, List, &Off, &CmdOff);
		CreatePack(1, List, &Off, &CmdOff);
	}

//	Model->Update(i);
	
//	CreatePack(0, List, &Off, &CmdOff, Model);
//	CreatePack(2, List, &Off, &CmdOff, Model);
//	CreatePack(1, List, &Off, &CmdOff, Model);
}

f32 AnimationSpeed = 15319;//3,74

FORCE_INLINE void Display()
{	
	
	vec3 SrcPos;
	Vec3(-50899, 945, -46252, SrcPos);

	glEnable(GL_TEXTURE_2D);

	glPolyFmt(POLY_ALPHA(25) | POLY_CULL_NONE);

	
/*
	glPushMatrix();		
	//	PlayerPos[1] -= Mul(PlayerHitboxSize, 2048);
		glTranslateV(SrcPos);
	//	PlayerPos[1] += Mul(PlayerHitboxSize, 2048);
		glScalefx(2548, 2548, 2548);

		glBindTexture(GL_TEXTURE_2D, 0);
		glCallList((u32*)Sphere);
	glPopMatrix(1);

*/
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT2);

	

	glPushMatrix();		
	//	PlayerPos[1] -= Mul(PlayerHitboxSize, 2048);
		glTranslateV(PlayerPos);
	//	PlayerPos[1] += Mul(PlayerHitboxSize, 2048);
		glScalefx(2548, 2548, 2548);
		glRotatefx(-F90, F1, F0, F0);

		Md2Proceess.Model = &Model;
		Md2Start(0, 20);//20
	//	ProcModel.Start(0, Model.NumFrames);
		DrawMd2(List);
		Md2End(Mul(DeltaTime, AnimationSpeed));
		glBindTexture(GL_TEXTURE_2D, YoungLinkTextures[0]);
		glCallList((u32*)List);
	/*
		glTranslatef32(0, F3, 0);
		glCallList((u32*)List);

		glTranslatef32(0, -F6, 0);
		glCallList((u32*)List);

		glTranslatef32(0, -F3, 0);
		glCallList((u32*)List);

		glTranslatef32(0, -F3, 0);
		glCallList((u32*)List);

		glTranslatef32(0, -F3, 0);
		glCallList((u32*)List);

		glTranslatef32(0, -F3, 0);
		glCallList((u32*)List);
	*/
//		glBindTexture(GL_TEXTURE_2D, 0);
//		glCallList((u32*)Sphere);
	glPopMatrix(1);

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);

	glPushMatrix();
		glScalefx(F100, F100, F100);		
		KokiriForestDraw();
	glPopMatrix(1);
}

int FlushMode = 0;

void PollKeyEvents()
{
	scanKeys();
	touchRead(&Touch);
	
	KeysHeld = keysHeld();
	KeysDown = keysDown();

	if (KeysDown & KEY_A) 
	{
		PlayerPos[1] = F20;
		PlayerCameraDist = 0;

		PlayerFallSpeed = -900;//0.25
		PlayerVelocity = 0;
	}
	if (KeysDown & KEY_B) UpdateTimer = !UpdateTimer;
	if (KeysDown & KEY_X)
	{
		FlushMode++;
		Vec3(0, F3, F8, CameraPos);
		Vec3(0, 0, -F1, CameraDir);
		Vec3(0, F1, 0, CameraUp);
		Vec3(0, F20, 0, PlayerPos);
		Vec3(0, 0, -F1, PlayerDir);
		Vec3(0, 0, -F1, PlayerNor);
		PlayerCameraDist = 0;

		PlayerFallSpeed = -900;//0.25
		PlayerVelocity = 0;
	}
	if (KeysDown & KEY_Y && PlayerOnGround)
	{
		PlayerVelocity = 2000;
	}
	
	//if (KeysDown & KEY_X) LinkDrawCount++;
	//if (KeysDown & KEY_Y && LinkDrawCount > 0) LinkDrawCount--;
	if (KeysDown & KEY_SELECT) RenderMap = !RenderMap;

	if (KeysHeld & KEY_START) exit(0);
}

int main() 
{	
	RendererInit();
	
//	MiniMapInit();
	YoungLinkLoadTextures();
//	TempleofTimeLoadTextures();
	KokiriForestLoadTextures();

	AudioInit();
	AudioLoadWav(SFX_MAINTHEME);
	AudioLoadWav(SFX_RIVER);
	AudioHandle MainHandle = AudioPlayWav(SFX_MAINTHEME, 200);
	AudioHandle RiverHandle = AudioPlayWav(SFX_RIVER, 200);
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
	Md2Load((u8*)Link_bin, (u32)Link_bin_size, 128, 128, &Model);
	List = (u32*)malloc((((Model.NumTriangles * 3) - 2) * 5 + 12) * sizeof(u32));
	List[0] = ((Model.NumTriangles * 3) - 2) * 5 + 11;
	List[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16);
	List[2] = GL_TRIANGLE;

	timerStart(0, ClockDivider_1024, 0, NULL);

	int Frame = 0;

	f32 Time = 0;
	while (1) 
	{	
		Copy3(PlayerPos, PlayerPosLast);
		PollKeyEvents();		

		if (UpdateTimer)
			Time += Mul(DeltaTime, LIGHT_DELAY);

	//	if (Time >= DEGREES_IN_CIRCLEF32)
	//		Time = 0.0f;

		vec3 SrcPos;

		Vec3(Mul(-sinf32(Time), F5), F5, Mul(-cosf32(Time), F5), SrcPos);
		AudioSet3D(SrcPos, F100, 10, 200, CameraPos, CameraDir, MainHandle);
		
		Vec3(-74023, 849, -103141, SrcPos);
		AudioSet3D(SrcPos, F50, 10, 200, CameraPos, CameraDir, RiverHandle);
		
		
		
	//	vec3 SrcPos = { 0, F2, 0 };
		
	//	vec3 SrcPos;
		
		
			/*	
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);
		glPushMatrix();
		glTranslateV(&Pos);
		glScalefx(1.0, 1.0, 1.0);
		if (Collision == -1)//no collision
			glBindTexture(0, 0);
		else if (Normal[1] < 0.1)
		{
			glBindTexture(GL_TEXTURE_2D, KokiriForestTextures[Collision]);
			f32 V = Radius - Dist;
			vec3 O = { Normal[0] * V, Normal[1] * V, Normal[2] * V };
			PlayerPos = Add3P(&PlayerPos, &O);
		//	PlayerPos = Add3P(&PlayerPos, &P);
		}
			
		glCallList((u32*)Sphere);
		glPopMatrix(1);
*/		
	//	if (Frame == 0)
		{
			UpdateCamera();

			static float Test = 2.0;
		//	for (int i = 0; i < 2000; i++)
		//	{
		//		Test = Test * 2 / 1.2 + Test * 0.99 + ToFloat(PlayerDir[0]);
		//	}	

			printf("\x1b[0;0H Collision Fps: %d, %f", MeasureFps(&CollisionFrame), Test);
		}
	//	else
		{
			glLoadIdentity();
		
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspectivefx(FOV, ASPECT_RATIO, NEAR_Z, FAR_Z);
		//	UpdateCamera();
		//	UpdateLink();

			PlayerPos[1] += 5248;
			gluLookAtV(CameraPos, PlayerPos, CameraUp);
			PlayerPos[1] -= 5248;
			
			glMatrixMode(GL_MODELVIEW);	
		
			Vec3(Mul(-sinf32(Time), F8), -F5, Mul(-cosf32(Time), F8), LightPos);
			Normalize3(LightPos);

			RendererSetLight(0, LightColor, LightPos);
			RendererSetLight(2, LightColor, CameraDir);

			
			Vec3(-50899, 945, -46252, SrcPos);
			vec3 Color = { 0, 4096, 0 };
			RendererSetPointLight(1, Color, SrcPos, PlayerPos, F10);

			Display();

			vec2 Time = { 1, 1 };
			//Vec2f(Mul(DeltaTime, 10), Time);

			//animate water
			for (int j = 0; j < 75; j += 3)
			{
				vec2 TexA; GetTexCoordFromList((u32*)KokiriForestwater_bin, j	 , TexA);
				vec2 TexB; GetTexCoordFromList((u32*)KokiriForestwater_bin, j + 1, TexB);
				vec2 TexC; GetTexCoordFromList((u32*)KokiriForestwater_bin, j + 2, TexC);
			//	f32 Multi = 0019531f;
			//	vec2 Time = { DeltaTime * Multi, DeltaTime * Multi };
				
				Sub2(TexA, Time, TexA);
				Sub2(TexB, Time, TexB);
				Sub2(TexC, Time, TexC);
			//	if (TexA[0] < 0) TexA[0] = 4096;
			//	if (TexA[1] < 0) TexA[1] = 4096;
			//	if (TexB[0] < 0) TexB[0] = 4096;
			//	if (TexB[1] < 0) TexB[1] = 4096;
			//	if (TexC[0] < 0) TexC[0] = 4096;
			//	if (TexC[1] < 0) TexC[1] = 4096;

				SetTexCoordToList((u32*)KokiriForestwater_bin, j	, TexA);
				SetTexCoordToList((u32*)KokiriForestwater_bin, j + 1, TexB);
				SetTexCoordToList((u32*)KokiriForestwater_bin, j + 2, TexC);
			//	if (SphereRayVsTriangle(Pos, Radius, RayDir, TriScale, &A, &B, &C, Normal, Dist)) return 34;
			}

			printf("\x1b[1;0H Render Fps: %d", MeasureFps(&RenderFrame));
		}
			
		/*
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);
		glPushMatrix();
		glTranslateV(SrcPos);
		glScalefx(F1, F1, F1);
		glBindTexture(0, 0);
		glCallList((u32*)Sphere);
		glPopMatrix(1);
		*/
	//	if (RenderMap)
	//		MiniMapRender();
		
	//	RendererDebugInfos();
	//	printf("\x1b[13;2H Player Col: %.1f %.1f %.1f", Normal[0].Float(), Normal[1].Float(), Normal[2].Float());
	//	printf("\x1b[13;2H Player Rot: %.1f %.1f", PlayerRot[0].Float(), PlayerRot[1].Float());
	//	printf("\x1b[14;2H Camera Dir: %.1f %.1f %.1f", CameraDir[0].Float(), CameraDir[1].Float(), CameraDir[2].Float());
	//	if (Frame == 1)
			glFlush(0);		
	//	swiWaitForVBlank();


		Frame++;

		if (Frame > 1)
			Frame = 0;
	//	swiWaitForVBlank();
	//	consoleClear();		

		Ticks += timerElapsed(0);
		if (Ticks >= INT32_MAX)
			Ticks = 0;
		Ticksf32 = Ticks;
		GetDeltaTime();		
		
		printf("\x1b[2;0H Global Fps: %d", MeasureFps(&GlobalFrame));
	//	printf("\x1b[19;2H Ticks: %d", Ticks);
	//	printf("\x1b[19;2H Dt int: %u", DeltaTimeI);
	}

	KokiriForestDeleteTextures();
	YoungLinkDeleteTextures();

	return 0;
}
