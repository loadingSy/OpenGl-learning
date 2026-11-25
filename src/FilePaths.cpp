#include <string>
#include <windows.h>

std::string getExeDirectory() {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0 || length == MAX_PATH) {
        return ".";
    }
    std::string fullPath(buffer, length);
    size_t pos = fullPath.find_last_of("\\/");
    if (pos == std::string::npos) {
        return ".";
    }
    return fullPath.substr(0, pos);
}

std::string getProjectPath(const std::string& relativePath) {
    #ifdef NDEBUG
        return getExeDirectory() + '/' relativePath;
    #else
        return std::string(PROJECT_SOURCE_DIR) + '/' + relativePath;
    #endif
}