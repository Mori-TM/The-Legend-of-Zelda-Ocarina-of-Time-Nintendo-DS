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
#include "Camera.h"
#include "MiniMap.h"
#include "Md2Loader.h"

#include "Models/KokiriForest.h"
#include "Models/TempleOfTime.h"
//#include "Models/YoungLink.h"

#include "LinkTex_pcx.h"

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
	List[0] = ((Model->Model->NumTriangles * 3) - 2) * 5 + 11;
	List[1] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16);
	List[2] = GL_TRIANGLE;
	
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

	for (i = 1; i < Model->Model->NumTriangles - 1; i++)
	{
		Model->Update(i);
		
		CreatePack(0, List, &Off, &CmdOff, Model);
		CreatePack(2, List, &Off, &CmdOff, Model);
		CreatePack(1, List, &Off, &CmdOff, Model);
	}

	Model->Update(i);
	
	CreatePack(0, List, &Off, &CmdOff, Model);
	CreatePack(2, List, &Off, &CmdOff, Model);
	CreatePack(1, List, &Off, &CmdOff, Model);
}

FORCE_INLINE void Display()
{	
	f32 Z = 0.0;
	f32 O = 1.0;
	/*
	m4x4 PlayerRot = 
	{
		O.Fixed, 0, 0, 0,
		CameraDir.x.Fixed, CameraDir.y.Fixed, CameraDir.z.Fixed, 0,
		0, O.Fixed, 0, 0,
		0, 0, 0, O.Fixed
	};
	*/
/*
	m3x3 PlayerRot = 
	{
		CameraDir.x.Fixed, 0, CameraDir.z.Fixed,
		0, 1, 0,
		-CameraDir.z.Fixed, 0,  CameraDir.x.Fixed,
	};
	*/
/*
	m4x4 PlayerRot = 
	{
		O.Fixed, CameraDir.x.Fixed, 0, 0,
		0, CameraDir.y.Fixed, O.Fixed, 0,
		0, CameraDir.z.Fixed, 0, 0,
		0, 0, 0, O.Fixed
	};
*/
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);
	glEnable(GL_TEXTURE_2D);
/*
	vec3 Up = { 0.0, 1.0, 0.0 };
	vec3 Left = Cross3P(&PlayerDir, &Up);
	Normalize3P(&Left);

	Up = Cross3P(&Left, &PlayerDir);
	Normalize3P(&Up);

	m4x4 Matrix =
	{
		Left.x.Fixed, Left.y.Fixed, Left.z.Fixed, 0,     //LEFT
		Up.x.Fixed, Up.y.Fixed, Up.y.Fixed, 0,                       //UP 
		PlayerDir.x.Fixed, PlayerDir.y.Fixed, PlayerDir.z.Fixed, 0,  //FORWARD
		PlayerPos.x.Fixed, PlayerPos.y.Fixed, PlayerPos.z.Fixed, O.Fixed
	};
*/
/*
	m4x4 Matrix =
	{
		Left.x.Fixed, Up.x.Fixed, PlayerDir.x.Fixed, PlayerPos.x.Fixed,
		Left.y.Fixed, Up.y.Fixed, PlayerDir.y.Fixed, PlayerPos.y.Fixed,
		Left.z.Fixed, Up.y.Fixed, PlayerDir.z.Fixed, PlayerPos.z.Fixed,
		0, 0, 0, O.Fixed
	};
*/
	glPushMatrix();
		glTranslateV(&PlayerPos);
	//	glMultMatrix4x4(&Matrix);
		glRotatefx(-90.0f, 1.0f, 0.0f, 0.0f);
