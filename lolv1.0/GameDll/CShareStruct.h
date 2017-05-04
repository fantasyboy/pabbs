#pragma once

#define  MAP_NAME "lol_v1.0"

#pragma pack(1)
typedef struct _SHARED_MOMORY
{
	bool bLockQ;
	bool bLockW;
	bool bLockE;
	bool bLockR;
	bool bOpenAA;
}SHARED_MEMORY , *PSHARED_MOMERY;
#pragma pack()