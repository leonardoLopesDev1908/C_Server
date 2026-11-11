#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#ifdef _WIN32

#include <windows.h>

size_t get_max_threads()
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);

  return sysinfo.dwNumberOfProcessors; 
}

#elif __linux__

#include <unistd.h>

long get_max_threads()
{
  return sysconf(_SC_NPROCESSORS_ONLN);
}

#endif

#endif
