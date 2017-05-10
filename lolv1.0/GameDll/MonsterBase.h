#pragma once
#include "base.h"
/*
怪物对象基类（派生类有:炮塔，小兵，龙，野怪，人物）
*/

//坐标
typedef struct EM_POINT_3D
{
	float x;
	float z;	
	float y;
}EM_POINT_3D, *PEM_POINT_3D;

//阵营
enum EM_CAMP
{
	CAM_UNKNOW = 0,
	CAMP_BULE =100,
	CAMP_RED = 200,
	CAM_NEUTRAL = 300,
};
//类型
enum EM_TYPE
{
	CAMP_EM1,
};

class MonsterBase :
	public base
{
public:
	MonsterBase(DWORD dwNodeBase);
	~MonsterBase();

	//获取名字
	virtual char* GetName()const;

	//接口
	float GetCurHp()const;
	float GetMaxHp()const;
	float GetCurMp()const;
	float GetMaxMp()const;

	//获取坐标
	EM_POINT_3D GetPoint()const;

	EM_CAMP GetCamp()const;

	DWORD GetType()const;

	//是否在战争迷雾中 (true 表示在)
	virtual bool BInShowInFag()const;

	//是否死亡
	bool BDead()const;

	//获取距离
	float GetDistance(EM_POINT_3D* mon);

	//获取朝向 返回值为 坐标
	EM_POINT_3D GetMonsterOrientation()const;

	//获取移动速度
	float GetMoveSpeed()const;

	//获取玩家是否移动
	bool GetBMoving()const;
};

