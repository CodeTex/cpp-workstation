#ifndef UTILS_H
#define UTILS_H

#include <string>

void listDevices();
bool checkBooleanFlag(int argc, char** argv, const std::string& flag);
bool checkIntFlag(int argc, char** argv, const std::string& flag, int defaultValue);

#endif // UTILS_H