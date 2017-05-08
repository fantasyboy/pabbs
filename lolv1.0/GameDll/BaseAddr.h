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
const DWORD Base_SkillCallEcxAddr = 0x01E46A3C;    //技能CALL ECX     偏移:2    //OD地址:0x00DD9028
const DWORD Base_SkillCallAddr = 0x009B6DC0;    //技能CALL     偏移:1    //OD地址:0x00DD904F
const DWORD Base_SkillCallHookAddr = 0x0069FA5D;    //技能CALL Hook     偏移:1    //OD地址:0x0069FA5D
const DWORD Base_GameWndHwndAddr = 0x01A6E8D0;    //游戏窗口基地址     偏移:2    //OD地址:0x0055DC54
const DWORD Base_MonsterCurrentHpOffset = 0x00000570;    //当前血量偏移     偏移:4    //OD地址:0x007C43B7
const DWORD Base_MonsterCurrentMpOffset = 0x000001F8;    //当前蓝量偏移     偏移:5    //OD地址:0x008967DF
const DWORD Base_equipmentTravseOffset2 = 0x00000288;    //装备数组遍历偏移2     偏移:3    //OD地址:0x00B8C079
const DWORD Base_equipmentTravseOffset1 = 0x000015D8;    //装备数组遍历偏移1     偏移:2    //OD地址:0x00B3CCDB
const DWORD Base_MonsterBVisableOffset = 0x0000394C;    //是否可见偏移     偏移:2    //OD地址:0x00A9CB43
const DWORD Base_MousePointAddr = 0x1E46A3C;            //鼠标位置基地址
const DWORD Base_FindWayCallAddr = 0x00827650; //寻路call
const DWORD Base_AttackHeroCallAddr = 0x00827650; //普攻CALL
//下面的代码暂时没有做更新
const DWORD Base_SkillOffset_Range1 = 0x3b0;		 //技能范围偏移
const DWORD Base_SkillOffset_Range2 = 0x3cc;		 //技能范围偏移


