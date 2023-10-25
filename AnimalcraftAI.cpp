#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "ucci.h"
#include "resource.h"

#pragma comment(lib, "winmm.lib")

// ���ںͻ�ͼ����
const int WINDOW_STYLES = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
const int MASK_COLOR = RGB(0, 255, 0);
const int SQUARE_SIZE = 80;
const int BOARD_EDGE = 8;
const int BOARD_WIDTH = BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE;
const int BOARD_HEIGHT = BOARD_EDGE + SQUARE_SIZE * 7 + BOARD_EDGE;

const char name[24][5]={"��","Ѩ","��","��","��","Ѩ","��","��","��","ʨ","��","��","��","��","è","��","��","ʨ","��","��","��","��","è","��"};//��������
//���� 
const int RANK_TOP = 3;//��ʼ��
const int RANK_BOTTOM = 9;//�յ���
const int FILE_LEFT = 3;//��ʼ��
const int FILE_RIGHT = 11;//�յ��� 
//����Ϊ���ӱ�ţ�0-6��Ӧ7������
const int PIECE_ELEPHANT = 0;
const int PIECE_LION = 1;
const int PIECE_TIGER = 2;
const int PIECE_LEOPARD = 3;
const int PIECE_WOLF = 4;
const int PIECE_DOG = 5;
const int PIECE_CAT = 6;
const int PIECE_MOUSE = 7;
//���� 
const int MAX_GEN_MOVES = 35; // ���������߷���
const int MAX_MOVES = 1000;     // ������ʷ�߷���

int t2=2000,depth=99999999,t,t3;//ʱ����ƺ���ȿ���
bool fenxi=0,player[2],turn,ranghu=0;

