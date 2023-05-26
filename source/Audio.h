#include "soundbank.h"
#include "soundbank_bin.h"

FORCE_INLINE void AudioInit()
{
    mmInitDefaultMem((mm_addr)soundbank_bin);
}

FORCE_INLINE void AudioLoad(mm_word module_ID)
{
	mmLoad(module_ID);
}

FORCE_INLINE void AudioLoadWav(mm_word module_ID)
{
	mmLoadEffect(module_ID);
}

FORCE_INLINE void AudioPlay(mm_word module_ID)
{
    mmStart(module_ID, MM_PLAY_LOOP);
}

typedef struct
{
    mm_sound_effect Effect;
    mm_sfxhand Handle;
    u8 Panning;
} AudioHandle;

FORCE_INLINE AudioHandle AudioPlayWav(mm_word module_ID, u8 Volume)
{
    mm_sound_effect Effect = 
    {
		{ module_ID } ,			// id
		1024,	// rate
		0,		// handle
		Volume,	// volume
		128,		// panning
	};
    
    AudioHandle Handle;
    Handle.Effect = Effect;
    Handle.Handle = mmEffectEx(&Handle.Effect);
    Handle.Panning = 128;
    
    return Handle;
}

void AudioSet3D(vec3 SrcPos, f32 MaxDist, u8 MinVolume, u32 MaxVolume, vec3 DstPos, vec3 DstDir, AudioHandle Handle)
{
    MaxVolume *= 4096;
    f32 Dist = GetDistanceVec3(SrcPos, DstPos);
    
    u8 Volume = 255;
    if (Dist >= MaxDist)
    {
        if (Handle.Panning > 128)
            Handle.Panning--;
        if (Handle.Panning < 128)
            Handle.Panning++;
        
        mmEffectVolume(Handle.Handle, MinVolume);
        mmEffectPanning(Handle.Handle, Handle.Panning);
        return;
    }
    else
    {
        f32 x = Div(Dist, MaxDist);
        x = MaxVolume - Mul(x, MaxVolume);//255
        Volume= x / 4096;
        if (Volume < MinVolume)
            Volume = MinVolume;
    }
    
    mmEffectVolume(Handle.Handle, Volume);
    
    s16 Panning = 128;//Center
    
    vec3 Up = { F0, F1, F0 };
    vec3 Side; Cross3(Up, DstDir, Side);
    Normalize3(Side);
    vec3 t;
    f32 x = Dot3(Sub3(SrcPos, DstPos, t), Side);
    x = Div(x, Dist);    
    x = 524288 + Mul(x, 524288);
    Panning = x / 4096;
    if (Panning < 10)
        Panning = 10;
    if (Panning >= 245)
        Panning = 245;
    
    Handle.Panning = Panning;
    mmEffectPanning(Handle.Handle, Panning);
}