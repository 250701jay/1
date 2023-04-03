//�˿�
#pragma once
#include"config.h"
#include"greedy.h"

class Passage {
public:
	Passage() {}
	
	Passage(int PassageID,int WorkFloor, int FromFloor, int AimFloor, double ComeTime);

	int iPassageID;
	int iWorkFloor;
	int iFromFloor;
	int iAimFloor;
	//�˿ͷ���
	int dir;


	double iComeTime;
	double iLandTime;//���ϵ��ݵ�ʱ��
	double iOffTime;//�µ��ݵ�ʱ��
	int eleno; //ָ�ɵĵ��ݺ�

	unsigned char is_vip;//�Ƿ���vip
	unsigned char is_disabled;//�Ƿ�м�
	
};





//�˿͹���ģ��
class PassageManage
{
private:
	PassageManage() {}  //˽�еĹ��캯��
public:
	static PassageManage* GetInstance()
	{
		static PassageManage Instance;
		return &Instance;
	}

	//Ӧ�����޷����ϵ��ݵĳ˿���Ҫ��һ������

	map<int, set<Passage*> > map_wait_passage;//<¥�㣬�˿�ָ��>�Ѿ������䣬���ǻ�û�н��ϵĳ˿�

											  //����Ѿ�����ĳ˿�
	
	bool addWaitPassage(Passage * p);

	bool delWaitPassageIn(int currentfloor);

};