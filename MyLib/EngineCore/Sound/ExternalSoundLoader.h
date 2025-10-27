#pragma once
#include <string>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include "Sound.h"



class ExternalSoundLoader
{
public:

    IMFSourceReader* pMFSourceReader{ nullptr };
    IMFMediaType* pMFMediaType{ nullptr };
    IXAudio2MasteringVoice* pMasteringVoice{ nullptr };


    ExternalSoundLoader();
    ~ExternalSoundLoader();
    void LoadAudioFile(std::wstring path, AudioSet* audioSet_, SoundTag tag_, MusicOrSoundEffect type_);



};
