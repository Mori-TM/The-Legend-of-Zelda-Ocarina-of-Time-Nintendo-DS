u32 KeysDown;
u32 KeysHeld;
touchPosition Touch;

FORCE_INLINE void PenDelta(int *dx, int *dy) 
{
	static int prev_pen[2] = { 0x7FFFFFFF, 0x7FFFFFFF };

	if (KeysHeld & KEY_TOUCH) 
	{
		touchRead(&Touch);
		if (prev_pen[0] != 0x7FFFFFFF) 
		{
			*dx = (prev_pen[0] - Touch.rawx);
			*dy = (prev_pen[1] - Touch.rawy);
		} 
		else 
		{
			*dx = *dy = 0;
		}

		prev_pen[0] = Touch.rawx;
		prev_pen[1] = Touch.rawy;
	} 
	else 
	{
		prev_pen[0] = prev_pen[1] = 0x7FFFFFFF;
		*dx = *dy = 0;
	}
}

void GetGLSizeFromNormal(u16* Size)
{
	switch (*Size)
	{
	case 8:
		*Size = 0;
		break;
	
	case 16:
		*Size = 1;
		break;

	case 32:
		*Size = 2;
		break;

	case 64:
		*Size = 3;
		break;

	case 128:
		*Size = 4;
		break;

	case 256:
		*Size = 5;
		break;

	case 512:
		*Size = 6;
		break;

	case 1024:
		*Size = 7;
		break;
	
	default:
		*Size = 0;
		break;
	}
}

#include "PCXLoader.h"

bool LoadAlpha = false;
bool LoadMirrored = false;
bool LoadDirect = false;
int LoadTexture(u8* Image)
{
	int Texture = -1;
	PCXHeader* Header = (PCXHeader*)(Image);
	
	sImage PCX;
	
	u8 PaletteType;
	if (!LoadPCX((u8*)Image, &PCX, &PaletteType))
		goto TextureLoadError;

	glGenTextures(1, &Texture);
	glBindTexture(0, Texture);

	u16 Width = Header->xmax - Header->xmin + 1;
	u16 Height = Header->ymax - Header->ymin + 1;
	GetGLSizeFromNormal(&Width);
	GetGLSizeFromNormal(&Height);

	int Para;
	if (LoadMirrored)
		Para = TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_FLIP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_FLIP_T;
	else
		Para = TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T;
		

	if (LoadAlpha)
		glTexImage2D(0, 0, GL_RGB256, Width, Height, 0, Para | GL_TEXTURE_COLOR0_TRANSPARENT, PCX.image.data8);
	else
		glTexImage2D(0, 0, PaletteType == IMAGE_PALETTE_16 ? GL_RGB16 : GL_RGB256, Width, Height, 0, Para, PCX.image.data8);
	
	if (PaletteType != IMAGE_PALETTE_NONE)
		glColorTableEXT( 0, 0, PaletteType == IMAGE_PALETTE_16 ? 16 : 256, 0, 0, PCX.palette);

	imageDestroy(&PCX);

TextureLoadError:
	LoadAlpha = false;
	LoadMirrored = false;
	LoadDirect = false;

	return Texture;
}


f32 DeltaTime = 0;
f32 LastTime = 0;
u32 DeltaTimeI = 0;
u32 LastTimeI = 0;

f32 GetDeltaTime()
{
	f32 CurrentTime = Ticks;
	DeltaTime = CurrentTime - LastTime;
	LastTime = CurrentTime;

	u16 CurrentTimeI = Ticks;
	DeltaTimeI = CurrentTimeI - LastTimeI;
	LastTimeI = CurrentTimeI;

	return DeltaTime;
}

//Fps
typedef struct
{
	int Frame;
	int FinalTime;
	int InitTime;
	int FinalFps;
} FrameMeasureData;
/*
int Frame = 0;
int FinalTime;
int InitTime = 0;
int FinalFps = 0;
*/

FrameMeasureData RenderFrame = { 0, 0, 0, 0 };
FrameMeasureData CollisionFrame = { 0, 0, 0, 0 };
FrameMeasureData GlobalFrame = { 0, 0, 0, 0 };

#define TIMER_SPEED (BUS_CLOCK/1024)

int MeasureFps(FrameMeasureData* Data)
{
	Data->Frame++;
	Data->FinalTime = Ticks / TIMER_SPEED;
	if (Data->FinalTime - Data->InitTime > 0)
	{
		Data->FinalFps = Data->Frame / (Data->FinalTime - Data->InitTime);
		Data->Frame = 0;
		Data->InitTime = Data->FinalTime;
	}
	
	return Data->FinalFps;
}