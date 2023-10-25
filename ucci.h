#include "bool.h"
#include <stdio.h>
#include <string.h>
#include "idle.h"
#include "pipe.h"

static int ReadDigit(char *lpLinePtr, int nMaxValue) {
  int nRetValue;
  nRetValue = 0;
  for (; ; ) {
    if (*lpLinePtr >= '0' && *lpLinePtr <= '9') {
      nRetValue *= 10;
      nRetValue += *lpLinePtr - '0';
      lpLinePtr ++;
      if (nRetValue > nMaxValue) {
        nRetValue = nMaxValue;
      }
    } else {
      break;
    }
  }
  return nRetValue;
}

static PipeStruct pipeStdHandle;

void BootLine(void) {
  pipeStdHandle.Open();
}
