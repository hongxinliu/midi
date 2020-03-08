#pragma once

#include <string>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

inline std::string binary_path() {
  const unsigned int bufferSize = 512;
  std::string path;
  path.resize(bufferSize + 1);

#if defined(_WIN32)
  ::GetModuleFileName(NULL, &path[0], bufferSize);

#elif defined(__linux__)
  // Get the process ID.
  int pid = getpid();

  // Construct a path to the symbolic link pointing to the process executable.
  // This is at /proc/<pid>/exe on Linux systems (we hope).
  std::string link = "/proc/" + std::to_string(pid) + "/exe";

  // Read the contents of the link.
  int count = readlink(link.c_str(), &path[0], bufferSize);
  if (count == -1) {
    return std::string();
  }
  path[count] = '\0';

#elif defined(__APPLE__)
  if (_NSGetExecutablePath(&path[0], &bufferSize)) {
    path.resize(bufferSize);
    _NSGetExecutablePath(&path[0], &bufferSize);
  }

#else
#error Cannot yet find the executable on this platform
#endif

  return path;
}
