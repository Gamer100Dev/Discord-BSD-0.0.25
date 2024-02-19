#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include "SharedFunctions.h"

std::vector<int> parseVersion(const std::string& version) {
    std::vector<int> result;
    std::istringstream iss(version);
    std::string part;

    while (std::getline(iss, part, '.')) {
        result.push_back(std::stoi(part));
    }

    return result;
}

bool compareVersions(const std::string& version1, const std::string& version2) {
    std::vector<int> v1 = parseVersion(version1);
    std::vector<int> v2 = parseVersion(version2);

    size_t size = std::max(v1.size(), v2.size());

    for (size_t i = 0; i < size; ++i) {
        int num1 = (i < v1.size()) ? v1[i] : 0;
        int num2 = (i < v2.size()) ? v2[i] : 0;

        if (num1 < num2) {
            return true;
        } else if (num1 > num2) {
            return false;
        }
    }

    return false;
}
std::string execCommand(const std::string& command) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "Error: command execution failed!";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}


int main() {
    std::string homeDir = std::getenv("HOME");
    std::string destinationPath = homeDir + "/.cache";
    std::string currentVersion = "0.0.25"; // corrected variable name
    std::string reposCommand = "curl -s https://api.github.com/users/Gamer100Dev/repos";
    std::string reposList = execCommand(reposCommand);

    size_t discordRepoStart = reposList.find("\"name\": \"Discord-BSD-");
    size_t discordRepoEnd = reposList.find("\",", discordRepoStart);

    if (discordRepoStart != std::string::npos && discordRepoEnd != std::string::npos) {
        std::string discordRepoName = reposList.substr(discordRepoStart + 13, discordRepoEnd - discordRepoStart - 13);

        size_t versionStart = discordRepoName.find_last_of("-");

        if (versionStart != std::string::npos) {
            std::string repoVersion = discordRepoName.substr(versionStart + 1);

            std::cout << "DownloadHandler output: New version is " << repoVersion
                      << " and current version is " << currentVersion << std::endl;


            if (compareVersions(repoVersion, currentVersion)) {
                std::string installScriptPath = destinationPath + "/Discord-BSD-" + repoVersion + "/install.sh";
                std::string repoString = "https://github.com/Gamer100Dev/Discord-BSD-" + repoVersion;


                downloadFolder(repoString, destinationPath, repoVersion);

                if (installApp(installScriptPath)) {
                    std::cout << "Installation successful!" << std::endl;
                } else {
                    std::cerr << "Installation failed." << std::endl;
                }
            } else if (compareVersions(currentVersion, repoVersion)) {
                std::cerr << "DownloadHandler error: This version is older than the current version." << std::endl;
            } else {
                std::cout << "DownloadHandler output: The versions are the same." << std::endl;
            }
        } else {
            std::cerr << "Failed to extract version from the Discord repository name." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Failed to find the Discord repository in the list." << std::endl;
        return 1;
    }

    return 0;
}

