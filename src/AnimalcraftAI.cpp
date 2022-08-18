#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include "ucci.h"
#include "resource.h"

#pragma comment(lib, "winmm.lib")

// 窗口和绘图属性
const int WINDOW_STYLES = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
const int MASK_COLOR = RGB(0, 255, 0);
const int SQUARE_SIZE = 80;
const int BOARD_EDGE = 8;
const int BOARD_WIDTH = BOARD_EDGE + SQUARE_SIZE * 9 + BOARD_EDGE;
const int BOARD_HEIGHT = BOARD_EDGE + SQUARE_SIZE * 7 + BOARD_EDGE;

const char name[24][5]={"　","穴","阱","■","阱","穴","　","　","象","狮","虎","豹","狼","狗","猫","鼠","象","狮","虎","豹","狼","狗","猫","鼠"};//棋子名称
//常量 
const int RANK_TOP = 3;//起始行
const int RANK_BOTTOM = 9;//终点行
const int FILE_LEFT = 3;//起始列
const int FILE_RIGHT = 11;//终点列 
//以下为棋子编号，0-6对应7种棋子
const int PIECE_ELEPHANT = 0;
const int PIECE_LION = 1;
const int PIECE_TIGER = 2;
const int PIECE_LEOPARD = 3;
const int PIECE_WOLF = 4;
const int PIECE_DOG = 5;
const int PIECE_CAT = 6;
const int PIECE_MOUSE = 7;
//其它 
const int MAX_GEN_MOVES = 35; // 最大的生成走法数
const int MAX_MOVES = 1000;     // 最大的历史走法数

int t2=1000,depth=99999999,t,t3;//时间控制和深度控制
bool fenxi=0,player[2],turn,ranghu=0;

// 判断棋子是否在棋盘中的数组
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

// 判断棋子是否在九宫的数组
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

// 步长
static const char ccDelta[4] = {-16, -1, 1, 16};
// 跳河步长
static const char ccJumpDelta[4] = {-48,-4,4,48};

// 棋盘初始设置
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
// 判断棋子是否在棋盘中
inline bool IN_BOARD(int sq) {
  return ccInBoard[sq] != 0;
}

// 获得格子的横坐标
inline int RANK_Y(int sq) {
  return sq >> 4;
}

// 获得格子的纵坐标
inline int FILE_X(int sq) {
  return sq & 15;
}

// 根据纵坐标和横坐标获得格子
inline int COORD_XY(int x, int y) {
  return x + (y << 4);
}

//
inline int PIECE_NAME(int pc) {
  return (pc&7);
}


// 翻转格子
inline int SQUARE_FLIP(int sq) {
  return 254 - sq;
}

// 纵坐标水平镜像
inline int FILE_FLIP(int x) {
  return 14 - x;
}

// 横坐标垂直镜像
inline int RANK_FLIP(int y) {
  return 15 - y;
}

// 是否在河中
inline bool INRIVER(int sq) {
  return ccInFort[sq]==3;
}
// 是否在兽穴中 
inline bool INSHOUXUE(int sq,int tag) {
  if(tag==8) return ccInFort[sq]==1;
  return ccInFort[sq]==5;
}
// 是否在陷阱中 
inline bool INXIANJING(int sq,int tag) {
  if(tag==8) return ccInFort[sq]==2;
  return ccInFort[sq]==4;
}

// 获得红黑标记(红子是8，黑子是16)
inline int SIDE_TAG(int sd) {
  return 8 + (sd << 3);
}

// 获得对方红黑标记
inline int OPP_SIDE_TAG(int sd) {
  return 16 - (sd << 3);
}

// 获得走法的起点
inline int SRC(int mv) {
  return mv & 255;
}

// 获得走法的终点
inline int DST(int mv) {
  return mv >> 8;
}

// 根据起点和终点获得走法
inline int MOVE(int sqSrc, int sqDst) {
  return sqSrc + sqDst * 256;
}

// 走法水平镜像
inline int BLACK(int sq) {
	int x=FILE_X(sq),y=RANK_Y(sq);
	return COORD_XY(14-x,y);
  } 

// 历史走法信息(占4字节)
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

// RC4密码流生成器
struct RC4Struct {
  int s[256];
  int x, y;

