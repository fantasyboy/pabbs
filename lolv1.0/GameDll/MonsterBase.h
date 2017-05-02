#pragma once
#include "base.h"
/*
怪物对象基类（派生类有:炮塔，小兵，龙，野怪，人物）
*/

//坐标
typedef struct EM_POINT_3D
{
	float x;
	float y;
	float z;
}EM_POINT_3D, *PEM_POINT_3D;

//阵营
enum EM_CAMP
{
	CAMP_EM,
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

	//接口
	float GetCurHp()const;
	float GetMaxHp()const;
	float GetCurMp()const;
	float GetMaxMp()const;

	//获取坐标
	EM_POINT_3D GetPoint()const;

	EM_CAMP GetCamp()const;

	EM_TYPE GetType()const;

	//是否在战争迷雾中
	virtual bool BInShowInFag()const;

	//是否死亡
	bool BDead()const;
};