// �ж������Ƿ��������е�����
static const char ccInBoard[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// �ж������Ƿ��ھŹ�������
static const char ccInFort[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 4, 0, 0, 3, 3, 3, 0, 0, 2, 0, 0, 0, 0,
  0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 2, 5, 0, 0, 0, 0,
  0, 0, 0, 4, 0, 0, 3, 3, 3, 0, 0, 2, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const bool ccCanJump[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// ����
static const char ccDelta[4] = {-16, -1, 1, 16};
// ���Ӳ���
static const char ccJumpDelta[4] = {-48,-4,4,48};

// ���̳�ʼ����
static int cucpcStartup[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0,10, 0, 8, 0, 0, 0,23, 0,17, 0, 0, 0, 0,
  0, 0, 0, 0,14, 0, 0, 0, 0, 0,21, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,12, 0, 0, 0,19, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,11, 0, 0, 0,20, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0,13, 0, 0, 0, 0, 0,22, 0, 0, 0, 0, 0,
  0, 0, 0, 9, 0,15, 0, 0, 0,16, 0,18, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
// �ж������Ƿ���������
inline bool IN_BOARD(int sq) {
  return ccInBoard[sq] != 0;
}

// ��ø��ӵĺ�����
inline int RANK_Y(int sq) {
  return sq >> 4;
}

// ��ø��ӵ�������
inline int FILE_X(int sq) {
  return sq & 15;
}

// ����������ͺ������ø���
inline int COORD_XY(int x, int y) {
  return x + (y << 4);
}

//
inline int PIECE_NAME(int pc) {
  return (pc&7);
}


// ��ת����
inline int SQUARE_FLIP(int sq) {
  return 254 - sq;
}

// ������ˮƽ����
inline int FILE_FLIP(int x) {
  return 14 - x;
}

// �����괹ֱ����
inline int RANK_FLIP(int y) {
  return 15 - y;
}

// �Ƿ��ں���
inline bool INRIVER(int sq) {
  return ccInFort[sq]==3;
}
// �Ƿ�����Ѩ�� 
inline bool INSHOUXUE(int sq,int tag) {
  if(tag==8) return ccInFort[sq]==1;
  return ccInFort[sq]==5;
}
// �Ƿ��������� 
inline bool INXIANJING(int sq,int tag) {
  if(tag==8) return ccInFort[sq]==2;
  return ccInFort[sq]==4;
}

// ��ú�ڱ��(������8��������16)
inline int SIDE_TAG(int sd) {
  return 8 + (sd << 3);
}

// ��öԷ���ڱ��
inline int OPP_SIDE_TAG(int sd) {
  return 16 - (sd << 3);
}

// ����߷������
inline int SRC(int mv) {
  return mv & 255;
}

// ����߷����յ�
inline int DST(int mv) {
  return mv >> 8;
}

// ���������յ����߷�
inline int MOVE(int sqSrc, int sqDst) {
  return sqSrc + sqDst * 256;
}

// �߷�ˮƽ����
inline int BLACK(int sq) {
	int x=FILE_X(sq),y=RANK_Y(sq);
	return COORD_XY(14-x,y);
  } 

// ��ʷ�߷���Ϣ(ռ4�ֽ�)
struct MoveStruct {
  int wmv,ucpcCaptured,wpc;
  int dwKey;

  void Set(int mv, int pcCaptured,int pc, int dwKey_) {
    wmv = mv;
    ucpcCaptured = pcCaptured;
    wpc=pc;
    dwKey = dwKey_;
  }
}; // mvs

// RC4������������
struct RC4Struct {
  int s[256];
  int x, y;

  void InitZero(void);   // �ÿ���Կ��ʼ��������������
  int Nextint(void) {  // ��������������һ���ֽ�
    int uc;
    x = (x + 1) & 255;
    y = (y + s[x]) & 255;
    uc = s[x];
    s[x] = s[y];
    s[y] = uc;
    return s[(s[x] + s[y]) & 255];
  }
  int NextLong(void) { // ���������������ĸ��ֽ�
    int uc0, uc1, uc2, uc3;
    uc0 = Nextint();
    uc1 = Nextint();
    uc2 = Nextint();
    uc3 = Nextint();
    return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
  }
};

// �ÿ���Կ��ʼ��������������
void RC4Struct::InitZero(void) {
  int i, j;
  int uc;

  x = y = j = 0;
  for (i = 0; i < 256; i ++) {
    s[i] = i;
  }
  for (i = 0; i < 256; i ++) {
    j = (j + s[i]) & 255;
    uc = s[i];
    s[i] = s[j];
    s[j] = uc;
  }
}

// Zobrist�ṹ
struct ZobristStruct {
  int dwKey, dwLock0, dwLock1;

  void InitZero(void) {                 // �������Zobrist
    dwKey = dwLock0 = dwLock1 = 0;
  }
  void InitRC4(RC4Struct &rc4) {        // �����������Zobrist
    dwKey = rc4.NextLong();
    dwLock0 = rc4.NextLong();
    dwLock1 = rc4.NextLong();
  }
  void Xor(const ZobristStruct &zobr) { // ִ��XOR����
    dwKey ^= zobr.dwKey;
    dwLock0 ^= zobr.dwLock0;
    dwLock1 ^= zobr.dwLock1;
  }
  void Xor(const ZobristStruct &zobr1, const ZobristStruct &zobr2) {
    dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
    dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
    dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
  }
};

// Zobrist��
static struct {
  ZobristStruct Player;
  ZobristStruct Table[16][256];
} Zobrist;

// ��ʼ��Zobrist��
static void InitZobrist(void) {
  int i, j;
  RC4Struct rc4;

  rc4.InitZero();
  Zobrist.Player.InitRC4(rc4);
  for (i = 0; i < 16; i ++) {
    for (j = 0; j < 256; j ++) {
      Zobrist.Table[i][j].InitRC4(rc4);
    }
  }
}
// ����ṹ
struct PositionStruct {
  bool sdPlayer;                   // �ֵ�˭�ߣ�0=�췽��1=�ڷ�
  int ucpcSquares[256];            // �����ϵ�����
  int nDistance, nMoveNum, nMoveNum2;         // ������ڵ�Ĳ�������ʷ�߷���
  MoveStruct mvsList[MAX_MOVES];   // ��ʷ�߷���Ϣ�б�
  ZobristStruct zobr;              // Zobrist
  bool CanJump(int src,int dst)
  {
  	  if(PIECE_NAME(ucpcSquares[src])==PIECE_LION||PIECE_NAME(ucpcSquares[src])==PIECE_TIGER)
  	  {
  	  	if(!ccCanJump[src]||!ccCanJump[dst]) return 0;
  	  	  for(int i=0;i<=3;i++)
  	  	  {
  	  	  	  if(dst-src==ccJumpDelta[i])
  	  	  	  {
		  	  	  	  	for(int j=src+ccDelta[i];j!=dst&&IN_BOARD(j);j+=ccDelta[i])
		  	  	  	  	{
  	  	  	  		        if(ucpcSquares[j]==PIECE_MOUSE+24-(ucpcSquares[src]-PIECE_NAME(ucpcSquares[src]))||!INRIVER(j)) return 0;
						}
						return 1;
				}
		  }
		  return 0;
	  }
  	  return 0;
  }
  bool CanMove(int src,int dst)
  {
  	  if(PIECE_NAME(ucpcSquares[src])==PIECE_MOUSE)
  	  {
  	  	  for(int i=0;i<=3;i++)
  	  	  {
  	  	  	  if(dst-src==ccDelta[i])
  	  	  	  {
					return 1;
				}
		  }
		  return 0;
		}
  	  	if(INRIVER(dst)) return 0;
  	  	  for(int i=0;i<=3;i++)
  	  	  {
  	  	  	  if(dst-src==ccDelta[i])
  	  	  	  {
					return 1;
				}
		  }
		  return 0;
  }
  bool CanEat(int src,int dst)
  {
  	if(ucpcSquares[dst]==0) return 0;
	  if(ucpcSquares[src]==0||ucpcSquares[dst]==0) return 0;
  	int as=PIECE_NAME(ucpcSquares[src]),bs=PIECE_NAME(ucpcSquares[dst]);
  	if(INXIANJING(dst,ucpcSquares[dst]-bs)) return 1;
  	  if(as==PIECE_MOUSE&&bs==PIECE_ELEPHANT)
  	  {
  	  	  if(INRIVER(src)&&!INRIVER(dst)) return 0;
  	  	  return 1;
	}
  	  if(as==PIECE_ELEPHANT&&bs==PIECE_MOUSE) return 0;
  	  return as<=bs;
  }
  void ClearBoard(void) {         // �������
    sdPlayer=FALSE;
	nDistance = 0;
    memset(ucpcSquares, 0, sizeof(ucpcSquares));
    zobr.InitZero();
  }
  void SetIrrev(void) {           // ���(��ʼ��)��ʷ�߷���Ϣ
    mvsList[0].Set(0, 0,0, zobr.dwKey);
    nMoveNum = 1;
  }
  void SetIrrev2(void) {           // ���(��ʼ��)��ʷ�߷���Ϣ
    nMoveNum2 = 1;
  }
  void Startup(void);             // ��ʼ������
  void ChangeSide(void) {         // �������ӷ�
    sdPlayer = !sdPlayer;
    zobr.Xor(Zobrist.Player);
  }
  void AddPiece(int sq, int pc) { // �������Ϸ�һö����
    ucpcSquares[sq] = pc;
    // �췽�ӷ֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)����
    if (pc < 16) {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    }
  }
  void DelPiece(int sq, int pc) { // ������������һö����
    ucpcSquares[sq] = 0;
    // �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
    if (pc < 16) {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    }
  }
  bool Captured(void) const {     // ��һ���Ƿ����
    return mvsList[nMoveNum - 1].ucpcCaptured != 0;
  }
  int MovePiece(int mv);                      // ��һ���������
  void UndoMovePiece(int mv, int pcCaptured); // ������һ���������
  bool MakeMove(int mv);                      // ��һ����
  void UndoMakeMove(void) {                   // ������һ����
    nDistance --;
    nMoveNum --;
    nMoveNum2 --;
    ChangeSide();
    UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
  }
  void NullMove(void) {                       // ��һ���ղ� 
    int dwKey;
    dwKey = zobr.dwKey;
    ChangeSide();
    mvsList[nMoveNum].Set(0, 0,0, dwKey);
    nMoveNum ++;
    nMoveNum2 ++;
    nDistance ++;
  }
  void UndoNullMove(void) {                   // ������һ���ղ�
    nDistance --;
    nMoveNum --;
    nMoveNum2 --;
    ChangeSide();
  }
  // ���������߷������"bCapture"Ϊ"1"��ֻ���ɳ����߷�
  int GenerateMoves(int *mvs, bool bCapture = 0);
  bool LegalMove(int mv);               // �ж��߷��Ƿ����
  bool IsMate(void);
  bool RepWuLai(void);
  bool RepWuSong(void);
  bool RepStatus(void);
  void Mirror(PositionStruct &posMirror) const; // �Ծ��澵��
};

// ��ʼ������
void PositionStruct::Startup(void) {
  int sq, pc;
  ClearBoard();
  for (sq = 0; sq < 256; sq ++) {
  	if(IN_BOARD(sq))
  	{
	    pc = cucpcStartup[sq];
	    if (pc != 0) {
	      AddPiece(sq, pc);
	    }
	}
  }
  SetIrrev();
  SetIrrev2();
}

// ��һ���������
int PositionStruct::MovePiece(int mv) {
  int sqSrc, sqDst, pc, pcCaptured;
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pcCaptured = ucpcSquares[sqDst];
  if (pcCaptured != 0) {
    DelPiece(sqDst, pcCaptured);
  }
  pc = ucpcSquares[sqSrc];
  DelPiece(sqSrc, pc);
  AddPiece(sqDst, pc);
  return pcCaptured;
}

// ������һ���������
void PositionStruct::UndoMovePiece(int mv, int pcCaptured) {
  int sqSrc, sqDst, pc;
  sqSrc = SRC(mv);
  sqDst = DST(mv);
  pc = ucpcSquares[sqDst];
  DelPiece(sqDst, pc);
  AddPiece(sqSrc, pc);
  if (pcCaptured != 0) {
    AddPiece(sqDst, pcCaptured);
  }
}

// ��һ����
bool PositionStruct::MakeMove(int mv) {
  int pcCaptured;
  int dwKey;

  dwKey = zobr.dwKey;

  pcCaptured = MovePiece(mv);
  ChangeSide();
  mvsList[nMoveNum].Set(mv, pcCaptured,ucpcSquares[DST(mv)], dwKey);
  nMoveNum ++;
  nMoveNum2 ++;
  nDistance ++;
  return 1;
}
// "GenerateMoves"����
const bool GEN_CAPTURE = 1;

// ���������߷������"bCapture"Ϊ"1"��ֻ���ɳ����߷�
int PositionStruct::GenerateMoves(int *mvs, bool bCapture)
{
  int  nGenMoves,  sqSrc, sqDst;
  int pcSelfSide, pcOppSide, pcSrc, pcDst;
  // ���������߷�����Ҫ�������¼������裺

  nGenMoves = 0;
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
  	if(!IN_BOARD(sqSrc)) continue;
    // 1. �ҵ�һ���������ӣ����������жϣ�
    pcSrc = ucpcSquares[sqSrc];
    if ((pcSrc & pcSelfSide) == 0) {
      continue;
    }
	int delta;
    // 2. ��������ȷ���߷�
    for(delta=0;delta<=3;delta++)
    {
    	sqDst=sqSrc+ccDelta[delta];
    	if(!IN_BOARD(sqDst)||INSHOUXUE(sqDst,pcSelfSide)) continue;
    	pcDst = ucpcSquares[sqDst];
    	if(!CanMove(sqSrc,sqDst))continue;
    	if(!ucpcSquares[sqDst]&&bCapture) continue;
        if (ucpcSquares[sqDst] ? ((pcDst & pcOppSide) != 0  && CanEat(sqSrc,sqDst) ): (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
	}
	for(delta=0;delta<=3;delta++)
	{
    	sqDst=sqSrc+ccJumpDelta[delta];
    	if(!IN_BOARD(sqDst)||INSHOUXUE(sqDst,pcSelfSide)) continue;
    	pcDst = ucpcSquares[sqDst];
    	if(!CanJump(sqSrc,sqDst))continue;
    	if(!ucpcSquares[sqDst]&&bCapture) continue;
        if (ucpcSquares[sqDst] ?  ((pcDst & pcOppSide) != 0  && CanEat(sqSrc,sqDst) ): (pcDst & pcSelfSide) == 0) {
          mvs[nGenMoves] = MOVE(sqSrc, sqDst);
          nGenMoves ++;
        }
	}
  }
  return nGenMoves;
}

// �ж��߷��Ƿ����
bool PositionStruct::LegalMove(int mv){
  int sqSrc, sqDst;
  int pcSelfSide,pcOppSide, pcSrc, pcDst;
  // �ж��߷��Ƿ�Ϸ�����Ҫ�������µ��жϹ��̣�

  // 1. �ж���ʼ���Ƿ����Լ�������
  sqSrc = SRC(mv);
  pcSrc = ucpcSquares[sqSrc];
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  if ((pcSrc & pcSelfSide) == 0) {
    return 0;
  }

  // 2. �ж�Ŀ����Ƿ����Լ�������
  sqDst = DST(mv);
  pcDst = ucpcSquares[sqDst];
  if ((pcDst & pcSelfSide) != 0) {
    return 0;
  }
  int delta;
  // 2. ��������ȷ���߷�
    for(delta=0;delta<=3;delta++)
    {
    	int sqDst2=sqSrc+ccDelta[delta];
    	if(!IN_BOARD(sqDst)||INSHOUXUE(sqDst,pcSelfSide)||sqDst2!=sqDst) continue;
    	pcDst = ucpcSquares[sqDst];
    	if(!CanMove(sqSrc,sqDst))continue;
        if (ucpcSquares[sqDst] ? ((pcDst & pcOppSide) != 0  && CanEat(sqSrc,sqDst) ): (pcDst & pcSelfSide) == 0) {
        	return 1;
        }
	}
    for(delta=0;delta<=3;delta++)
    {
    	int sqDst2=sqSrc+ccJumpDelta[delta];
    	if(!IN_BOARD(sqDst)||INSHOUXUE(sqDst,pcSelfSide)||sqDst2!=sqDst) continue;
    	pcDst = ucpcSquares[sqDst];
    	if(!CanJump(sqSrc,sqDst))continue;
        if (ucpcSquares[sqDst] ? ((pcDst & pcOppSide) != 0  && CanEat(sqSrc,sqDst) ): (pcDst & pcSelfSide) == 0) {
        	return 1;
        }
	}
	return 0;
}

// �ж��Ƿ�ɱ
bool PositionStruct::IsMate(void) {
	if(((ucpcSquares[99]>=8&&ucpcSquares[99]<=23)&&!sdPlayer)||((ucpcSquares[107]>=8&&ucpcSquares[107]<=23)&&sdPlayer)) return 1;
	return 0;
}
bool PositionStruct::RepWuLai(void){
	if(nMoveNum<=15) return 0;
	if(LegalMove(MOVE(DST(mvsList[nMoveNum-2].wmv),SRC(mvsList[nMoveNum-1].wmv)))) return 0;
	int count[24][256];
	memset(count,0,sizeof(count));
	for(int i=nMoveNum-3;i>=nMoveNum-16&&i>=1;i-=2)
	{
		if(INXIANJING(DST(mvsList[i].wmv),16)||INXIANJING(DST(mvsList[i].wmv),8))
		{
			return 0;
		}
		count[mvsList[i].wpc][DST(mvsList[i].wmv)]++;
		if(count[mvsList[i].wpc][DST(mvsList[i].wmv)]>=3&&DST(mvsList[i].wmv)==DST(mvsList[nMoveNum-1].wmv))
		{
			return 1;
		}
	}
	return 0;
}
bool PositionStruct::RepWuSong(void){
	if(nMoveNum<=35) return 0;
	int animal=mvsList[nMoveNum-1].wpc,dst=DST(mvsList[nMoveNum-1].wmv),count[6],qigenum=0;
	memset(count,0,sizeof(count));
	for(int i=nMoveNum-3;i>=nMoveNum-36&&i>=0;i-=2)
	{
		if(INXIANJING(DST(mvsList[i].wmv),16)||INXIANJING(DST(mvsList[i].wmv),8))
		{
			return 0;
		}
		if(mvsList[i].wpc!=animal) return 0;
		bool rep=0;
		for(int j=0;j<qigenum;j++)
		{
			if(DST(mvsList[i].wmv)==count[j])
			{
				rep=1;
				break;
			}
		}
		if(rep==0) count[qigenum++]=DST(mvsList[i].wmv);
		if(qigenum>5) return 0;
	}
	for(int j=0;j<qigenum;j++)
	{
		if(dst==count[j])
		{
			return 1;
		}
	}
	return 0;
}
bool PositionStruct::RepStatus(void) {

	if(RepWuLai())
	{
		return 1;
	}
	if(RepWuSong())
	{
		return 1;
	}

	return 0;
}

static PositionStruct pos; // ����ʵ��

// ��ͼ�ν����йص�ȫ�ֱ���
static struct {
  HINSTANCE hInst;                              // Ӧ�ó�����ʵ��
  HWND hWnd;                                    // �����ھ��
  HDC hdc, hdcTmp;                              // �豸�����ֻ��"ClickSquare"��������Ч
  HBITMAP bmpBoard, bmpSelected, bmpTrap, bmpDen, bmpPieces[24]; // ��ԴͼƬ���
  int sqSelected, mvLast;                       // ѡ�еĸ��ӣ���һ����
  BOOL bGameOver;                     // �Ƿ�ת���̣��Ƿ���Ϸ����(���ü�������ȥ)
} Xqwl;

// ��ʼ�����
void Startup(void) {
  pos.Startup();
  Xqwl.sqSelected = Xqwl.mvLast = 0;
  Xqwl.bGameOver = FALSE;
}
//�������fen����
void fen_to_map(char fen[])
{
	Startup();
	pos.ClearBoard();
	int x,y,i;
	x=FILE_LEFT;
	y=RANK_TOP;
	for(i=0;y<=RANK_BOTTOM;i++)
	{
		if(fen[i]=='/'||fen[i]==' ')
		{
			x=FILE_LEFT;
			y++;
		}
		else if(fen[i]>='0'&&fen[i]<='9')
		{
			for(int j=1;j<=fen[i]-'0';j++)
			{
				x++;
			}
		}
		else if(fen[i]!=' ')
		{
			switch(fen[i])
			{
				case 'w': pos.AddPiece(COORD_XY(x,y),20); break;
				case 'p': pos.AddPiece(COORD_XY(x,y),19); break;
				case 't': pos.AddPiece(COORD_XY(x,y),18); break;
				case 'l': pos.AddPiece(COORD_XY(x,y),17); break;
				case 'e': pos.AddPiece(COORD_XY(x,y),16); break;
				case 'd': pos.AddPiece(COORD_XY(x,y),21); break;
				case 'c': pos.AddPiece(COORD_XY(x,y),22); break;
				case 'm': pos.AddPiece(COORD_XY(x,y),23); break;
				case 'W': pos.AddPiece(COORD_XY(x,y),12); break;
				case 'P': pos.AddPiece(COORD_XY(x,y),11); break;
				case 'T': pos.AddPiece(COORD_XY(x,y),10); break;
				case 'L': pos.AddPiece(COORD_XY(x,y),9); break;
				case 'E': pos.AddPiece(COORD_XY(x,y),8); break;
				case 'D': pos.AddPiece(COORD_XY(x,y),13); break;
				case 'C': pos.AddPiece(COORD_XY(x,y),14); break;
				case 'M': pos.AddPiece(COORD_XY(x,y),15); break;
			}
			x++;
		}
	}
	if(fen[i]!='r') pos.ChangeSide();
	pos.SetIrrev();
	pos.SetIrrev2();
	Xqwl.sqSelected = Xqwl.mvLast = 0;
	Xqwl.bGameOver = 0;
}



// �������йص�ȫ�ֱ���
static struct {
  int mvResult;                  // �����ߵ���
  char fen[1024];
} Search;



void map_to_fen(void)
{
	strcpy(Search.fen,"\0");
	int x,y,empty=0;
	for(y=RANK_TOP;y<=RANK_BOTTOM;y++)
	{
		for(x=FILE_LEFT;x<=FILE_RIGHT;x++)
		{
			int sq=COORD_XY(x,y);
			if(pos.ucpcSquares[sq]==0)
			{
				empty++;
			}
			else
			{
				if(empty>=1)
				{
					char aa[3];
					aa[0]=empty+'0';
					aa[1]='\0';
					strcat(Search.fen,aa);
					empty=0;
				}
				switch(pos.ucpcSquares[sq])
				{
					case 8: strcat(Search.fen,"E"); break;
					case 9: strcat(Search.fen,"L"); break;
					case 10: strcat(Search.fen,"T"); break;
					case 11: strcat(Search.fen,"P"); break;
					case 12: strcat(Search.fen,"W"); break;
					case 13: strcat(Search.fen,"D"); break;
					case 14: strcat(Search.fen,"C"); break;
					case 15: strcat(Search.fen,"M"); break;
					case 16: strcat(Search.fen,"e"); break;
					case 17: strcat(Search.fen,"l"); break;
					case 18: strcat(Search.fen,"t"); break;
					case 19: strcat(Search.fen,"p"); break;
					case 20: strcat(Search.fen,"w"); break;
					case 21: strcat(Search.fen,"d"); break;
					case 22: strcat(Search.fen,"c"); break;
					case 23: strcat(Search.fen,"m"); break;
				}
			}
		}
		if(empty>=1)
		{
			char aa[3];
			aa[0]=empty+'0';
			aa[1]='\0';
			strcat(Search.fen,aa);
			empty=0;
		}
		if(y<RANK_BOTTOM) strcat(Search.fen,"/");
	}
	if(pos.sdPlayer) strcat(Search.fen," b");
	else strcat(Search.fen," r");
}
void map_to_fen2(int undomoves)
{
	int i;
	for(i=0;i<undomoves;i++)
	{
		pos.UndoMakeMove();
	}
	strcpy(Search.fen,"\0");
	int x,y,empty=0;
	for(y=RANK_TOP;y<=RANK_BOTTOM;y++)
	{
		for(x=FILE_LEFT;x<=FILE_RIGHT;x++)
		{
			int sq=COORD_XY(x,y);
			if(pos.ucpcSquares[sq]==0)
			{
				empty++;
			}
			else
			{
				if(empty>=1)
				{
					char aa[3];
					aa[0]=empty+'0';
					aa[1]='\0';
					strcat(Search.fen,aa);
					empty=0;
				}
				switch(pos.ucpcSquares[sq])
				{
					case 8: strcat(Search.fen,"E"); break;
					case 9: strcat(Search.fen,"L"); break;
					case 10: strcat(Search.fen,"T"); break;
					case 11: strcat(Search.fen,"P"); break;
					case 12: strcat(Search.fen,"W"); break;
					case 13: strcat(Search.fen,"D"); break;
					case 14: strcat(Search.fen,"C"); break;
					case 15: strcat(Search.fen,"M"); break;
					case 16: strcat(Search.fen,"e"); break;
					case 17: strcat(Search.fen,"l"); break;
					case 18: strcat(Search.fen,"t"); break;
					case 19: strcat(Search.fen,"p"); break;
					case 20: strcat(Search.fen,"w"); break;
					case 21: strcat(Search.fen,"d"); break;
					case 22: strcat(Search.fen,"c"); break;
					case 23: strcat(Search.fen,"m"); break;
				}
			}
		}
		if(empty>=1)
		{
			char aa[3];
			aa[0]=empty+'0';
			aa[1]='\0';
			strcat(Search.fen,aa);
			empty=0;
		}
		if(y<RANK_BOTTOM) strcat(Search.fen,"/");
	}
	if(pos.sdPlayer) strcat(Search.fen," b");
	else strcat(Search.fen," r");
	strcat(Search.fen," moves");
	for(i=0;i<undomoves;i++)
	{
		int mv=pos.mvsList[pos.nMoveNum].wmv; 
		pos.MakeMove(mv);
		char tofour[15]="\0";
		sprintf(tofour," %c%c%c%c",FILE_X(SRC(mv))+'a'-3,RANK_Y(SRC(mv))+'0'-3,FILE_X(DST(mv))+'a'-3,RANK_Y(DST(mv))+'0'-3);
		strcat(Search.fen,tofour);
	}
}

// TransparentBlt �������������������ԭ������ Windows 98 ����Դй©������
static void TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,
    HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent) {
  HDC hImageDC, hMaskDC;
  HBITMAP hOldImageBMP, hImageBMP, hOldMaskBMP, hMaskBMP;

  hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
  hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
  hImageDC = CreateCompatibleDC(hdcDest);
  hMaskDC = CreateCompatibleDC(hdcDest);
  hOldImageBMP = (HBITMAP) SelectObject(hImageDC, hImageBMP);
  hOldMaskBMP = (HBITMAP) SelectObject(hMaskDC, hMaskBMP);

  if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc) {
    BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
  } else {
    StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,
        hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
  }
  SetBkColor(hImageDC, crTransparent);
  BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
  SetBkColor(hImageDC, RGB(0,0,0));
  SetTextColor(hImageDC, RGB(255,255,255));
  BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
  SetBkColor(hdcDest, RGB(255,255,255));
  SetTextColor(hdcDest, RGB(0,0,0));
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
      hMaskDC, 0, 0, SRCAND);
  BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,
      hImageDC, 0, 0, SRCPAINT);

  SelectObject(hImageDC, hOldImageBMP);
  DeleteDC(hImageDC);
  SelectObject(hMaskDC, hOldMaskBMP);
  DeleteDC(hMaskDC);
  DeleteObject(hImageBMP);
  DeleteObject(hMaskBMP);
}
// ����͸��ͼƬ
inline void DrawTransBmp(HDC hdc, HDC hdcTmp, int xx, int yy, HBITMAP bmp) {
  SelectObject(hdcTmp, bmp);
  TransparentBlt2(hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, hdcTmp, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
}

// ��������
static void DrawBoard(HDC hdc) {
  int x, y, xx, yy, sq, pc;
  HDC hdcTmp;

  // ������
  hdcTmp = CreateCompatibleDC(hdc);
  SelectObject(hdcTmp, Xqwl.bmpBoard);
  BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, hdcTmp, 0, 0, SRCCOPY);
  // ������
  for (x = FILE_LEFT; x <= FILE_RIGHT; x ++) {
    for (y = RANK_TOP; y <= RANK_BOTTOM; y ++) {
        xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
        yy = BOARD_EDGE + (y - RANK_TOP) * SQUARE_SIZE;
      sq = COORD_XY(x, y);
      pc = pos.ucpcSquares[sq];
      	switch(ccInFort[sq])
      	{
      		case 1:
      		case 5:
      		{
      			DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpDen);
      			break;
			}
      		case 2:
      		case 4: DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpTrap);
		}
      if (pc != 0) {
        DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
      }
      if (sq == Xqwl.sqSelected || sq == SRC(Xqwl.mvLast) || sq == DST(Xqwl.mvLast)) {
        DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpSelected);
      }
    }
  }
  DeleteDC(hdcTmp);
}