  void InitZero(void);   // 用空密钥初始化密码流生成器
  int Nextint(void) {  // 生成密码流的下一个字节
    int uc;
    x = (x + 1) & 255;
    y = (y + s[x]) & 255;
    uc = s[x];
    s[x] = s[y];
    s[y] = uc;
    return s[(s[x] + s[y]) & 255];
  }
  int NextLong(void) { // 生成密码流的下四个字节
    int uc0, uc1, uc2, uc3;
    uc0 = Nextint();
    uc1 = Nextint();
    uc2 = Nextint();
    uc3 = Nextint();
    return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
  }
};

// 用空密钥初始化密码流生成器
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

// Zobrist结构
struct ZobristStruct {
  int dwKey, dwLock0, dwLock1;

  void InitZero(void) {                 // 用零填充Zobrist
    dwKey = dwLock0 = dwLock1 = 0;
  }
  void InitRC4(RC4Struct &rc4) {        // 用密码流填充Zobrist
    dwKey = rc4.NextLong();
    dwLock0 = rc4.NextLong();
    dwLock1 = rc4.NextLong();
  }
  void Xor(const ZobristStruct &zobr) { // 执行XOR操作
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

// Zobrist表
static struct {
  ZobristStruct Player;
  ZobristStruct Table[16][256];
} Zobrist;

// 初始化Zobrist表
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
// 局面结构
struct PositionStruct {
  bool sdPlayer;                   // 轮到谁走，0=红方，1=黑方
  int ucpcSquares[256];            // 棋盘上的棋子
  int nDistance, nMoveNum, nMoveNum2;         // 距离根节点的步数，历史走法数
  MoveStruct mvsList[MAX_MOVES];   // 历史走法信息列表
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
  void ClearBoard(void) {         // 清空棋盘
    sdPlayer=FALSE;
	nDistance = 0;
    memset(ucpcSquares, 0, sizeof(ucpcSquares));
    zobr.InitZero();
  }
  void SetIrrev(void) {           // 清空(初始化)历史走法信息
    mvsList[0].Set(0, 0,0, zobr.dwKey);
    nMoveNum = 1;
  }
  void SetIrrev2(void) {           // 清空(初始化)历史走法信息
    nMoveNum2 = 1;
  }
  void Startup(void);             // 初始化棋盘
  void ChangeSide(void) {         // 交换走子方
    sdPlayer = !sdPlayer;
    zobr.Xor(Zobrist.Player);
  }
  void AddPiece(int sq, int pc) { // 在棋盘上放一枚棋子
    ucpcSquares[sq] = pc;
    // 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
    if (pc < 16) {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    }
  }
  void DelPiece(int sq, int pc) { // 从棋盘上拿走一枚棋子
    ucpcSquares[sq] = 0;
    // 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
    if (pc < 16) {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    } else {
      zobr.Xor(Zobrist.Table[pc - 8][sq]);
    }
  }
  bool Captured(void) const {     // 上一步是否吃子
    return mvsList[nMoveNum - 1].ucpcCaptured != 0;
  }
  int MovePiece(int mv);                      // 搬一步棋的棋子
  void UndoMovePiece(int mv, int pcCaptured); // 撤消搬一步棋的棋子
  bool MakeMove(int mv);                      // 走一步棋
  void UndoMakeMove(void) {                   // 撤消走一步棋
    nDistance --;
    nMoveNum --;
    nMoveNum2 --;
    ChangeSide();
    UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
  }
  void NullMove(void) {                       // 走一步空步 
    int dwKey;
    dwKey = zobr.dwKey;
    ChangeSide();
    mvsList[nMoveNum].Set(0, 0,0, dwKey);
    nMoveNum ++;
    nMoveNum2 ++;
    nDistance ++;
  }
  void UndoNullMove(void) {                   // 撤消走一步空步
    nDistance --;
    nMoveNum --;
    nMoveNum2 --;
    ChangeSide();
  }
  // 生成所有走法，如果"bCapture"为"1"则只生成吃子走法
  int GenerateMoves(int *mvs, bool bCapture = 0);
  bool LegalMove(int mv);               // 判断走法是否合理
  bool IsMate(void);
  bool RepWuLai(void);
  bool RepWuSong(void);
  bool RepStatus(void);
  bool RepStatus2(void);
  void Mirror(PositionStruct &posMirror) const; // 对局面镜像
};

// 初始化棋盘
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

// 搬一步棋的棋子
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

// 撤消搬一步棋的棋子
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

// 走一步棋
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
// "GenerateMoves"参数
const bool GEN_CAPTURE = 1;

// 生成所有走法，如果"bCapture"为"1"则只生成吃子走法
int PositionStruct::GenerateMoves(int *mvs, bool bCapture)
{
  int  nGenMoves,  sqSrc, sqDst;
  int pcSelfSide, pcOppSide, pcSrc, pcDst;
  // 生成所有走法，需要经过以下几个步骤：

  nGenMoves = 0;
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  for (sqSrc = 0; sqSrc < 256; sqSrc ++) {
  	if(!IN_BOARD(sqSrc)) continue;
    // 1. 找到一个本方棋子，再做以下判断：
    pcSrc = ucpcSquares[sqSrc];
    if ((pcSrc & pcSelfSide) == 0) {
      continue;
    }
	int delta;
    // 2. 根据棋子确定走法
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

// 判断走法是否合理
bool PositionStruct::LegalMove(int mv){
  int sqSrc, sqDst;
  int pcSelfSide,pcOppSide, pcSrc, pcDst;
  // 判断走法是否合法，需要经过以下的判断过程：

  // 1. 判断起始格是否有自己的棋子
  sqSrc = SRC(mv);
  pcSrc = ucpcSquares[sqSrc];
  pcSelfSide = SIDE_TAG(sdPlayer);
  pcOppSide = OPP_SIDE_TAG(sdPlayer);
  if ((pcSrc & pcSelfSide) == 0) {
    return 0;
  }

  // 2. 判断目标格是否有自己的棋子
  sqDst = DST(mv);
  pcDst = ucpcSquares[sqDst];
  if ((pcDst & pcSelfSide) != 0) {
    return 0;
  }
  int delta;
  // 2. 根据棋子确定走法
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

// 判断是否被杀
bool PositionStruct::IsMate(void) {
	if(((ucpcSquares[99]>=8&&ucpcSquares[99]<=23)&&!sdPlayer)||((ucpcSquares[107]>=8&&ucpcSquares[107]<=23)&&sdPlayer)) return 1;
	return 0;
}
bool PositionStruct::RepWuLai(void){
	if(nMoveNum<=15) return 0;
	if(CanEat(DST(mvsList[nMoveNum-3].wmv),SRC(mvsList[nMoveNum-2].wmv))) return 0;
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

// 检测重复局面
bool PositionStruct::RepStatus2(void){
  bool bSelfSide;
  const MoveStruct *lpmvs,*lpmvs2;
  int nRecur=1;
  bSelfSide = 0;

  lpmvs = mvsList + nMoveNum - 1;
  lpmvs2 = mvsList + nMoveNum - 2;
  int src=DST(lpmvs->wmv),dst=DST(lpmvs2->wmv);
  while (lpmvs->wmv != 0 && lpmvs->ucpcCaptured == 0) {
    if (bSelfSide) {
      if (lpmvs->dwKey == zobr.dwKey) {
        nRecur --;
        if (nRecur == 0) {
           return CanEat(src,dst);
        }
      }
    }
    bSelfSide = !bSelfSide;
    lpmvs --;
  }
  return 0;
}
static PositionStruct pos; // 局面实例

// 与图形界面有关的全局变量
static struct {
  HINSTANCE hInst;                              // 应用程序句柄实例
  HWND hWnd;                                    // 主窗口句柄
  HDC hdc, hdcTmp;                              // 设备句柄，只在"ClickSquare"过程中有效
  HBITMAP bmpBoard, bmpSelected, bmpTrap, bmpDen, bmpPieces[24]; // 资源图片句柄
  int sqSelected, mvLast;                       // 选中的格子，上一步棋
  BOOL bGameOver;                     // 是否翻转棋盘，是否游戏结束(不让继续玩下去)
} Xqwl;

// 初始化棋局
void Startup(void) {
  pos.Startup();
  Xqwl.sqSelected = Xqwl.mvLast = 0;
  Xqwl.bGameOver = FALSE;
}
//读入局面fen代码
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



// 与搜索有关的全局变量
static struct {
  int mvResult;                  // 电脑走的棋
  char fen[100];
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

// TransparentBlt 的替代函数，用来修正原函数在 Windows 98 下资源泄漏的问题
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
// 绘制透明图片
inline void DrawTransBmp(HDC hdc, HDC hdcTmp, int xx, int yy, HBITMAP bmp) {
  SelectObject(hdcTmp, bmp);
  TransparentBlt2(hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, hdcTmp, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
}

// 绘制棋盘
static void DrawBoard(HDC hdc) {
  int x, y, xx, yy, sq, pc;
  HDC hdcTmp;

  // 画棋盘
  hdcTmp = CreateCompatibleDC(hdc);
  SelectObject(hdcTmp, Xqwl.bmpBoard);
  BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, hdcTmp, 0, 0, SRCCOPY);
  // 画棋子
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

// 播放资源声音
inline void PlayResWav(int nResId) {
  PlaySound(MAKEINTRESOURCE(nResId), Xqwl.hInst, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
}

// "DrawSquare"参数
const BOOL DRAW_SELECTED = TRUE;

// 绘制格子
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
	char *point=input;
	strcpy(input,"\0");
	map_to_fen();
	sprintf(input,"fen %s",Search.fen);
	pipeStdHandle.LineOutput(input);
	bool hasban=0;
	strcpy(input,"banmoves");
	
	int mvs[MAX_GEN_MOVES],moves=pos.GenerateMoves(mvs);
	for(int i=0;i<moves;i++)
	{
		pos.MakeMove(mvs[i]);
		if(pos.RepStatus())
		{
			char input2[10];
			sprintf(input2," %c%c%c%c",FILE_X(SRC(mvs[i]))+'a'-3,RANK_Y(SRC(mvs[i]))+'0'-3,FILE_X(DST(mvs[i]))+'a'-3,RANK_Y(DST(mvs[i]))+'0'-3);
			strcat(input,input2);
			hasban=1;
		}
		pos.UndoMakeMove();
	}
	if(hasban) pipeStdHandle.LineOutput(input);
	
	pipeStdHandle.LineOutput("go infinite");
	Sleep(t2);
	pipeStdHandle.LineOutput("stop");
	strcpy(input,"\0");
	while (strncmp(input,"bestmove ",9)!=0) {
		pipeStdHandle.LineInput(input);
	}
	point+=9;
	int a=point[0]-'a'+3,b=point[1]-'0'+3,c=point[2]-'a'+3,d=point[3]-'0'+3;
	int mv=MOVE(COORD_XY(a,b),COORD_XY(c,d));
	Search.mvResult=mv;
}
// 电脑回应一步棋
static void ResponseMove(void) {
  // 电脑走一步棋
  SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  Computer();
  SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  pos.MakeMove(Search.mvResult);
  // 清除上一步棋的选择标记
  DrawSquare(SRC(Xqwl.mvLast));
  DrawSquare(DST(Xqwl.mvLast));
  // 把电脑走的棋标记出来
  Xqwl.mvLast = Search.mvResult;
  DrawSquare(SRC(Xqwl.mvLast), DRAW_SELECTED);
  DrawSquare(DST(Xqwl.mvLast), DRAW_SELECTED);
 if (pos.IsMate()) {
    // 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
    MessageBox(0,"请再接再厉！","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
  }else {
  			if(pos.nMoveNum2>600)
  			{
    MessageBox(0,"超过自然限着作和！","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
			  }
        	else if(pos.RepStatus())
        	{
    // 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
    MessageBox(0,"祝贺你取得胜利！","AnimalcraftAI",0);
    Xqwl.bGameOver = TRUE;
			}
			else
			{
				PlayResWav(IDR_ELEPHANT+PIECE_NAME(pos.mvsList[pos.nMoveNum-1].wpc));
				if(pos.Captured()) pos.SetIrrev2();
			}
  }

}
bool training=0;
// 点击格子事件处理
static void ClickSquare(int sq) {
	char send[1024];
  int pc, mv;
  Xqwl.hdc = GetDC(Xqwl.hWnd);
  Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
  pc = pos.ucpcSquares[sq];

  if ((pc & SIDE_TAG(pos.sdPlayer)) != 0) {
    // 如果点击自己的子，那么直接选中该子
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
    // 如果点击的不是自己的子，但有子选中了(一定是自己的子)，那么走这个子
    mv = MOVE(Xqwl.sqSelected, sq);

    if (pos.LegalMove(mv)) {
      if (pos.MakeMove(mv)) {

        Xqwl.mvLast = mv;
        DrawSquare(Xqwl.sqSelected, DRAW_SELECTED);
        DrawSquare(sq, DRAW_SELECTED);
        Xqwl.sqSelected = 0;



        // 检查重复局面
        
        if (pos.IsMate()) {
			MessageBox(0,"祝贺你取得胜利！","AnimalcraftAI",0);
          Xqwl.bGameOver = TRUE;
        } else if(pos.nMoveNum2>600)
  			{
    MessageBox(0,"超过自然限着作和！","AnimalcraftAI",0);
          Xqwl.bGameOver = 1;
			  }
			else if(pos.RepStatus())
			{
				if(pos.RepWuLai()) MessageBox(0,"违例：无赖循环","AnimalcraftAI",0);
				if(pos.RepWuSong()) MessageBox(0,"违例：长捉","AnimalcraftAI",0);
				pos.UndoMakeMove();
			}
			else {
				PlayResWav(IDR_ELEPHANT+PIECE_NAME(pos.mvsList[pos.nMoveNum-1].wpc));
				if(pos.Captured()) pos.SetIrrev2();
				if(training==0) ResponseMove();
			}
      }
    }
  }
  DeleteDC(Xqwl.hdcTmp);
  ReleaseDC(Xqwl.hWnd, Xqwl.hdc);
}
static LRESULT CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int nd=0,vl=-10000,mv=0;
	bool print=0; 
	char pv[4];
	char input[1024],*point=input;
	pipeStdHandle.LineInput(input);
	if(strncmp(input,"info ",5)==0)
	{
		point+=5;
		if(strncmp(point,"depth ",6)==0)
		{
			point+=6;
			nd=ReadDigit(point,100);
			point++;
			if(strncmp(point,"score ",6)==0)
			{
				bool minus=0;
				point+=6;
				if(point[0]=='-')
				{
					point++;
					minus=1;
				}
				vl=ReadDigit(point,100000);
				if(minus) vl=-vl;
				point++;
				if(strncmp(point,"pv ",3)==0)
				{
					point+=3;
					int i=0;
					for(;i<=3;i++)
					{
						pv[i]=point[i];
					}
					mv=MOVE(COORD_XY(pv[0]-'a'+3,pv[1]-'0'+3),COORD_XY(pv[2]-'a'+3,pv[3]-'0'+3));
					print=1;
				}
			}
		}
	}
	if(print)
	{
		char aa[15];
		strcpy(aa,name[pos.ucpcSquares[SRC(mv)]]);
		switch(DST(mv)-SRC(mv))
		{
			case 1: strcat(aa,"→");break;
			case 4: strcat(aa,"→");break;
			case -1: strcat(aa,"←");break;
			case -4: strcat(aa,"←");break;
			case -16: strcat(aa,"↑");break;
			case -48: strcat(aa,"↑");break;
			case 16: strcat(aa,"↓");break;
			case 48: strcat(aa,"↓");break;
		}
		char output[500]="\0";
		sprintf(output," %d   (%d) %s\n",nd,vl,aa);
		SetWindowText(GetDlgItem(hWnd,IDC_EDIT1),output);
	}
	UpdateWindow(hWnd);
	switch(uMsg)
	{
  		case WM_DESTROY:
    		PostQuitMessage(0);
    		break;
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	
	return FALSE;
}
char FEN[100];
// 窗体事件捕捉过程
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char send[1024];
  int x, y;
  HDC hdc;
  RECT rect;
  PAINTSTRUCT ps;
  MSGBOXPARAMS mbp;

  switch (uMsg) {
  // 新建窗口
  case WM_CREATE:
    // 调整窗口位置和尺寸
    GetWindowRect(hWnd, &rect);
    x = rect.left;
    y = rect.top;
    rect.right = rect.left + BOARD_WIDTH;
    rect.bottom = rect.top + BOARD_HEIGHT;
    AdjustWindowRect(&rect, WINDOW_STYLES, TRUE);
    MoveWindow(hWnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    break;
  // 退出
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  // 菜单命令
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
    case IDM_ANALYZE:
    {
		char input[1024];
		char *point=input;
		strcpy(input,"\0");
		map_to_fen();
		sprintf(input,"fen %s",Search.fen);
		pipeStdHandle.LineOutput(input);
		strcpy(input,"\0");
		
		int mvs[MAX_GEN_MOVES],moves=pos.GenerateMoves(mvs);
		for(int i=0;i<moves;i++)
		{
			pos.MakeMove(mvs[i]);
			if(pos.RepStatus())
			{
				sprintf(input,"banmoves %c%c%c%c",FILE_X(SRC(mvs[i]))+'a'-3,RANK_Y(SRC(mvs[i]))+'0'-3,FILE_X(DST(mvs[i]))+'a'-3,RANK_Y(DST(mvs[i]))+'0'-3);
				pipeStdHandle.LineOutput(input);
			}
			pos.UndoMakeMove();
		}
		
		
		
		
	  pipeStdHandle.LineOutput("go infinite");

	  HWND hwnd2 = CreateDialog(Xqwl.hInst,MAKEINTRESOURCE(IDD_ANALYZE_DIALOG),Xqwl.hWnd,(DLGPROC)DialogProc);
	  ShowWindow(hwnd2,SW_SHOW);  // 接收消息
	  MSG msg;
	  while (GetMessage(&msg, NULL, 0, 0)) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	  }
  	  pipeStdHandle.LineOutput("stop");
  	  while (strncmp(input,"bestmove ",9)!=0) {
	    pipeStdHandle.LineInput(input);
	  }
	  Sleep(50);
	  break;
	}
    case IDM_FILE_EXIT:
      DestroyWindow(Xqwl.hWnd);
      break;
    case IDM_STANDARD:
	{
		strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e1t r");
      Startup();
      fen_to_map(FEN); 
      hdc = GetDC(Xqwl.hWnd);
      DrawBoard(hdc);
      ReleaseDC(Xqwl.hWnd, hdc);
		break;
	}
    case IDM_HANDICAP:
	{
		strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e2 r");
      Startup();
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
		if(OpenClipboard(NULL))//打开剪贴板
		{
			HANDLE hClip;
			char* pBuf;
			EmptyClipboard();//清空剪贴板
	
			//写入数据
			hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(Search.fen)+1);
			pBuf=(char*)GlobalLock(hClip);
			strcpy(pBuf,Search.fen);
			GlobalUnlock(hClip);//解锁
			SetClipboardData(CF_TEXT,hClip);//设置格式
	
			//关闭剪贴板
			CloseClipboard();
		}
		break;
	} 
    }
    break;
  // 绘图
  case WM_PAINT:
    hdc = BeginPaint(Xqwl.hWnd, &ps);
    DrawBoard(hdc);
    EndPaint(Xqwl.hWnd, &ps);
    break;
  // 鼠标点击
  case WM_LBUTTONDOWN:
    x = FILE_LEFT + (LOWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    y = RANK_TOP + (HIWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
    if (x >= FILE_LEFT && x <= FILE_RIGHT && y >= RANK_TOP && y <= RANK_BOTTOM) {
      ClickSquare(COORD_XY(x, y));
    }
    break;
  // 其他事件
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return FALSE;
}

// 装入资源图片
inline HBITMAP LoadResBmp(int nResId) {
  return (HBITMAP) LoadImage(Xqwl.hInst, MAKEINTRESOURCE(nResId), IMAGE_BITMAP,0,0, LR_DEFAULTSIZE | LR_SHARED);
}

// 入口过程
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	pipeStdHandle.Open("engine.exe");
	pipeStdHandle.LineOutput("uaci\n");
	int tt=clock();
	char input[1024];
	while (strcmp(input,"uaciok")!=0) {
		if(clock()-tt>=1000)
		{
			MessageBox(0,"引擎文件加载失败","AnimalcraftAI",0);
			break; 
		}
		pipeStdHandle.LineInput(input);
	}
	
	PlayResWav(IDR_START);
	
  int i;
  MSG msg;
  WNDCLASSEX wce;

  // 初始化全局变量
  srand((DWORD) time(NULL));
  InitZobrist();
  Xqwl.hInst = hInstance;
  Startup();
  strcpy(FEN,"T1E3m1l/1C5d1/2W3p2/9/2P3w2/1D5c1/L1M3e1t r");

  // 装入图片
  Xqwl.bmpBoard = LoadResBmp(IDB_BOARD);
  Xqwl.bmpSelected = LoadResBmp(IDB_SELECTED);
  for (i = PIECE_ELEPHANT; i <= PIECE_MOUSE; i ++) {
    Xqwl.bmpPieces[SIDE_TAG(0) + i] = LoadResBmp(IDB_RE + i);
    Xqwl.bmpPieces[SIDE_TAG(1) + i] = LoadResBmp(IDB_BE + i);
  }
    Xqwl.bmpTrap = LoadResBmp(IDB_TRAP);
    Xqwl.bmpDen = LoadResBmp(IDB_DEN);

  // 设置窗口
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

  // 打开窗口
  Xqwl.hWnd = CreateWindow("AnimalcraftAI", "AnimalcraftAI", WINDOW_STYLES,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  if (Xqwl.hWnd == NULL) {
    return 0;
  }
  ShowWindow(Xqwl.hWnd, nCmdShow);
  UpdateWindow(Xqwl.hWnd);


	

  // 接收消息
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

	pipeStdHandle.LineOutput("quit");
	pipeStdHandle.Close();
  return msg.wParam;
}
