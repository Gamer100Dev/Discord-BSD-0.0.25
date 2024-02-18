// SharedFunctions.h
#ifndef SHAREDFUNCTIONS_H
#define SHAREDFUNCTIONS_H

#include <string>

void downloadFile(const std::string& fileUrl, const std::string& filePath);
void downloadFolder(const std::string& folderUrl, const std::string& destinationPath, const std::string& versionNumber);
bool installApp(const std::string& installScriptPath);

#endif // SHAREDFUNCTIONS_H
