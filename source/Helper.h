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

bool LoadAlpha = false;
bool LoadMirrored = false;
int LoadTexture(int Size, u8* Image)
{
	int Texture;
	
	sImage pcx;
	loadPCX((u8*)Image, &pcx);
	if (LoadAlpha)
		image8to16trans(&pcx, 0);
	else
		image8to16(&pcx);
	
	glGenTextures(1, &Texture);
	glBindTexture(0, Texture);

	int Para;
	if (LoadMirrored)
		Para = TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_FLIP_S | GL_TEXTURE_WRAP_T | GL_TEXTURE_FLIP_T;
	else
		Para = TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T;
		

	if (LoadAlpha)
		glTexImage2D(0, 0, GL_RGBA, Size, Size, 0, Para, pcx.image.data8);
	else
		glTexImage2D(0, 0, GL_RGB, Size, Size, 0, Para, pcx.image.data8);
	imageDestroy(&pcx);

	LoadAlpha = false;
	LoadMirrored = false;

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