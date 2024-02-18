#include <iostream>
#include <cstdlib>
#include <string>
#include "SharedFunctions.h"

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

    std::string reposCommand = "curl -s https://api.github.com/users/Gamer100Dev/repos";
    std::string reposList = execCommand(reposCommand);

    size_t discordRepoStart = reposList.find("\"name\": \"Discord-BSD-");
    size_t discordRepoEnd = reposList.find("\",", discordRepoStart);

    if (discordRepoStart != std::string::npos && discordRepoEnd != std::string::npos) {
        std::string discordRepoName = reposList.substr(discordRepoStart + 13, discordRepoEnd - discordRepoStart - 13);

        size_t versionStart = discordRepoName.find_last_of("-");

        if (versionStart != std::string::npos) {
            std::string repoVersion = discordRepoName.substr(versionStart + 1);

            std::string installScriptPath = destinationPath + "/Discord-BSD-" + repoVersion + "/install.sh";
            std::string repoString = "https://github.com/Gamer100Dev/Discord-BSD-" + repoVersion;
            downloadFolder(repoString, destinationPath, repoVersion);


            if (installApp(installScriptPath)) {
                std::cout << "Installation successful!" << std::endl;
            } else {
                std::cerr << "Installation failed." << std::endl;
            }
        } else {
            std::cerr << "Failed to extract version from the Discord repository name." << std::endl;
            return 1; // Exit with an error code
        }
    } else {
        std::cerr << "Failed to find the Discord repository in the list." << std::endl;
        return 1; // Exit with an error code
    }

    return 0;
}
