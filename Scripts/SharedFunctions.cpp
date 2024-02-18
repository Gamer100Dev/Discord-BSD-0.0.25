// SharedFunctions.cpp
#include "SharedFunctions.h"
#include <curl/curl.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
void downloadFile(const std::string& fileUrl, const std::string& destinationPath) {
    char* encodedUrl = curl_easy_escape(nullptr, fileUrl.c_str(), static_cast<int>(fileUrl.length()));
    if (!encodedUrl) return;
    std::string command = "curl -sL " + std::string(encodedUrl) + " -o " + destinationPath;
    curl_free(encodedUrl);
    int result = system(command.c_str());
    if (result != 0) std::cerr << "Failed to download file: " << fileUrl << std::endl;
}

void downloadFolder(const std::string& folderUrl, const std::string& destinationPath, const std::string& repoVersion) {

    std::string mkdirCommand = "mkdir -p " + destinationPath;
    int resultMkdir = system(mkdirCommand.c_str());

    if (resultMkdir != 0) {
        std::cerr << "Failed to create directory: " << destinationPath << std::endl;
        return;
    }

    std::string gitCloneCommand = "git clone " + folderUrl + " " + destinationPath + "/Discord-BSD-" + repoVersion;
    std::cout << "COMMAND: " << gitCloneCommand << std::endl;
    int resultGitClone = system(gitCloneCommand.c_str());

    if (resultGitClone != 0) {
        std::cerr << "Failed to clone repository: " << folderUrl << std::endl;
        return;
    }
}

bool installApp(const std::string& installScriptPath) {
    std::string installScriptPathA = installScriptPath ;
    std::string command = "chmod +x " + installScriptPathA;
    int resultChmod = system(command.c_str());

    if (resultChmod != 0) return false;

    command = installScriptPath;
    int resultInstall = system(command.c_str());

    return resultInstall == 0;
}
