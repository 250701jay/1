#pragma once
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<random>
#include<string>
#include<time.h>
#include<algorithm>
#include<fstream>
#include<queue>
using namespace std;
//��������
#define CAPACITY 20		//������
#define FLOOR_COUNT 32	//¥������
#define ELE_COUNT 4		//��������
#define FLOOR_HIGHT 3000 //	¥��߶�
//ģ��������
#define TIME_INTERVAL 0.1
#define DEBUG_ENO -1		//��ʽ���ݱ��


//����״̬��
enum EleState {
	IDLE,//����
	STOP,//ֹͣ,��ʱ����
	OPEN_DOOR,//����
	OFF_PASSENAGE,//�³˿�
	LAND_PASSENAGE,//�ϳ˿�
	CLOSE_DOOR,//����
	MOVE,//ֹͣ
	TURN//ת��
};

//ȫ�ֱ���
extern int currentday;
//��ǰʱ��
extern double currentTime;
//��ǰģʽ
extern unsigned char currentMode;
//Ŀǰ�����ϵĳ˿�����
extern int landpassage;
//Ŀǰ�Ѿ�����ĳ˿�����
extern int offpassage;