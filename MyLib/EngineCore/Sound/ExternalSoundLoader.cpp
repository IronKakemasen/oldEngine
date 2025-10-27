#include "ExternalSoundLoader.h"

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")


ExternalSoundLoader::ExternalSoundLoader()
{

}

ExternalSoundLoader::~ExternalSoundLoader()
{
    pMFMediaType->Release();
    pMFSourceReader->Release();
    MFShutdown();
}

void ExternalSoundLoader::LoadAudioFile(std::wstring path, AudioSet* audioSet_, SoundTag tag_, MusicOrSoundEffect type_)
{

    MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

    MFCreateSourceReaderFromURL(path.c_str(), NULL, &pMFSourceReader);

    MFCreateMediaType(&pMFMediaType);
    pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

    pMFMediaType->Release();
    pMFMediaType = nullptr;
    pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

    MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &audioSet_->waveFormat, nullptr);

    while (true)
    {
        IMFSample* pMFSample{ nullptr };
        DWORD dwStreamFlags{ 0 };
        pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

        if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        IMFMediaBuffer* pMFMediaBuffer{ nullptr };
        pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

        BYTE* pBuffer{ nullptr };
        DWORD cbCurrentLength{ 0 };
        pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

        audioSet_->mediaData.resize(audioSet_->mediaData.size() + cbCurrentLength);
        memcpy(audioSet_->mediaData.data() + audioSet_->mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

        pMFMediaBuffer->Unlock();

        pMFMediaBuffer->Release();
        pMFSample->Release();
    }

    audioSet_->CreateXaudio2();
    audioSet_->pXAudio2->CreateMasteringVoice(&audioSet_->pMasteringVoice);
    audioSet_->id.Set(tag_, type_);
}
