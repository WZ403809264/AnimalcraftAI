#ifndef IDLE_H
#define IDLE_H

#ifdef _WIN32
  #include <windows.h>
  inline void Idle(void) {
    Sleep(1);
  }
#else
  #include <unistd.h>
  inline void Idle(void) {
    usleep(1000);
  }
#endif

#endif
