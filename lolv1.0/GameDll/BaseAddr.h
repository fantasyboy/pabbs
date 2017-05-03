#pragma once
#include <windows.h>

const DWORD Base_GameStartTime = 0x01A6D808;	 //游戏开局时间
const DWORD Base_RoleSelfAddr = 0x1A6DA70;		 //玩家基地址（玩家本身）
const DWORD Base_BufferAddr = 0x1A7383C;		 //buff数组基地址
const DWORD Base_MonsterArrayAddr = 0x2a991bc;   //怪物数组基地址

const DWORD Base_SkillTravseOffset1 = 0x2948;    //技能数组第一层偏移
const DWORD Base_SkillTravseOffset2 = 0x560;     //技能数组第二层偏移
const DWORD Base_SkillOffset_Object = 0x0f4;     //技能对象偏移
const DWORD Base_SkillOffset_MP = 0x5a8;		 //技能蓝耗偏移
const DWORD Base_SkillOffset_Range = 0x3b0;		 //技能范围偏移
const DWORD Base_equipmentTravseOffset1 = 0x15d8; //装备数组遍历偏移1
const DWORD Base_equipmentTravseOffset2 = 0x288;  //装备数组遍历偏移2
