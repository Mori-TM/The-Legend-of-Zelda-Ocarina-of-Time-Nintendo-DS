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

FORCE_INLINE void AudioPlay(mm_word module_ID)
{
    mmStart(module_ID, MM_PLAY_LOOP);
}