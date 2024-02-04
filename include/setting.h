#pragma once

#include <string>
#include <fstream>

bool setting_init(std::wstring setting_name);

class Setting
{
public:
    static Setting& getInstance();

    bool readSettingFile(std::wstring setting_name);


    // signature
    std::string signatureGWorld;
    std::string signatureGEngine;
    std::string signatureFNameAppendString;
    std::string signatureForceGarbageCollection;
    std::string signatureKickPlayer;
    std::string signatureGetEmptyFText;
    std::string signatureSpawnPlayActor;
    std::string signatureGetGameState;
    std::string signatureSendAnnounce;
    std::string signatureGetCharacters;
    std::string signatureGetController;
    std::string signatureGetPlayerState;
    std::string signatureGetPlayerName;
    std::string signatureGetPlayerUID;
    std::string signatureGetWorldName;
    std::string signatureGetPlayerStateFromController;
    std::string signatureGetPlayerNetworkAddress;

private:
    Setting();
    Setting(const Setting&);
    Setting& operator=(const Setting&);
};
