#include "crmfile.h"
#include "soundhandler.h"
#include "config.h"

#include <algorithm>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

namespace SoundHandler
{
    // Raw sound data blobs and SDL_mixer chunks
    static CrmFile    sounddata[SOUND_END];
    static Mix_Chunk* sdlsounds[SOUND_END];

    // Minimal WAV header prefix used to wrap Paula-style samples
    static uint8_t wavheader[] = "RIFF    WAVEfmt ";

    // Convert game sample (offset-binary, meta in first 4 bytes) into a WAV buffer.
    // Returns heap-allocated buffer that must be delete[]'d by caller.
    static uint8_t* CreateWAV(uint8_t* data, uint32_t filelength)
    {
        uint32_t period = (uint32_t)(data[0]) << 8 | data[1];
        uint32_t length = (uint32_t)(data[2]) << 8 | data[3];

        // hello Paula
        period = 3546895 / period; // sample rate
        length *= 2;               // 2x because meta says words, buffer holds bytes

        // guard against corrupt sample lengths
        if ((length + 4) > filelength)
        {
            length = filelength - 4;
        }

        // signed -> offset-binary conversion in-place for payload
        for (uint32_t i = 4; i < (length + 4); i++)
        {
            data[i] ^= 0x80;
        }

        // allocate WAV (44-byte header + payload)
        uint8_t* wavbuffer = new uint8_t[length + 44];

        std::copy(wavheader, wavheader + 16, wavbuffer);

        // RIFF chunk size
        wavbuffer[4]  = ((36 + length) >> 0) & 0xff;
        wavbuffer[5]  = ((36 + length) >> 8) & 0xff;
        wavbuffer[6]  = ((36 + length) >> 16) & 0xff;
        wavbuffer[7]  = ((36 + length) >> 24) & 0xff;

        // fmt chunk size (16)
        wavbuffer[16] = (16 >> 0) & 0xff;
        wavbuffer[17] = (16 >> 8) & 0xff;
        wavbuffer[18] = (16 >> 16) & 0xff;
        wavbuffer[19] = (16 >> 24) & 0xff;

        // audio format PCM
        wavbuffer[20] = 1; wavbuffer[21] = 0;

        // channels = 1 (mono)
        wavbuffer[22] = 1; wavbuffer[23] = 0;

        // sample rate
        wavbuffer[24] = (period >> 0) & 0xff;
        wavbuffer[25] = (period >> 8) & 0xff;
        wavbuffer[26] = (period >> 16) & 0xff;
        wavbuffer[27] = (period >> 24) & 0xff;

        // byte rate (mono, 8-bit)
        wavbuffer[28] = (period >> 0) & 0xff;
        wavbuffer[29] = (period >> 8) & 0xff;
        wavbuffer[30] = (period >> 16) & 0xff;
        wavbuffer[31] = (period >> 24) & 0xff;

        // block align
        wavbuffer[32] = 1;
        wavbuffer[33] = 0;

        // bits per sample = 8
        wavbuffer[34] = 8;
        wavbuffer[35] = 0;

        // data chunk header
        wavbuffer[36] = 'd'; wavbuffer[37] = 'a';
        wavbuffer[38] = 't'; wavbuffer[39] = 'a';

        // data size
        wavbuffer[40] = (length >> 0) & 0xff;
        wavbuffer[41] = (length >> 8) & 0xff;
        wavbuffer[42] = (length >> 16) & 0xff;
        wavbuffer[43] = (length >> 24) & 0xff;

        // payload
        std::copy(data + 4, data + length + 4, wavbuffer + 44);

        return wavbuffer;
    }

    void Quit()
    {
        // Free all loaded sound chunks to avoid leaks and stale pointers
        for (int i = 0; i < SOUND_END; ++i)
        {
            if (sdlsounds[i] != nullptr)
            {
                Mix_FreeChunk(sdlsounds[i]);
                sdlsounds[i] = nullptr;
            }
        }

        // Halt any playing channels before closing the mixer backend
        Mix_HaltChannel(-1);

        // Close the mixer and its dynamically loaded backends (if any)
        Mix_CloseAudio();
        Mix_Quit();
    }

    void Init()
    {
        // Ensure clean state
        for (int i = 0; i < SOUND_END; ++i) sdlsounds[i] = nullptr;

        if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) != 0)
        {
            SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Mix_OpenAudio failed: %s", Mix_GetError());
            return;
        }

        Mix_AllocateChannels(16);

        int freq = 0; Uint16 fmt = 0; int chans = 0;
        if (Mix_QuerySpec(&freq, &fmt, &chans) == 0)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Mix_QuerySpec returned 0: %s", Mix_GetError());
        }
        else
        {
            SDL_Log("SDL_mixer: opened %d Hz, chans=%d", freq, chans);
        }

        for (int i = 0; i < SOUND_END; ++i)
        {
            sounddata[i].Load(Config::GetSoundFilename(static_cast<SoundHandler::Sounds>(i)).c_str());

            if (!sounddata[i].data)
            {
                SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Missing sound data for id=%d", i);
                continue;
            }

            uint8_t* wavdata = CreateWAV(sounddata[i].data, sounddata[i].size);
            if (!wavdata)
            {
                SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "CreateWAV failed for id=%d", i);
                continue;
            }

            const int totalSize = sounddata[i].size - 4 + 44;
            SDL_RWops* rwop = SDL_RWFromMem(wavdata, totalSize);
            if (!rwop)
            {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_RWFromMem failed (id=%d): %s", i, SDL_GetError());
                delete[] wavdata;
                continue;
            }

            Mix_Chunk* ch = Mix_LoadWAV_RW(rwop, 0);
            SDL_RWclose(rwop);

            if (!ch)
            {
                SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV_RW failed (id=%d): %s", i, Mix_GetError());
                delete[] wavdata;
                continue;
            }

            sdlsounds[i] = ch;
            delete[] wavdata;
        }
    }

    void Play(int i)
    {
        if (i < 0 || i >= SOUND_END)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Play() index out of range: %d", i);
            return;
        }
        if (!sdlsounds[i])
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Play() called with null chunk for id=%d", i);
            return;
        }
        if (Mix_PlayChannel(-1, sdlsounds[i], 0) == -1)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_AUDIO, "Mix_PlayChannel failed: %s", Mix_GetError());
        }
    }
}
