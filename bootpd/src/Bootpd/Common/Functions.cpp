#include "Functions.h"

#if defined(_WIN32) || defined(_WIN64)
#include <rpc.h>
#pragma comment(lib, "rpcrt4.lib")

std::string Functions::GenerateUUID() {
    UUID uuid;
    UuidCreate(&uuid);

    RPC_CSTR str = NULL;
    UuidToStringA(&uuid, &str);
    std::string result = (char*)str;
    RpcStringFreeA(&str);

    return result;
}
#else
#include <fstream>

std::string Functions::GenerateUUID() {
    std::ifstream f("/proc/sys/kernel/random/uuid");
    std::string uuid;
    if (f.is_open()) {
        std::getline(f, uuid);
    } else {
        // Fallback: manual UUID v4 generation
        static const char* hex = "0123456789abcdef";
        uuid = "00000000-0000-0000-0000-000000000000";
        for (size_t i = 0; i < 36; i++) {
            if (uuid[i] == '0') {
                // seed from /dev/urandom would be better, but /proc is simpler
                int rnd = rand() % 16;
                if (i == 14) rnd = 4; // Version 4
                if (i == 19) rnd = (rand() % 4) + 8; // Variant
                uuid[i] = hex[rnd];
            }
        }
    }
    return uuid;
}
#endif