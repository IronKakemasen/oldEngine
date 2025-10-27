#pragma once
#include <xaudio2.h>
#include <vector>
#include <unordered_map>

enum class SoundStatus
{
    kPlay,
    kPlaying,
    kStop,

};

enum class SoundTag
{
    kTest,
    kGet,

    kCount
};

enum MusicOrSoundEffect
{
    kMusic,
    kSoundEffect
};

struct SoundIdentification
{   
    SoundTag tag;
    MusicOrSoundEffect type;

    void Set(SoundTag tag_, MusicOrSoundEffect type_);
};

struct AudioSet
{
    SoundIdentification id;
    SoundStatus status = SoundStatus::kStop;
    std::vector<BYTE> mediaData;
    WAVEFORMATEX* waveFormat{ nullptr };
    IXAudio2SourceVoice* pSourceVoice{ nullptr };

    static inline IXAudio2* pXAudio2{ nullptr };
    static inline IXAudio2MasteringVoice* pMasteringVoice{ nullptr };

    AudioSet();
    ~AudioSet();

    void Play();
    void Activate();
    void Stop();
    void Reset();
    void SetStatus();
    void CreateXaudio2();
};

