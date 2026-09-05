//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ELFSDL Audio - Only Wav - the sdl limit
//-----------------------------------------------------------------------------
#include "console/scriptPreprocessor.h"
#include "console/engineAPI.h"

#include "SDL3_audio.h"
#include <math/mMathFn.h>
#include <console/script.h>


namespace ElfSDL3
{

namespace Audio {
    //--------------------------------------------------------------------------
    SDL_AudioDeviceID AudioDevice = 0;

    bool isInitialized() { return AudioDevice != 0 ;}
    //--------------------------------------------------------------------------
    bool bindStream(SDL_AudioStream* stream)
    {
        if (!isInitialized() || !stream) return false;
        SDL_AudioDeviceID currentDevice = SDL_GetAudioStreamDevice(stream);

        if (currentDevice == AudioDevice) {
            return true;
        }
        if (currentDevice != 0) {
            SDL_UnbindAudioStream(stream);
        }
        return SDL_BindAudioStream(AudioDevice, stream);
    }
    //--------------------------------------------------------------------------
    bool unBindStream(SDL_AudioStream* stream)
    {
        if (!isInitialized() || !stream) return false;
        SDL_UnbindAudioStream( stream);
        return (SDL_GetAudioStreamDevice(stream) == 0);
    }
    //--------------------------------------------------------------------------
    S32 loadWav(const char* fileName)
    {
        if (!isInitialized() || !fileName || dStrlen(fileName) == 0) return 0;


        WavData wavData;
        if (!SDL_LoadWAV(fileName, &wavData.spec, &wavData.buffer, &wavData.len)) {
            Con::errorf("Couldn't load Wavefile:%s file: %s", fileName, SDL_GetError());
            return 0;
        }
        wavData.stream = SDL_CreateAudioStream(&wavData.spec, nullptr);
        if (!wavData.stream ) {
            Con::errorf("Couldn't create audio stream: %s", SDL_GetError());
            return 0;
        }

        if (!bindStream(wavData.stream )) {
            SDL_Log("Failed to bind '%s' stream to device: %s", fileName, SDL_GetError());
            SDL_DestroyAudioStream(wavData.stream);
            SDL_free(wavData.buffer);
            return 0;
        }

        return WaveDataMap.add(wavData);
    }

    //--------------------------------------------------------------------------
    void UnloadWavData(WavData wavData) {
        if (wavData.stream) {
            unBindStream(wavData.stream);
            SDL_DestroyAudioStream(wavData.stream);
            wavData.stream = nullptr;
        }
        if (wavData.buffer) {
            SDL_free(wavData.buffer);
            wavData.buffer = nullptr;
        }
    }
    //--------------------------------------------------------------------------
    void SDLCALL MyAudioLoopCallback(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
        WavData* data = (WavData*)userdata;
        if (additional_amount > 0) {
            SDL_PutAudioStreamData(stream, data->buffer, data->len);
        }
    }
    //--------------------------------------------------------------------------
    bool setMasterVolume(float value){
        if (isInitialized()) {
            return SDL_SetAudioDeviceGain(AudioDevice, value);
        }
        return false;
    }
    float getMasterVolume(){
        return isInitialized() ? SDL_GetAudioDeviceGain(AudioDevice) : 1.0f;
    }
    //--------------------------------------------------------------------------
    bool play(WavData* data, float gain , bool loop ) {
        if (!isInitialized() || !data || !data->stream)  return false;
        SDL_ClearAudioStream(data->stream);
        SDL_SetAudioStreamGain(data->stream, gain);

        if (loop) {
            SDL_SetAudioStreamGetCallback(data->stream, MyAudioLoopCallback, data);
        } else {
            SDL_SetAudioStreamGetCallback(data->stream, nullptr, nullptr);
        }

        SDL_PutAudioStreamData(data->stream, data->buffer, data->len);
        SDL_ResumeAudioStreamDevice(data->stream);
        return true;
    }

