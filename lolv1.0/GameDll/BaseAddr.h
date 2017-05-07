#pragma once
#include <windows.h>

const DWORD Base_GameStartTime = 0x01A6D808;    //游戏开局时间     偏移:1    //OD地址:0x0060610E
const DWORD Base_RoleSelfAddr = 0x01A6DA70;    //玩家基地址     偏移:2    //OD地址:0x0055AF00
const DWORD Base_BufferAddr = 0x01A7383C;    //buff数组基地址     偏移:2    //OD地址:0x00565F9A
const DWORD Base_MonsterArrayAddr = 0x02A991BC;    //怪物数组基地址     偏移:2    //OD地址:0x006FA547
const DWORD Base_SkillTravseOffset1 = 0x00002948;    //技能数组第一层偏移     偏移:1    //OD地址:0x006C246D
const DWORD Base_SkillTravseOffset2 = 0x00000560;    //技能数组第二层偏移     偏移:2    //OD地址:0x0059EC70
const DWORD Base_SkillOffset_Object = 0x000000F4;    //技能对象偏移     偏移:2    //OD地址:0x006C2484
const DWORD Base_SkillOffset_MP = 0x000005A8;    //技能蓝耗偏移     偏移:5    //OD地址:0x0077FBDB


const DWORD Base_SkillOffset_Range1 = 0x3b0;		 //技能范围偏移
const DWORD Base_SkillOffset_Range2 = 0x3cc;		 //技能范围偏移
const DWORD Base_equipmentTravseOffset1 = 0x15d8; //装备数组遍历偏移1
const DWORD Base_equipmentTravseOffset2 = 0x288;  //装备数组遍历偏移2
const DWORD Base_MonsterCurrentHpOffset = 0x570;  //当前血量偏移
const DWORD Base_MonsterCurrentMpOffset = 0x1f8;  //当前蓝量偏移
const DWORD Base_SkillCallEcxAddr = 0x1E46A3C;     //技能CALL ECX
const DWORD Base_SkillCallAddr = 0x009B6DC0;       //技能CALL
const DWORD Base_SkillCallHookAddr = 0x0069FA5D;   //技能CALL Hook

const DWORD Base_GameWndHwndAddr = 0x1A6E8D0;

