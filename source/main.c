//For syntax highlighting
//#define ARM9
#define FORCE_INLINE __attribute__((always_inline)) inline
#define PI 12868
//#define NO_HARDWARE_ACCELERATION
//#define NDEBUG
#define USE_INSTUCTION_CHACHE
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

#include <fat.h>
#include <sys/dir.h>
#include <dirent.h>
#include <unistd.h>

#include "FileSystem.h"
#include "glTexImage.h"
#include "Fixed.h"
#include "Math3D.h"
#include "Globals.h"
#include "Helper.h"
#include "glFixed.h"
#include "DisplayList.h"
#include "64ToDS.h"

#include "Models/KokiriForest.h"
//#include "Models/Sphere.h"


#include "Renderer.h"
#include "Audio.h"
#include "MiniMap.h"
#include "Md2Loader.h"
#include "Collision.h"
#include "Camera.h"
#include "Player.h"
#include "Animation.h"

/*
To-Do
-subdived ramp at deku tree
-split forest mesh more?
-implement wall collision
-clean up collision mesh
-remove overlapping triangles in render mesh
-fix weird alpha texture issue on ds hardware - done!
-Use 4/16 color textures too not only 256
-combine texture palettes

Infos for me
-If ramp triangle collision doesn't work 100% just subdived it
-collision model needs to be half the size of the render model, 
 because collision only uses 16 bit fixed for data storing
-16 color textures need a min res of 128x128-dound out by try and error; This was a cache error by libnds

Some more:
VRAM_C_SUB_BG == GX_VRAM_CLEARDEPTH_128_C

GX_SetBankForClearImage(GX_VRAM_CLEARDEPTH_128_C);
GX_BeginLoadClearImage();
{
	GX_LoadClearImageDepth((void *)DEPTH_VRAM256x192,       // src
						   sizeof(DEPTH_VRAM256x192));      // size
}
GX_EndLoadClearImage();

*/






//f32 AnimationSpeed = 15319;//3,74
f32 AnimationSpeed = 3319;//3,74

FORCE_INLINE void Display()
{	
	
//	vec3 SrcPos;
//	Vec3(-50899, 945, -46252, SrcPos);

//	glEnable(GL_TEXTURE_2D);

//	glPolyFmt(POLY_ALPHA(25) | POLY_CULL_NONE);

	
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


	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1| POLY_FOG);

	glPushMatrix();		
	//	vec3 r = { 0, 0, F1 };
		vec3 Up = { 0, F1, 0 };
		
		vec3 v0; Copy3(PlayerPos, v0);
		vec3 v1; Copy3(CameraPos, v1);
		vec3 r;
		Sub3(v0, v1, r);
		r[1] = 0;
		Normalize3(r);
		  
		vec3 Dir = { -r[0], 0, -r[2] };
		//Add3(PlayerPos, PlayerDir, r)
		
	//	glScalefx(2548, 2548, 2548);
	//	gluLookAtV(PlayerPos, Add3(PlayerPos, Dir, r), Up);
	//	glRotatefx(-F90, F1, F0, F0);
		
		glTranslateV(PlayerPos);
		glScalefx(2548, 2548, 2548);
		glRotatefx(-F90, F1, F0, F0);
	//	glRotatefx(atan2f32(PlayerDir[0], PlayerDir[2]), F0, F0, F1);
		
		Md2Proceess.Model = &PlayerModel;
	//	Md2Start(1, 20);//20
	//	Md2Start(20, 38);
		Md2StartBlend(21, 40, 10 , 1, 20);
//		Md2StartBlend(1, 20, 10, 21, 40);
	//	Md2StartBlend(1, 20, 10, 28, 20, 38);
		AnimationUpdateList(PlayerDisplayList, PlayerModel.NumTriangles, 40, 10);
		
		Md2End(Mul(DeltaTime, AnimationSpeed));
		glBindTexture(GL_TEXTURE_2D, YoungLinkTextures[0]);
		glCallList((u32*)PlayerDisplayList);

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

