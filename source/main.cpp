#define FORCE_INLINE __attribute__((always_inline)) inline
#include <nds.h>
#include <maxmod9.h>
#include <time.h>
#include <stdio.h>

#include "Texkokiri_forest_minimap_pcx.h"
#include "LinkTex_pcx.h"

#include "Fixed.h"
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
vec3 LightColor = { 1.0f, 0.449f, 0.001f };

u32 LinkDrawCount = 0;
bool RenderMap = true;

FORCE_INLINE void Display()
{	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	//	glTranslatefx(CameraLookAt.x, CameraLookAt.y - 3, CameraLookAt.z);
		glTranslateV(&LinkPos);
	//	glRotatefx(LinkRotationY + 180.0f, 0.0f, 1.0f, 0.0f);
		glRotatefx(LinkRot.y + 90.0f, 0.0f, 1.0f, 0.0f);
		glScalefx(0.75f, 0.75f, 0.75f);
		
		YoungLinkDraw();

		for (u32 i = 0; i < LinkDrawCount; i++)
		{
			glTranslatefx(2.0f, 0.0f, 0.0f);
			YoungLinkDraw();
		}
		
	glPopMatrix(1);

//	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | POLY_FORMAT_LIGHT0);

	glPushMatrix();
		glTranslatefx(0.0f, -2.0f, 0.0f);
		glScalefx(100.0f, 100.0f, 100.0f);

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
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0f / 192.0f, 0.1f, 1000.0f);
	gluLookAtfx(0.0f, 2.0f, 4.5,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
			 */ 
	glLight(0, RGB15(31, 0, 0), 0, floattov10(-1.0f), 0);
	glLight(1, RGB15(0, 31, 0), floattov10(1.0f), 0, 0);
	glLight(2, RGB15(0, 0, 31), floattov10(-1.0f), 0, 0);

	glMaterialf(GL_AMBIENT, RGB15(1, 1, 1));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialf(GL_EMISSION, RGB15(16, 16, 16));
	glMaterialShinyness();

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0);  
	
//	glMatrixMode(GL_MODELVIEW);
	
	EngineInitAudio();
	EngineLoadAudio(MOD_ZELDA);
	EnginePlayAudio(MOD_ZELDA);

	f32 x = 22.0f;

	f32 Time = 0.0f;
	while(1) 
	{	
		PollKeyEvents();

		if (RotateLight)
			Time+= 1.0f;
		glLoadIdentity();

		vec3 LightPos = { -sinf32(Time) * 10.0f, -8.0f, -cosf32(Time) * 8.0f };
		Normalize3P(&LightPos);
		glLight(0, RGB15(floattov10(LightColor.x.Float()), floattov10(LightColor.y.Float()), floattov10(LightColor.z.Float())), floattov10(LightPos.x.Float()), floattov10(LightPos.y.Float()), floattov10(LightPos.z.Float()));

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspectivefx(-70.0f, 1.333333333333333f, 0.1f, 1000.0f);
		//gluPerspective(70.0f, 256.0f / 192.0f, 0.1f, 1000.0f);
		UpdateCamera();
		UpdateLink();
	//	gluLookAtfx(0.0f, 2.0f, 4.5,
	//			0.0f, 0.0f, 0.0f,
	//			0.0f, 1.0f, 0.0f);
		glMatrixMode(GL_MODELVIEW);		
	//	glLoadIdentity();
	//	Camera();
		Display();

	//	glLight(0, RGB15(31, 0, 0), 0, floattov10(-1.0f), 0);
	//	glLight(1, RGB15(0, 10, 0), floattov10(1.0f), 0, 0);
	//	glLight(2, RGB15(0, 0, 10), floattov10(-1.0f), 0, 0);
		/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspectivefx(70, f32(256.0f) / f32(192.0f), 0.1, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		Camera();
		Display();
		*/
		
		
		if (RenderMap)
			/*
		{
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrthofx(0.0f, 255.0f, 191.0f, 0.0f, -10.0f, 10.0f);
			glMatrixMode(GL_MODELVIEW);
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
			gluLookAtfx(0.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f);
			glPushMatrix();
			
			glScalefx(9.6f, 6.2f, 2.0f);
			glColor3f(0.2, 1.0f, 0.2f);
			glBindTexture(GL_TEXTURE_2D, Textures[0]);
			glBegin(GL_QUADS);
			{			
				glTexCoordfx(1.0f, 1.0f);
				glVertexfx(0.0f, 0.0f, 0.0f);
				
				glTexCoordfx(0.0f, 1.0f);
				glVertexfx(10.0f, 0.0f, 0.0f);

				glTexCoordfx(0.0f, 0.0f);
				glVertexfx(10.0f, 10.0f, 0.0f);
				
				
				glTexCoordfx(1.0f, 0.0f);
				glVertexfx(0.0f, 10.0f, 0.0f);
			}
			glEnd();
			glPopMatrix(1);
		}	
		*/
		{
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrthofx(0.0f, 255.0f, 191.0f, 0.0f, -10.0f, 10.0f);
			glMatrixMode(GL_MODELVIEW);
			glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE);
			gluLookAtfx(0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f);
			glPushMatrix();

			glScalefx(9.6f, 6.2f, 2.0f);
			glColor3f(0.2, 1.0f, 0.2f);
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
		printf("\x1b[3;2H Light Dir: %.1f %.1f %.1f", LightPos.x.Float(), LightPos.y.Float(), LightPos.z.Float());
		printf("\x1b[4;1H Light Color: %.2f %.2f %.2f", LightColor.x.Float(), LightColor.y.Float(), LightColor.z.Float());
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
