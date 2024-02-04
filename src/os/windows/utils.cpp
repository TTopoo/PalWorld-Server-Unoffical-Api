#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>

std::wstring local_codepage_to_utf16(std::string input) {
    size_t       need = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), 0, 0);
    wchar_t     *wstr = static_cast<wchar_t *>(malloc(need + sizeof(wchar_t)));
    size_t       used = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), wstr, need);
    std::wstring result(wstr, used);
    free(wstr);
    return result;
}

std::string utf16_to_local_codepage(wchar_t *data, size_t len) {
    BOOL ignore;

    size_t      need = WideCharToMultiByte(CP_ACP, 0, data, len, 0, 0, "?", &ignore);
    char       *cstr = static_cast<char *>(malloc(need + 1));
    size_t      used = WideCharToMultiByte(CP_ACP, 0, data, len, cstr, need, "?", &ignore);
    std::string result(cstr, used);
    free(cstr);
    return result;
}

std::vector<unsigned short> hexstring2shorts(const std::string &hex) {
    std::vector<unsigned short> shorts;

    std::istringstream iss(hex);
    std::string byteString;
    while (iss >> byteString) {
        if (byteString == "??") {
            shorts.push_back(0xFFFF);
        }
        else {
            unsigned short byteValue;
            std::istringstream(byteString) >> std::hex >> byteValue;
            shorts.push_back(byteValue);
        }
    }
    return shorts;
}

unsigned long long search_pattern(const unsigned char* data, const unsigned int data_len, const std::vector<unsigned short> &&pattern) {
    const unsigned char *startAddr = data;
    const unsigned char *endAddr = data + data_len;
    auto it = std::search(
        startAddr, endAddr,
        pattern.begin(), pattern.end(),
        [](unsigned char data, unsigned short pattern) {
            return pattern == 0xFFFF || pattern == data;
        });

    if (it != endAddr) {
        return (unsigned long long)it;
    }
    else {
        return 0;
    }
}

HMODULE self_module_handle() {
    MEMORY_BASIC_INFORMATION mbi;
    return ((::VirtualQuery(self_module_handle, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
}

inline std::wstring self_module_path() {
    static const size_t INITIAL_BUFFER_SIZE = MAX_PATH;
    static const size_t MAX_ITERATIONS      = 7;
    std::wstring ret;
    DWORD bufferSize = INITIAL_BUFFER_SIZE;
    auto module_handle = self_module_handle();
    if (module_handle == NULL) {
        return L"";
    }

    for (size_t iterations = 0; iterations < MAX_ITERATIONS; ++iterations) {
        ret.resize(bufferSize);
        DWORD charsReturned = ::GetModuleFileNameW(module_handle, &ret[0], bufferSize);
        if (charsReturned < ret.length()) {
            ret.resize(charsReturned);
            return ret;
        }
        else {
            bufferSize *= 2;
        }
    }
    return L"";
}

std::wstring real_file_path(std::wstring setting_name) {
    std::filesystem::path module_path;

    auto module_filename = self_module_path();
    if (module_filename.empty()) {
        return L"";
    }

    module_path = module_filename;
    module_path = module_path.parent_path();
    module_path.append(setting_name);

    return module_path.wstring();
}
