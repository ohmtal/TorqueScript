//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// SDL3 Audio
// since i need to manage the WavDate in a struct i do NOT implement it with
// nativ commands
//-----------------------------------------------------------------------------
#pragma once
#include <SDL3/SDL.h>
#include "resourceManager/ElfResource.h"


namespace ElfSDL3 {

namespace Audio{
    struct WavData {
        Uint8* buffer = nullptr;
        Uint32 len = 0;
        SDL_AudioStream* stream = nullptr;
        SDL_AudioSpec spec;
    };

   struct Tone {
        S32 midiNote;
        F32 duration;
        F32 amplitute = 0.2f;
   };

   struct ToneEntry {
        S32 LineNumber = 0;
        Tone tone = {0};
   };

   typedef Vector<ToneEntry> Melody;
   inline void UnloadMelody(Melody) {}
   inline ElfResource::ElfStorage<Melody , UnloadMelody>  MelodyMap;
   // -----------------
   void UnloadWavData(WavData wavData);
    inline ElfResource::ElfStorage<WavData , UnloadWavData>  WaveDataMap;


    bool Init();
    void ShutDown();
}



}

