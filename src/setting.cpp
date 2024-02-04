#include "setting.h"

#include "utils.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

bool setting_init(std::wstring setting_name)
{
    return Setting::getInstance().readSettingFile(setting_name);
}

Setting::Setting()
{

}

Setting &Setting::getInstance()
{
    static Setting instance;
    return instance;
}

bool Setting::readSettingFile(std::wstring setting_name)
{
    try
    {
        std::ifstream setting_stream;
        std::wstring setting_path;

#ifdef _WIN32
        setting_path = real_file_path(setting_name);
#else
        setting_path = std::wstring setting_path = setting_name;

#endif
        if (setting_path.empty()) {
            return false;
        }

        setting_stream.open(setting_path);
        if(!setting_stream.is_open())
        {
            return false;
        }

        json data;
        setting_stream >> data;

        data = data["pal-plugin-loader"];
        json signature;
        #ifdef _WIN32
            signature = data["signature"]["windows"];
        #else
            signature = data["signature"]["linux"];
        #endif

        signatureGWorld = signature["GWorld"];
        signatureGEngine = signature["GEngine"];
        signatureFNameAppendString = signature["FNameAppendString"];
        signatureForceGarbageCollection = signature["ForceGarbageCollection"];
        signatureKickPlayer = signature["KickPlayer"];
        signatureGetEmptyFText = signature["GetEmptyFText"];
        signatureSpawnPlayActor = signature["SpawnPlayActor"];
        signatureGetGameState = signature["GetGameState"];
        signatureSendAnnounce = signature["SendAnnounce"];
        signatureGetCharacters = signature["GetCharacters"];
        signatureGetController = signature["GetController"];
        signatureGetPlayerState = signature["GetPlayerState"];
        signatureGetPlayerName = signature["GetPlayerName"];
        signatureGetPlayerUID = signature["GetPlayerUID"];
        signatureGetWorldName = signature["GetWorldName"];
        signatureGetPlayerStateFromController = signature["GetPlayerStateFromController"];
        signatureGetPlayerNetworkAddress = signature["GetPlayerNetworkAddress"];

    }
    catch (const std::exception &e)
    {
        return false;
    }
    catch (const json::exception& e)
    {
        return false;
    }
    return true;
}
