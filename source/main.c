#define FORCE_INLINE static __attribute__((always_inline)) inline
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

#include "Texkokiri_forest_minimap_pcx.h"
#include "LinkTex_pcx.h"

#include "Math3D.h"
#include "Helper.h"
#include "Audio.h"
#include "Camera.h"

#include "Models/KokiriForest.h"
#include "Models/TempleOfTime.h"
#include "Models/YoungLink.h"

int Textures[2];

//int LinkRotationY = 0;

int SceneToRender = 0;
bool RotateLight = true;
vec3 LightColor = { 1.0, 0.449, 0.001 };

u32 LinkDrawCount = 0;
bool RenderMap = false;

FORCE_INLINE void Display()
{	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	//	glTranslatef(CameraLookAt.x, CameraLookAt.y - 3, CameraLookAt.z);
		glTranslateV(&LinkPos);
	//	glRotatef(LinkRotationY + 180.0, 0.0, 1.0, 0.0);
		glRotatef(LinkRot.y + 90.0, 0.0, 1.0, 0.0);
		glScalef(0.75, 0.75, 0.75);
		
		YoungLinkDraw();

		for (u32 i = 0; i < LinkDrawCount; i++)
		{
			glTranslatef(2.0, 0.0, 0.0);
			YoungLinkDraw();
		}
		
	glPopMatrix(1);

//	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);

	glPushMatrix();
		glTranslatef(0.0, -2.0, 0.0);
		glScalef(100.0, 100.0, 100.0);

		if (SceneToRender == 0)
			TempleofTimeDraw();
		else if (SceneToRender == 1)
			KokiriForestDraw();
	glPopMatrix(1);
}
/*
void SmoothLinkRotation(int rx)
{
	if (LinkRotationY - 15 <= -rx && LinkRotationY + 15 >= -rx)
		LinkRotationY = -rx;
	else
	{
		if (LinkRotationY <= -rx)
		{
			LinkRotationY += 10;
		}
		if (LinkRotationY >= -rx)
		{
			LinkRotationY -= 10;
		}
	}
}
*/
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
	if (KeysDown & KEY_B) RotateLight = !RotateLight;

	if (KeysDown & KEY_X) LinkDrawCount++;
	if (KeysDown & KEY_Y && LinkDrawCount > 0) LinkDrawCount--;
	if (KeysDown & KEY_SELECT) RenderMap = !RenderMap;

	if (KeysHeld & KEY_START) exit(0);
}


int vrc;
int prc;

FORCE_INLINE void Camera()
{
	

/*
	u16 Keys = keysHeld();
	if (Keys & KEY_UP) 
	{
		SmoothLinkRotation(rx);
	}
	if (Keys & KEY_DOWN) 
	{
		rx -= 180;
		SmoothLinkRotation(rx);
		rx += 180;
	}
	if (Keys & KEY_LEFT) 
	{
		rx -= 90;
		SmoothLinkRotation(rx);
		rx += 90;
	}
	if (Keys & KEY_RIGHT) 
	{
		rx += 90;
		SmoothLinkRotation(rx);
		rx -= 90;
	}
*/
	
}

int main() 
{	
	videoSetMode(MODE_0_3D);
	glInit();

	consoleDemoInit();	

	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankD(VRAM_D_TEXTURE);
	
//	LoadAlpha = true;
	Textures[0] = LoadTexture(TEXTURE_SIZE_64, (u8*)Texkokiri_forest_minimap_pcx);
	YoungLinkLoadTextures();
	TempleofTimeLoadTextures();
		
	glEnable(GL_ANTIALIAS);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POLY_OVERFLOW);

	glClearColor(227, 215, 247, 31);
	glClearPolyID(63);
	glClearDepth(0x7FFF);
	
	glViewport(0, 0, 255, 191);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 1000.0);
	gluLookAt(0.0, 2.0, 4.5,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
			  
	glLight(0, RGB15(31, 0, 0), 0, floattov10(-1.0), 0);
	glLight(1, RGB15(0, 31, 0), floattov10(1.0), 0, 0);
	glLight(2, RGB15(0, 0, 31), floattov10(-1.0), 0, 0);

	glMaterialf(GL_AMBIENT, RGB15(1, 1, 1));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialf(GL_EMISSION, RGB15(16, 16, 16));
	glMaterialShinyness();

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);  
	
	glMatrixMode(GL_MODELVIEW);
	
	EngineInitAudio();
	EngineLoadAudio(MOD_ZELDA);