    //--------------------------------------------------------------------------
    bool  stop(WavData* data) {
        if (!isInitialized() || !data || !data->stream) return false;
        SDL_SetAudioStreamGetCallback(data->stream, nullptr, nullptr);
        SDL_ClearAudioStream(data->stream);
        return true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------

    // TODO : This should be moved in a Melody file ?!

    /* Convert a MIDI note number to frequency */
    static F64 midiToFrequency(S32 midi_note)
    {
        if (midi_note <= 0) return 0.0; //silents
        return 440.0 *  ElfMath::mPow(2.0, (midi_note - 69) / 12.0);
    }

    /*
     * Add a short fade in/out to avoid clicks when notes change.
     */
    static float envelope(S32 sample, S32 total_samples, S32 sampleRate = 48000)
    {
        const S32 fade_samples = sampleRate / 100;

        F32 gain = 1.0f;

        if (sample < fade_samples) {
            gain = (F32)sample / fade_samples;
        }

        if (sample > total_samples - fade_samples) {
            gain = (F32)(total_samples - sample) / fade_samples;
        }

        if (gain < 0.0f) {
            gain = 0.0f;
        }

        return gain;
    }
    // -----------------
    static F32 *generateNoteF(int midi_note, F32 duration, int *byte_count, F32 amplitude = 0.20f, S32 sampleRate = 48000)
    {
        const S32 sample_count = (S32)(sampleRate * duration);

        F32* samples = (F32*)dMalloc(sample_count * sizeof(F32));
        if (!samples) {
            return NULL;
        }

        *byte_count = sample_count * (int)sizeof(F32);

        if (midi_note <= 0) {
            dMemset(samples, 0, *byte_count);
            return samples;
        }

        const double frequency = midiToFrequency(midi_note);

        for (int i = 0; i < sample_count; i++) {
            double time = (F64)i / sampleRate;
            F64 wave = ElfMath::mSin(2.0 * M_PI * frequency * time) * 0.85 +
            ElfMath::mSin(2.0 * M_PI * frequency * 2.0 * time) * 0.15;

            F32 gain = amplitude * envelope(i, sample_count);

            samples[i] = (F32)(wave * gain);
        }


        return samples;
    }

    // -------------------------------------------------------------------------
    S32 QSORT_CALLBACK compare_ToneEntryLineNumbers( const ToneEntry* infoA , const ToneEntry* infoB ) {
        if (infoA->LineNumber == infoB->LineNumber) return 0;
        if (infoB->LineNumber > infoA->LineNumber) return -1;
        return 1;
    }

    // -------------------------------------------------------------------------
    // Generate a melody by tones and return the WavData Object ID
    S32 GenerateMelody(Melody melody)  {
        if (melody.size() == 0) return false;

        melody.sort(&compare_ToneEntryLineNumbers);

        WavData wavData;
        wavData.spec = {
            .format     = SDL_AUDIO_F32,
            .channels   = 1,
            .freq       = 48000
        };
        Vector< F32*> toneBuffer;
        Vector< S32>  byteCounts;

        U32 total_bytes = 0;
        for (S32 i = 0; i < melody.size(); i++)  {
            S32  byteCount = 0;
            F32* noteBuffer =  generateNoteF(
                    melody[i].tone.midiNote
                    , melody[i].tone.duration
                    , &byteCount
                    , melody[i].tone.amplitute
                    , wavData.spec.freq
            );
            if (noteBuffer && byteCount > 0) {
                toneBuffer.push_back(noteBuffer);
                byteCounts.push_back(byteCount);
                total_bytes += byteCount;
            }
        }

        if (total_bytes == 0) return 0;
        wavData.len = total_bytes;
        wavData.buffer = (U8*)dMalloc(total_bytes);
        if (!wavData.buffer) {
            for (S32 i = 0; i < toneBuffer.size(); i++) dFree(toneBuffer[i]);
            return 0;
        }


        U8* writePtr = wavData.buffer;
        for (S32 i = 0; i < toneBuffer.size(); i++) {
            dMemcpy(writePtr, toneBuffer[i], byteCounts[i]);
            writePtr += byteCounts[i];
            dFree(toneBuffer[i]);
        }
        toneBuffer.clear();
        byteCounts.clear();


        wavData.stream = SDL_CreateAudioStream(&wavData.spec, nullptr);
        if (!wavData.stream ) {
            Con::errorf("Couldn't create audio stream: %s", SDL_GetError());
            return 0;
        }

        if (!bindStream(wavData.stream )) {
            SDL_Log("Failed to bind  stream to device: %s", SDL_GetError());
            SDL_DestroyAudioStream(wavData.stream);
            SDL_free(wavData.buffer);
            return 0;
        }

        return WaveDataMap.add(wavData);
    }


    //--------------------------------------------------------------------------
    //                    === Melody Maker :D ===

    // ------
    S32 MelodyBegin() {
        return MelodyMap.add(Melody());
    }

    // ------
    bool MelodyAddFromConsoleVector(S32 MelodyID, ConsoleVector values) {
        Melody* melodyPtr = MelodyMap.get(MelodyID);
        if (!melodyPtr) return false;

        // Mindestens Zeilennummer und Note müssen im Vector sein
        S32 lineNumber = (S32)values.points[0];
        if (lineNumber < 0) return false;

        ToneEntry newTone = {0};
        newTone.LineNumber     = lineNumber;
        newTone.tone.midiNote  = (S32)values.points[1];
        newTone.tone.duration  = values.points[2] > 0.f ? values.points[2] : 0.2f; // Fallback
        newTone.tone.amplitute = values.points[3] > 0.f ? values.points[3] : 0.2f; // Fallback

        // overwrite when exists
        for (S32 i = 0; i < melodyPtr->size(); i++) {
            if ((*melodyPtr)[i].LineNumber == lineNumber) {
                (*melodyPtr)[i] = newTone;
                return true;
            }
        }

        melodyPtr->push_back(newTone);
        return true;
    }

    // ------
    bool MelodyDel(S32 MelodyID, S32 lineNumber) {
        Melody* melodyPtr = MelodyMap.get(MelodyID);
        if (!melodyPtr) return false;

        for (S32 i = 0; i < melodyPtr->size(); i++) {
            if ((*melodyPtr)[i].LineNumber == lineNumber) {
                (*melodyPtr).erase(i);
                return true;
            }
        }
        return false;
    }

    // ------
    String MelodyList(S32 MelodyID, S32 lineStart = -1, S32 lineEnd = -1) {
        Melody* melodyPtr = MelodyMap.get(MelodyID);
        if (!melodyPtr || melodyPtr->size() == 0) return "";

        melodyPtr->sort(&compare_ToneEntryLineNumbers);

        Melody& melody = *melodyPtr;

        StringBuilder result;

        for (S32 i = 0; i < melody.size(); i++) {

            if (lineStart > 0 && melody[i].LineNumber < lineStart) continue;
            if (lineEnd > 0 && melody[i].LineNumber > lineEnd) continue;

            if ( melody[i].tone.duration != 0.2f || melody[i].tone.amplitute != 0.2f) {

                result.format("%d %d %.2f %.2f\n",
                            melody[i].LineNumber, melody[i].tone.midiNote,
                            melody[i].tone.duration, melody[i].tone.amplitute);
            } else {
                result.format("%d %d\n", melody[i].LineNumber, melody[i].tone.midiNote);
            }
            // result.format("#%03d Note: %03d | Dur: %.2f | Vol: %.2f\n",
            //             melody[i].LineNumber, melody[i].tone.midiNote,
            //             melody[i].tone.duration, melody[i].tone.amplitute);
        }
        return result.end();
    }

    // ------
    // run => play the melody
    bool MelodyRun(S32 MelodyID, F32 gain = 1.f) {
        Melody* melodyPtr = MelodyMap.get(MelodyID);
        if (!melodyPtr) return false;

        S32 wavID = GenerateMelody(*melodyPtr);
        if (wavID == 0) return false;
        WavData* data = WaveDataMap.get(wavID);
        if (!data) return false;

        play(data, gain, false);

        F32 durationInSeconds = (F32)data->len / (1.0f * sizeof(F32) * data->spec.freq);
        U32 durationInMs = (U32)(durationInSeconds * 1000.0f) + 200; // +200ms Puffer zur Sicherheit

        Con::evaluatef("schedule(%d, 0, Audio_UnLoadWav, %d);",durationInMs, wavID);

        return true;
    }

    // ------
    // gen => generate wavedata from melody
    S32 MelodyGen(S32 MelodyID) {
        Melody* melodyPtr = MelodyMap.get(MelodyID);
        if (!melodyPtr) return 0;
        return GenerateMelody(*melodyPtr);
    }

    // end => remove the melody from map
    bool MelodyEnd(S32 MelodyID) {
       return MelodyMap.remove(MelodyID);
    }
    // <<<< Melody Maker <<<<
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool Init()
    {
        if (AudioDevice != 0) {
            return true;
        }
        AudioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        Con::infof("Audio_Init device id:%d.", AudioDevice);
        return  AudioDevice != 0;
    }

    void ShutDown()
    {
        Audio::WaveDataMap.clear();
        Audio::MelodyMap.clear();
    }

} //namespace Audio
// ========================== BINDINGS ====================================
DefineEngineFunction(Audio_Init, bool, (), , "Init Audio Device for playback") {
    return Audio::Init();
}
DefineEngineFunction(Audio_Shutdown, void, (), , "ShutDown Audio Device ") {
    Audio::ShutDown();
}

DefineEngineFunction(Audio_LoadWav, S32, (const char* filename), , "Load a Wave File.\n@return WavID") {
    return Audio::loadWav(filename);
}

DefineEngineFunction(Audio_UnLoadWav, S32, (S32 WavID), , "Unload a Wave Steam") {
    return Audio::WaveDataMap.remove(WavID);
}


DefineEngineFunction(Audio_Play, bool, (S32 WavID, F32 gain, bool loop),(1.0f, false) , "Play a Wave Stream.@param gain is the normalized volume") {
    Audio::WavData* wavData = Audio::WaveDataMap.get(WavID);
    if (!wavData) return false;
    return Audio::play(wavData, gain, loop);
}
DefineEngineFunction(Audio_Stop, bool, (S32 WavID), , "Stop a Wave Stream.") {
    Audio::WavData* wavData = Audio::WaveDataMap.get(WavID);
    if (!wavData) return false;
    return Audio::stop(wavData);
}

DefineEngineFunction(Audio_SetMasterVolume, bool, (F32 volume), , "set the volume for all streams") {
    return Audio::setMasterVolume(volume);
}
DefineEngineFunction(Audio_GetMasterVolume, F32, (), , "get the volume for all streams") {
    return Audio::getMasterVolume();
}

// Audio_Init done?
// $testMidi = TestMidi(); Audio_Play($testMidi);
DefineEngineFunction(TestMidi, S32, (),,"make a midi test ....") {
    // //  C4, D4, E4, F4, G4, A4, B4, C5
    // //  60, 62, 64, 65, 67, 69, 71, 72
    Audio::Melody melody;
    melody.push_back({10, {60, 0.3f}}); // C
    melody.push_back({20, {64, 0.3f}}); // E
    melody.push_back({30, {67, 0.3f}}); // G
    melody.push_back({40, { 0, 0.3f}}); // silence
    melody.push_back({50, {67, 0.3f}}); // G
    melody.push_back({60, {64, 0.3f}}); // E
    melody.push_back({70, {60, 0.3f}}); // C

    return Audio::GenerateMelody(melody);

}

DefineEngineFunction(MelodyBegin, S32, (),,"create a melody maker") { return Audio::MelodyBegin();}
DefineEngineFunction(MelodyAdd, bool, (S32 MelodyID, ConsoleVector data),
                     ,"data: {(int)LINENUMBER, (int)MIDITONE, (float) duration=0.2f, (float)amplitude=0.2f}\n"
                     "add a tone at line number - in line exists it will be overwritten") {
    return Audio::MelodyAddFromConsoleVector(MelodyID, data);
}
DefineEngineFunction(MelodyDel, bool, (S32 MelodyID, S32 lineNumber),,"Delete tone at linenumber" ) {
    return Audio::MelodyDel(MelodyID, lineNumber);
}
DefineEngineFunction(MelodyList, String, (S32 MelodyID, S32 lineNumberStart, S32 lineNumberEnd),(-1, -1),"list the melody") {
    return Audio::MelodyList(MelodyID, lineNumberStart, lineNumberEnd);
}
DefineEngineFunction(MelodyRun, bool, (S32 MelodyID, F32 gain ),(1.0),"run = play the melody") {
    return Audio::MelodyRun(MelodyID);
}
DefineEngineFunction(MelodyGen, S32, (S32 MelodyID),,"gen = generate wavData from this melody") {
    return Audio::MelodyGen(MelodyID);
}
DefineEngineFunction(MelodyEnd, bool, (S32 MelodyID),,"remove this melody - if you used MelodyGen: the wave data will not be touched") {
    return Audio::MelodyEnd(MelodyID);
}

} //namespace