// ������Դ����
inline void PlayResWav(int nResId) {
  PlaySound(MAKEINTRESOURCE(nResId), Xqwl.hInst, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
}

// "DrawSquare"����
const BOOL DRAW_SELECTED = TRUE;

// ���Ƹ���
static void DrawSquare(int sq, BOOL bSelected = FALSE) {
  int sqFlipped, xx, yy, pc;

  sqFlipped = sq;
  xx = BOARD_EDGE + (FILE_X(sqFlipped) - FILE_LEFT) * SQUARE_SIZE;
  yy = BOARD_EDGE + (RANK_Y(sqFlipped) - RANK_TOP) * SQUARE_SIZE;
  SelectObject(Xqwl.hdcTmp, Xqwl.bmpBoard);
  BitBlt(Xqwl.hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, Xqwl.hdcTmp, xx, yy, SRCCOPY);

  pc = pos.ucpcSquares[sq];
      	switch(ccInFort[sq])
      	{
      		case 1:
      		case 5:
      		{
      			DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpDen);
      			break;
			}
      		case 2:
      		case 4: DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpTrap);
		}
  if (pc != 0) {
    DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
  }
  if (bSelected) {
    DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpSelected);
  }
}
static void Computer(void)
{
	char input[1024];
	map_to_fen2((pos.nMoveNum-1>=40)?40:(pos.nMoveNum-1));
	sprintf(input,"fen %s",Search.fen);
	pipeStdHandle.LineOutput(input);
	Sleep(5);
	pipeStdHandle.LineOutput("go time 2000");
	Sleep(5);
	while (1) {
		if(pipeStdHandle.LineInput(input))
		{
			if(!strncmp(input,"bestmove ",9)) break;
		}
		else Sleep(1);
	}
	char *point=input;
	point+=9;
	int a=point[0]-'a'+3,b=point[1]-'0'+3,c=point[2]-'a'+3,d=point[3]-'0'+3;
	int mv=MOVE(COORD_XY(a,b),COORD_XY(c,d));
	Search.mvResult=mv;
}
// ���Ի�Ӧһ����
static void ResponseMove(void) {
  // ������һ����
  SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  Computer();
  SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  if(!pos.LegalMove(Search.mvResult))
  {
    // ����ֳ�ʤ������ô����ʤ�������������ҵ���������������ʾ��
    MessageBox(0,"ף����ȡ��ʤ����","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
  }
  // �����һ�����ѡ����
  DrawSquare(SRC(Xqwl.mvLast));
  DrawSquare(DST(Xqwl.mvLast));
  // �ѵ����ߵ����ǳ���
  pos.MakeMove(Search.mvResult);
  Xqwl.mvLast = Search.mvResult;
  DrawSquare(SRC(Xqwl.mvLast), DRAW_SELECTED);
  DrawSquare(DST(Xqwl.mvLast), DRAW_SELECTED);
 if (pos.IsMate()) {
    // ����ֳ�ʤ������ô����ʤ�������������ҵ���������������ʾ��
    MessageBox(0,"���ٽ�������","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
  }else {
  			if(pos.nMoveNum2>600)
  			{
    MessageBox(0,"������Ȼ�������ͣ�","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
			  }
        	else if(pos.RepStatus())
        	{
    // ����ֳ�ʤ������ô����ʤ�������������ҵ���������������ʾ��
    MessageBox(0,"ף����ȡ��ʤ����","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
			}
			else
			{
				if(pos.Captured()) pos.SetIrrev2();
				PlayResWav(IDR_ELEPHANT+PIECE_NAME(pos.mvsList[pos.nMoveNum-1].wpc));
			}
  }

}
bool training=0;
// ��������¼�����
static void ClickSquare(int sq) {
	char send[1024];
  int pc, mv;
  Xqwl.hdc = GetDC(Xqwl.hWnd);
  Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
  pc = pos.ucpcSquares[sq];

  if ((pc & SIDE_TAG(pos.sdPlayer)) != 0) {
    // �������Լ����ӣ���ôֱ��ѡ�и���
    if (Xqwl.sqSelected != 0) {
      DrawSquare(Xqwl.sqSelected);
    }
    Xqwl.sqSelected = sq;
    DrawSquare(sq, DRAW_SELECTED);
    if (Xqwl.mvLast != 0) {
      DrawSquare(SRC(Xqwl.mvLast));
      DrawSquare(DST(Xqwl.mvLast));
    }

  } else if (Xqwl.sqSelected != 0 && !Xqwl.bGameOver) {
    // �������Ĳ����Լ����ӣ�������ѡ����(һ�����Լ�����)����ô�������
    mv = MOVE(Xqwl.sqSelected, sq);

    if (pos.LegalMove(mv)) {
      if (pos.MakeMove(mv)) {

        Xqwl.mvLast = mv;
        DrawSquare(Xqwl.sqSelected, DRAW_SELECTED);
        DrawSquare(sq, DRAW_SELECTED);
        Xqwl.sqSelected = 0;



        // ����ظ�����
        
        if (pos.IsMate()) {
			MessageBox(0,"ף����ȡ��ʤ����","AnimalcraftAI",0);
          Xqwl.bGameOver = TRUE;
        } else if(pos.nMoveNum2>600)
  			{
    MessageBox(0,"������Ȼ�������ͣ�","AnimalcraftAI",0);
          Xqwl.bGameOver = 1;
			  }
			else if(pos.RepStatus())
			{
				if(pos.RepWuLai()) MessageBox(0,"Υ��������ѭ��","AnimalcraftAI",0);
				if(pos.RepWuSong()) MessageBox(0,"Υ������׽","AnimalcraftAI",0);
				pos.UndoMakeMove();
			}
			else {
				if(pos.Captured()) pos.SetIrrev2();
				PlayResWav(IDR_ELEPHANT+PIECE_NAME(pos.mvsList[pos.nMoveNum-1].wpc));
				if(training==0) ResponseMove();
			}
      }
    }
  }
  DeleteDC(Xqwl.hdcTmp);
  ReleaseDC(Xqwl.hWnd, Xqwl.hdc);
}
char FEN[100];

static BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  char input[1024];

  switch (uMsg) {
  // �˳�
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_CLOSE:
  	pipeStdHandle.LineOutput("stop");
	while(1)
	{
		if(pipeStdHandle.LineInput(input))
		{
			if(strncmp(input,"bestmove",8)==0) break;
		}
		else Idle();
	}
  	DestroyWindow(hWnd);
	break;
  case WM_INITDIALOG:
	map_to_fen2((pos.nMoveNum-1>=40)?40:(pos.nMoveNum-1));
	sprintf(input,"fen %s",Search.fen);
	pipeStdHandle.LineOutput(input);
	Sleep(5);
	pipeStdHandle.LineOutput("go infinite");
  	break;
  // �����¼�
  default:
  	if(pipeStdHandle.LineInput(input))
  	{
  		int depth=0,score=0;
  		char pv[200]="";
  		if(strncmp(input,"info",4)==0)
  		{
	  		for(int i=0;input[i]!='\0';i++)
	  		{
	  			if(strncmp(input+i,"depth",5)==0)
	  			{
	  				char *point=input+i+6;
	  				depth=ReadDigit(point,99);
				}
	  			else if(strncmp(input+i,"score",5)==0)
	  			{
	  				if(input[i+6]=='-')
	  				{
	  					char *point=input+i+7;
	  					score=-ReadDigit(point,99999);
					}
					else
					{
	  					char *point=input+i+6;
	  					score=ReadDigit(point,99999);
					}
				}
	  			else if(strncmp(input+i,"pv",2)==0)
	  			{
	  				int pvlen=0,j;
	  				for(j=i+2;input[j]!='\0';j+=5)
	  				{
	  					char thismv[20];
	  					pvlen++;
						int mv=MOVE(COORD_XY(input[j+1]-'a'+3,input[j+2]-'0'+3),COORD_XY(input[j+3]-'a'+3,input[j+4]-'0'+3));
	  					pos.MakeMove(mv);
	  					strcpy(thismv,name[pos.ucpcSquares[DST(mv)]]);
	  					switch(DST(mv)-SRC(mv))
	  					{
	  						case 1:
	  						case 4:
	  							strcat(thismv,"�� ");
	  							break;
	  						case -1:
	  						case -4:
	  							strcat(thismv,"�� ");
	  							break;
	  						case -48:
	  						case -16:
	  							strcat(thismv,"�� ");
	  							break;
	  						case 48:
	  						case 16:
	  							strcat(thismv,"�� ");
	  							break;
						}
	  					strcat(pv,thismv);
					}
	  				for(j=0;j<pvlen;j++)
	  				{
	  					pos.UndoMakeMove();
					}
				}
			}
		}
		char output[1024];
		if(depth!=0&&strlen(pv)!=0)
		{
			sprintf(output,"%d (%d) %s",depth,score,pv);
			HWND hwnd=GetDlgItem(hWnd,IDC_EDIT1);
			SetWindowText(hwnd,output);
		}
	}
	else Idle(); 
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return FALSE;
}

// �����¼���׽����
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char send[1024];
  int x, y;
  HDC hdc;
  RECT rect;
  PAINTSTRUCT ps;
  MSGBOXPARAMS mbp;

  switch (uMsg) {
  // �½�����
  case WM_CREATE:
    // ��������λ�úͳߴ�
    GetWindowRect(hWnd, &rect);
    x = rect.left;
    y = rect.top;
    rect.right = rect.left + BOARD_WIDTH;
    rect.bottom = rect.top + BOARD_HEIGHT;
    AdjustWindowRect(&rect, WINDOW_STYLES, TRUE);
    MoveWindow(hWnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    break;
  // �˳�
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  // �˵�����
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDM_FILE_RED:
	  PlayResWav(IDR_START);
	  training=0;
      Startup();
      fen_to_map(FEN);
	  map_to_fen();
      strcpy(send,"");
	  sprintf(send,"fen %s",Search.fen);
      pipeStdHandle.LineOutput(send);

      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
      break;
    case IDM_FILE_BLACK:
	  PlayResWav(IDR_START);
	  training=0;
      Startup();
      fen_to_map(FEN);
	  map_to_fen();
      strcpy(send,"");
	  sprintf(send,"fen %s",Search.fen);
      pipeStdHandle.LineOutput(send);

      ResponseMove();
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
      break;
    case IDM_FILE_TRAINING:
	  PlayResWav(IDR_START);
	  training=1;
      Startup();
      fen_to_map(FEN); 
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
      break;
    case IDM_UNDO:
      pos.UndoMakeMove();
	  pos.UndoMakeMove();
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
      break;
    case IDM_FILE_EXIT:
      DestroyWindow(Xqwl.hWnd);
      break;
    case IDM_STANDARD:
	{
		strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e1t r");
      fen_to_map(FEN); 
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
		break;
	}
    case IDM_HANDICAP:
	{
		strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e2 r");
      fen_to_map(FEN);
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
		break;
	}
    case IDM_LOADFEN:
    {
		if (!IsClipboardFormatAvailable(CF_TEXT))
		{
			break;
		}
		if (!OpenClipboard(NULL))
		{
			break;
		}
		HGLOBAL hMem = GetClipboardData(CF_TEXT);
		if (hMem != NULL)
		{
			LPTSTR lpStr = (LPTSTR)GlobalLock(hMem);
			if (lpStr != NULL)
			{
				strcpy(FEN,lpStr);
				GlobalUnlock(hMem);
  				Startup();
				fen_to_map(FEN);
				hdc = GetDC(Xqwl.hWnd);
			    DrawBoard(hdc);
			    ReleaseDC(Xqwl.hWnd, hdc);
			}
		}
		CloseClipboard();
		break;
	}
	case IDM_OUTPUTFEN:
	{
		map_to_fen();
		if(OpenClipboard(NULL))//�򿪼�����
		{
			HANDLE hClip;
			char* pBuf;
			EmptyClipboard();//��ռ�����
	
			//д������
			hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(Search.fen)+1);
			pBuf=(char*)GlobalLock(hClip);
			strcpy(pBuf,Search.fen);
			GlobalUnlock(hClip);//����
			SetClipboardData(CF_TEXT,hClip);//���ø�ʽ
	
			//�رռ�����
			CloseClipboard();
		}
		break;
	} 
	case IDD_ANALYZE_DIALOG:
	{
		MSG msg;
	  // �򿪴���
	  HWND hwnd = CreateDialogParam(Xqwl.hInst,MAKEINTRESOURCE(IDD_ANALYZE_DIALOG),Xqwl.hWnd,DialogProc,NULL);
	  if (hwnd == NULL) {
	  	break; 
	  }
		ShowWindow(hwnd,5);
		UpdateWindow(hwnd);
	  // ������Ϣ
	  while (GetMessage(&msg, NULL, 0, 0)) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	  }
		break;
	}
    break;
    }
    break;
  // ��ͼ
  case WM_PAINT:
    hdc = BeginPaint(Xqwl.hWnd, &ps);
    DrawBoard(hdc);
    EndPaint(Xqwl.hWnd, &ps);
    break;
  // �����
  case WM_LBUTTONDOWN:
    x = FILE_LEFT + (LOWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    y = RANK_TOP + (HIWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    if (x >= FILE_LEFT && x <= FILE_RIGHT && y >= RANK_TOP && y <= RANK_BOTTOM) {
      ClickSquare(COORD_XY(x, y));
    }
    break;
  // �����¼�
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return FALSE;
}

// װ����ԴͼƬ
inline HBITMAP LoadResBmp(int nResId) {
  return (HBITMAP) LoadImage(Xqwl.hInst, MAKEINTRESOURCE(nResId), IMAGE_BITMAP,0,0, LR_DEFAULTSIZE | LR_SHARED);
}

// ��ڹ���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	pipeStdHandle.Open("engine.exe");
	pipeStdHandle.LineOutput("uaci");
	int tt=clock();
	char input[1024];
	while (strcmp(input,"uaciok")!=0) {
		if(clock()-tt>=1000)
		{
			MessageBox(0,"�����ļ�����ʧ��","AnimalcraftAI",0);
			pipeStdHandle.LineOutput("quit");
			pipeStdHandle.Close();
			return 0; 
		}
		pipeStdHandle.LineInput(input);
	}
	
	PlayResWav(IDR_START);
	
  int i;
  MSG msg;
  WNDCLASSEX wce;

  // ��ʼ��ȫ�ֱ���
  srand((DWORD) time(NULL));
  InitZobrist();
  Xqwl.hInst = hInstance;
  strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e1t r");
  fen_to_map(FEN);

  // װ��ͼƬ
  Xqwl.bmpBoard = LoadResBmp(IDB_BOARD);
  Xqwl.bmpSelected = LoadResBmp(IDB_SELECTED);
  for (i = PIECE_ELEPHANT; i <= PIECE_MOUSE; i ++) {
    Xqwl.bmpPieces[SIDE_TAG(0) + i] = LoadResBmp(IDB_RE + i);
    Xqwl.bmpPieces[SIDE_TAG(1) + i] = LoadResBmp(IDB_BE + i);
  }
    Xqwl.bmpTrap = LoadResBmp(IDB_TRAP);
    Xqwl.bmpDen = LoadResBmp(IDB_DEN);

  // ���ô���
  wce.cbSize = sizeof(WNDCLASSEX);
  wce.style = 0;
  wce.lpfnWndProc = (WNDPROC) WndProc;
  wce.cbClsExtra = wce.cbWndExtra = 0;
  wce.hInstance = hInstance;
  wce.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, LR_SHARED);
  wce.hCursor = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
  wce.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
  wce.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
  wce.lpszClassName = "AnimalcraftAI";
  wce.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);
  RegisterClassEx(&wce);

  // �򿪴���
  Xqwl.hWnd = CreateWindow("AnimalcraftAI", "AnimalcraftAI", WINDOW_STYLES,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  if (Xqwl.hWnd == NULL) {
    return 0;
  }
  ShowWindow(Xqwl.hWnd, nCmdShow);
  UpdateWindow(Xqwl.hWnd);


	

  // ������Ϣ
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

	pipeStdHandle.LineOutput("quit");
	pipeStdHandle.Close();
  return msg.wParam;
}