/*
		vec2 Dir0 = { 0.0, 1.0 };
		vec2 Dir = PlayerRot;
	//	Normalize2P(&Dir);
	//	f32 Angle = -acosf32(Dot2P(&Dir0, &Dir) / (sqrtf32(Dir.x * Dir.x + Dir.y * Dir.y) * sqrtf32(Dir0.x * Dir0.x + Dir0.y * Dir0.y)));
	//	f32 Angle = -sinf32(Dir.y / (sqrtf32(Dir.x * Dir.x + Dir.y * Dir.y))) * 180.0 / PI;
	//	f32 Angle = (atan2f32(Dir.x.Float(), Dir.y.Float()));
		f32 x1 = 1.0;
		f32 y1 = 0.0;
		f32 x2 = PlayerRot.x;
		f32 y2 = PlayerRot.y;
		f32 dot = x1*x2 + y1*y2;
		f32 det = x1*y2 - y1*x2;
	//	f32 Angle = atan2f32(det.Float(), dot.Float());
		f32 Angle = atan2f32((y1 - y2).Float(), (x1 - x2).Float());
	//	f32 Angle = asinf32(Length3P())
		glRotatefx(Angle, 0.0, 0.0, 1.0);
		*/
	//	vec3 Dir0 = PlayerPos;
	//	Dir0.y = 0.0;
	//	vec3 Dir1 = CameraDir;
	//	Dir1.y = 0.0;
	//	Dir0 = Add3P(&Dir0, &Dir1);
	//	gluLookAtV(&PlayerPos, &Dir0, &CameraUp);
	//	glRotatefx(-90.0f, 1.0f, 0.0f, 0.0f);
	//	glScalefx(8.0, 8.0, 8.0);
		
	//	glRotatefx(PlayerRotY + 90.0f, 0.0f, 1.0f, 0.0f);

		Md2::Proccess ProcModel;
		ProcModel.Model = &Model;
		ProcModel.Start(0, Model.NumFrames);
	//	ProcModel.Start(0, Model.NumFrames);
		DrawMd2(List, &ProcModel);
		ProcModel.End((DeltaTime * 3.74).Float());
		glBindTexture(GL_TEXTURE_2D, YoungLinkTextures[0]);
		glCallList((u32*)List);
	//	glScalefx(0.75f, 0.75f, 0.75f);
		
	glPopMatrix(1);

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);

	glPushMatrix();
		glTranslatefx(0.0f, -2.0f, 0.0f);
		glScalefx(100.0f, 100.0f, 100.0f);

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

	if (KeysDown & KEY_X) LinkDrawCount++;
	if (KeysDown & KEY_Y && LinkDrawCount > 0) LinkDrawCount--;
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

	timerStart(0, ClockDivider_1024, 0, NULL);

	f32 Time = 0.0f;
	while(1) 
	{	
		PollKeyEvents();
		
		if (UpdateTimer)
			Time += DeltaTime * 0.003f;

	//	if (Time >= DEGREES_IN_CIRCLEF32)
	//		Time = 0.0f;

		glLoadIdentity();

		LightPos = { -sinf32(Time) * 8.0f, -8.0f, -cosf32(Time) * 8.0f };	
		Normalize3P(&LightPos);

	//	glLight(0, RGB15(floattov10(LightColor.x.Float()), floattov10(LightColor.y.Float()), floattov10(LightColor.z.Float())), floattov10(LightPos.x.Float()), floattov10(LightPos.y.Float()), floattov10(LightPos.z.Float()));
	//	LightPos =
		RendererSetLight(0, LightColor, LightPos);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspectivefx(-70.0f, 1.333333333333333f, 0.1f, 1000.0f);
	//	UpdateCamera();
	//	UpdateLink();
		UpdateCamera();
		glMatrixMode(GL_MODELVIEW);		
		Display();
		
		if (RenderMap)
			MiniMapRender();
		
		RendererDebugInfos();
		printf("\x1b[13;2H Player Rot: %.1f %.1f", PlayerRot.x.Float(), PlayerRot.y.Float());
		printf("\x1b[14;2H Camera Dir: %.1f %.1f %.1f", CameraDir.x.Float(), CameraDir.y.Float(), CameraDir.z.Float());
		glFlush(0);		
		Ticks += timerElapsed(0);
		if (Ticks >= INT32_MAX)
			Ticks = 0;
		Ticksf32 = (float)Ticks;
		GetDeltaTime();
	}

	TempleofTimeDeleteTextures();
	KokiriForestDeleteTextures();
	YoungLinkDeleteTextures();

	return 0;
}