bool RequestTextureReload = false;

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
//	if (KeysDown & KEY_B) UpdateTimer = !UpdateTimer;
	if (KeysDown & KEY_B) RequestTextureReload = true;
	if (KeysDown & KEY_L) 
	{
	//	LastTexture = 0;
	//	TextureCount = 0;
	//	TextureCountMax--;
		RequestTextureReload = true;
	}
	if (KeysDown & KEY_R)
	{
	//	LastTexture = 0;
	//	TextureCount = 0;
	//	TextureCountMax++;
		RequestTextureReload = true;
	}
	
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



void ReloadTextures()
{
	swiWaitForVBlank();
	while (GFX_BUSY);

	glResetTextures();
//	free(KokiriForestCollisionModel);
//	YoungLinkDeleteTextures();
//	KokiriForestDeleteTextures();

	YoungLinkLoadTextures();
	KokiriForestLoadTextures();

	RequestTextureReload = false;
}

int main(int Argc, char** Argv) 
{	
//	fatInit(32, true);
	
	consoleDemoInit();
	
	for (int i = 0; i < Argc; i++)
		printf("%s\n", Argv[i]);
	
	ArgDef = Argv[0];

	if (!InitFileSystem(Argc, Argv))
	{
		printf("Failed to init FS\n");
		printf("If you use MelonDS, make sure you enable DLDI in \"Config -> Emu settings\" under \"DLDI\" \"Eanable DLDI\"\n");
		while(1);
	}
	
	RendererInit();	
	
	sysSetCartOwner(BUS_OWNER_ARM9);

	printf("Hey\n");
	printf("\n");

//	MiniMapInit();

	printf("Be patient, Models and Textures now get loaded from storage!\n\n");
	YoungLinkLoadTextures();
	KokiriForestLoadTextures();
	KokiriForestLoadModels();	
	
	AudioInit();
	AudioLoadWav(SFX_MAINTHEME);
	AudioLoadWav(SFX_RIVER);
	AudioHandle MainHandle = AudioPlayWav(SFX_MAINTHEME, 200);
	AudioHandle RiverHandle = AudioPlayWav(SFX_RIVER, 200);
	
	YoungLinkInit();

	timerStart(0, ClockDivider_1024, 0, NULL);
	int Frame = 0;

	f32 Time = 0;
	
	consoleClear();
	
	while (1) 
	{	
		if (RequestTextureReload)
			ReloadTextures();
	
	//	MiniMapRender();

		Copy3(PlayerPos, PlayerPosLast);
		PollKeyEvents();		

		if (UpdateTimer)
			Time += Mul(DeltaTime, LIGHT_DELAY);
			
		vec3 SrcPos;

		Vec3(Mul(-sinf32(Time), F5), F5, Mul(-cosf32(Time), F5), SrcPos);
		AudioSet3D(SrcPos, F100, 10, 200, CameraPos, CameraDir, MainHandle);
		
		Vec3(-74023, 849, -103141, SrcPos);
		AudioSet3D(SrcPos, F50, 10, 200, CameraPos, CameraDir, RiverHandle);
		
		
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
	//	if (0 == 1)
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
			
			Vec3(-50899, 945, -46252, SrcPos);
			vec3 Color = { 0, 4096, 0 };
			RendererSetPointLight(1, Color, SrcPos, PlayerPos, F10);

			Display();

			vec2 Time = { 1, 1 };
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
		
		u16 TimeElapsed = timerElapsed(0);
		Ticks += TimeElapsed;
		if (Ticks >= INT32_MAX)
			Ticks = 0;
			
			
		Ticksf32 += TimeElapsed * 30;		
		if (Ticksf32 >= INT32_MAX)
			Ticksf32 = 0;
		
		GetDeltaTime();		
		
		printf("\x1b[2;0H Global Fps: %d", MeasureFps(&GlobalFrame));
		printf("\x1b[4;0H Demo by Moritz Gooth");
		printf("\x1b[5;0H Youtube: Mori TM");
		printf("\x1b[6;0H I hope you like it :)");
		
		RendererDebugInfos();
	//	printf("\x1b[19;2H Ticks: %d", Ticks);
	//	printf("\x1b[19;2H Dt int: %u", DeltaTimeI);
	}

	KokiriForestDeleteTextures();

	YoungLinkDestroy();

	return 0;
}
