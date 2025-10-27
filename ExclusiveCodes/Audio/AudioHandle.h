#pragma once
#include <array>
#include <string>
#include <memory>

// 音声ID
enum class AudioID
{
    // BGM
    gameScene,

    // SE
    jump,
    cardTouch,
    goal,
    field,
    getKey,
    movableMove,
    cardChoose,
    arrow,
    anker,
    pause,
    gear,
    disSelectCard,
    usedCardSelect,
    reset,
    decision,
    clear,

    count
};

// AudioIDとパスを紐付け
struct AudioDefinition
{
    AudioID id;
    const wchar_t* path;
};

class AudioHandle
{
public:
    static void Initialize();
    static int Get(AudioID id);  // AudioPlayerが管理する配列インデックスを返す

private:
    static constexpr std::array<AudioDefinition, static_cast<size_t>(AudioID::count)> audioDefinitions_ =
    {
        {
            { AudioID::gameScene, L"Resource/Audio/BGM/gameScene.mp3" },

            { AudioID::jump, L"Resource/Audio/SE/jump.mp3" },
            { AudioID::cardTouch, L"Resource/Audio/SE/cardTouch.mp3" },
            { AudioID::goal, L"Resource/Audio/SE/goal.mp3" },
            { AudioID::field, L"Resource/Audio/SE/field.mp3" },
            { AudioID::getKey, L"Resource/Audio/SE/getKey.mp3" },
            { AudioID::movableMove, L"Resource/Audio/SE/movableMove.mp3" },
            { AudioID::cardChoose, L"Resource/Audio/SE/cardChoose.mp3" },
            { AudioID::arrow, L"Resource/Audio/SE/arrow.mp3" },
            { AudioID::anker, L"Resource/Audio/SE/anker.mp3" },
            { AudioID::pause, L"Resource/Audio/SE/gear_low.mp3" },
            { AudioID::gear, L"Resource/Audio/SE/gear.mp3" },
            { AudioID::disSelectCard, L"Resource/Audio/SE/disSelectCard.mp3" },
            { AudioID::usedCardSelect, L"Resource/Audio/SE/usedCardSelect.mp3" },
            { AudioID::reset, L"Resource/Audio/SE/reset.mp3" },
            { AudioID::decision, L"Resource/Audio/SE/decision.mp3" },
            { AudioID::clear, L"Resource/Audio/SE/clear.mp3" },
        }
    };

    static std::array<int, static_cast<size_t>(AudioID::count)> audioIndices_; // AudioPlayer内のID
    static bool initialized_;
};
