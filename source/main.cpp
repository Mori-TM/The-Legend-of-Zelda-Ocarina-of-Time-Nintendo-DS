#define FORCE_INLINE __attribute__((always_inline)) inline
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

#include "Texkokiri_forest_minimap_pcx.h"

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

#include "Models/KokiriForest.h"
#include "Models/TempleOfTime.h"
#include "Models/YoungLink.h"

FORCE_INLINE void Display()
{	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glTranslateV(&PlayerPos);
		glRotatefx(PlayerRotY + 90.0f, 0.0f, 1.0f, 0.0f);
	//	glScalefx(0.75f, 0.75f, 0.75f);
		
		YoungLinkDraw();

		for (int i = 0; i < LinkDrawCount; i++)
		{
			glTranslatefx(2.0f, 0.0f, 0.0f);
			YoungLinkDraw();
		}
		
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
	
	#define VERTICES 2088
	vec3 OGPoses[VERTICES];
	for (int i = 0; i < VERTICES; i += 3)
	{
		OGPoses[i] = GetVertexFromList(YoungLink, i);
		OGPoses[i + 1] = GetVertexFromList(YoungLink, i + 1);
		OGPoses[i + 2] = GetVertexFromList(YoungLink, i + 2);
	}

	cpuStartTiming(0);

	f32 Time = 0.0f;
	while(1) 
	{	
		PollKeyEvents();
		
		if (UpdateTimer)
			Time += DeltaTime * 0.0025f;

	//	if (Time >= DEGREES_IN_CIRCLEF32)
	//		Time = 0.0f;

		f32 Sin = sinf32(Time);

	//	for (int i = 0; i < VERTICES; i += 3)
	//	{
	//		vec3 NewPos0 = OGPoses[i];
	//		vec3 NewPos1 = OGPoses[i + 1];
	//		vec3 NewPos2 = OGPoses[i + 2];
	//
	//		vec3 Normal = GetNormalFromList(YoungLink, i);
	//		vec3 Dir = Add3(Normal, Mul3(Normal, Vec3f(Sin)));
	//		
	//		NewPos0 = Add3(Dir, NewPos0);
	//		NewPos1 = Add3(Dir, NewPos1);
	//		NewPos2 = Add3(Dir, NewPos2);
	//
	//
	//		SetVertexToList(YoungLink, i, NewPos0);
	//		SetVertexToList(YoungLink, i + 1, NewPos1);
	//		SetVertexToList(YoungLink, i + 2, NewPos2);
	//	}

		glLoadIdentity();

		LightPos = { -Sin * 10.0f, -8.0f, -cosf32(Time) * 8.0f };
		Normalize3P(&LightPos);
	//	glLight(0, RGB15(floattov10(LightColor.x.Float()), floattov10(LightColor.y.Float()), floattov10(LightColor.z.Float())), floattov10(LightPos.x.Float()), floattov10(LightPos.y.Float()), floattov10(LightPos.z.Float()));
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

		glFlush(0);		
		Ticks = cpuGetTiming();
		GetDeltaTime();
	}

	TempleofTimeDeleteTextures();
	KokiriForestDeleteTextures();
	YoungLinkDeleteTextures();

	return 0;
}
