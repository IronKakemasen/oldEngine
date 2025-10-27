#include "Sound.h"
#pragma comment(lib,"xaudio2.lib")
#include <thread>

void SoundIdentification::Set(SoundTag tag_, MusicOrSoundEffect type_)
{
    tag = tag_;
    type = type_;
}

AudioSet::~AudioSet()
{
    //pMasteringVoice->DestroyVoice();
    pXAudio2->Release();
    CoTaskMemFree(waveFormat);
}

void AudioSet::Play()
{
    pSourceVoice = { nullptr };

    pXAudio2->CreateSourceVoice(&pSourceVoice, waveFormat);

    XAUDIO2_BUFFER buffer{ 0 };
    buffer.pAudioData = mediaData.data();
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());
    pSourceVoice->SubmitSourceBuffer(&buffer);
    pSourceVoice->Start(0);
}

void AudioSet::SetStatus()
{
    XAUDIO2_VOICE_STATE state{ 0 };
    pSourceVoice->GetState(&state);
    if (state.BuffersQueued != 0)
    {
        status = SoundStatus::kPlaying;
    }
    else
    {
        status = SoundStatus::kStop;
        //pSourceVoice->DestroyVoice();
    }
}


void AudioSet::Activate()
{
    if(status != SoundStatus::kPlaying)status = SoundStatus::kPlay;
}

void AudioSet::Stop()
{
    status = SoundStatus::kStop;
}

void AudioSet::Reset()
{
    //pSourceVoice->Start(0);
}

void AudioSet::CreateXaudio2()
{
    XAudio2Create(&pXAudio2, 0);
#if defined(_DEBUG)
    XAUDIO2_DEBUG_CONFIGURATION debug{ 0 };
    debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
    debug.BreakMask = XAUDIO2_LOG_ERRORS;
    pXAudio2->SetDebugConfiguration(&debug, 0);
#endif

}

AudioSet::AudioSet()
{
    //CreateXaudio2();
    //pSourceVoice->DestroyVoice();
}
