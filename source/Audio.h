#include "soundbank.h"
#include "soundbank_bin.h"

FORCE_INLINE void EngineInitAudio()
{
    mmInitDefaultMem((mm_addr)soundbank_bin);
}

FORCE_INLINE void EngineLoadAudio(mm_word module_ID)
{
	mmLoad(module_ID);
}

FORCE_INLINE void EnginePlayAudio(mm_word module_ID)
{
    mmStart(module_ID, MM_PLAY_LOOP);
}