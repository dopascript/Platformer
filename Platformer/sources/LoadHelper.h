#ifndef LOADHELPER_H
#define LOADHELPER_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, char seperator);
std::string loadTextFile(const std::string &pPath);

#endif