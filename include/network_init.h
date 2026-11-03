#ifndef NETWORK_INIT_H
#define NETWORK_INIT_H

#ifdef _WIN32

#include <WinSock2.h>
#include <Windows.h>
#include <ws2tcpip.h>



int platform_init()
{
  WSADATA wsaData{};
  return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void platform_cleanup() { WSACleanup(); }

#elif __linux__

#include <arpa/inet.h>
#include <netdb.h>

void platform_init() {}
void platform_cleanup() {}

#endif