//	EnginePlayAudio(MOD_ZELDA);

	f32 x = 22;

	unsigned int Time = 0;
	while(1) 
	{	
		PollKeyEvents();

		if (RotateLight)
			Time++;
		glLoadIdentity();

		vec3 LightPos = { -(10.0 * SinF(Time)), -8.0, -(8.0 * CosF(Time)) };
		Normalize3P(&LightPos);
		glLight(0, RGB15(floattov10(LightColor.x), floattov10(LightColor.y), floattov10(LightColor.z)), floattov10(LightPos.x), floattov10(LightPos.y), floattov10(LightPos.z));	

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, 256.0 / 192.0, 0.1, 1000.0);
		UpdateCamera();
		UpdateLink();
		glMatrixMode(GL_MODELVIEW);		
	//	Camera();
		Display();

	//	glLight(0, RGB15(31, 0, 0), 0, floattov10(-1.0), 0);
	//	glLight(1, RGB15(0, 10, 0), floattov10(1.0), 0, 0);
	//	glLight(2, RGB15(0, 0, 10), floattov10(-1.0), 0, 0);
		/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, 256.0 / 192.0, 0.1, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		Camera();
		Display();
		*/

		if (RenderMap)
		{
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, 255, 191, 0, -10.0, 10.0);
			glMatrixMode(GL_MODELVIEW);
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
			gluLookAt(0.0, 0.0, 1.0,
					0.0, 0.0, 0.0,
					0.0, 1.0, 0.0);
			glPushMatrix();
			
			glScalef(9.6, 6.2, 2.0);
			glColor3f(0.2, 1.0, 0.2);
			glBindTexture(GL_TEXTURE_2D, Textures[0]);
			glBegin(GL_QUADS);
			{			
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.0, 0.0, 0.0);
				
				glTexCoord2f(0.0, 1.0);
				glVertex3f(10.0, 0.0, 0.0);

				glTexCoord2f(0.0, 0.0);
				glVertex3f(10.0, 10.0, 0.0);
				
				
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.0, 10.0, 0.0);
			}
			glEnd();
			glPopMatrix(1);
		}		
		
		glGetInt(GL_GET_VERTEX_RAM_COUNT, &vrc);
		glGetInt(GL_GET_POLYGON_RAM_COUNT, &prc);
		
		printf("\x1b[1;2H Vertices in Ram: %i/6144", vrc);
		printf("\x1b[2;2H Triangles in Ram: %i/2048", prc);
		printf("\x1b[3;2H Light Dir: %.1f %.1f %.1f", LightPos.x, LightPos.y, LightPos.z);
		printf("\x1b[4;1H Light Color: %.2f %.2f %.2f", LightColor.x, LightColor.y, LightColor.z);
		printf("\x1b[5;3H Battery: %ld%%", getBatteryLevel());
		if (SceneToRender == 0)
			printf("\x1b[6;3H Scene: Temple of Time");
		else if (SceneToRender == 1)
			printf("\x1b[6;3H Scene: Kokiri Forest");

		printf("\x1b[8;1H A = chnage scene");
		printf("\x1b[9;1H B = rotate light");
		printf("\x1b[10;1H X/Y = add/sub Links");
		printf("\x1b[11;1H select = map");

		glFlush(0);		
	}

	TempleofTimeDeleteTextures();
	KokiriForestDeleteTextures();
	YoungLinkDeleteTextures();

	return 0;
}
