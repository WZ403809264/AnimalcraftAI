#ifndef BOOL_H
#define BOOL_H

#ifndef _WIN32
#define __int64 long long
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef int Bool;
typedef char BoolChar;
typedef short BoolShort;
typedef long BoolLong;
typedef long long BoolLongLong;

inline Bool Eqv(Bool bArg1, Bool bArg2) {
  return bArg1 ? bArg2 : !bArg2;
}

inline Bool Xor(Bool bArg1, Bool bArg2) {
  return bArg1 ? !bArg2 : bArg2;
}

#endif
