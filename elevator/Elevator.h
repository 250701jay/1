#pragma once
#include"config.h"
#include"greedy.h"
#include"PassageManage.h"

//��¼ģ��
struct Record
{
	//����ʱ��
	double opendoorTime;
	//���ݱ��
	int eleNo;
	//����¥��
	int opendoorFloor;
	//�ϳ˿�����
	int inpassgeCount;
	//��Ӧ�ĳ˿�ָ��,�����¼��Ӧ�ĸ�����Ϣ
	Passage* p[CAPACITY];
};

//����ģ����
class Elevator {
public:
	Elevator() {}
	Elevator(int id) {
		iEleID = id;
		dir = 0;
		currentpos = 1;
		currentheight = 0;
		state = IDLE;
		_t_controller = 0;
	}
	int iEleID;//���ݱ��
	unsigned int dir;//����
	int currentpos;//λ��
	double velocity = 1.5;//���������ٶ�
	double currentheight;//��ǰ�߶�

	//int peopleNeedOff[FLOOR_COUNT + 1] = { 0 };	//����Ŀ�ĵ�������Ҫ�ڸò��µ��������������ݵ����Ƿ�Ŀ�ĵ������ϵĵ���

	map<int, vector<Passage*> >  inner_people;//��ǰ�ڵ����ϵ��ˣ�<Ŀ�ĵأ����еĳ˿�>
	map<int, vector<Passage*> > outer_people;//��ǰ�ڵ�������ˣ�<��ʼ�أ����еĳ˿�>,ֻ���Ŀ�ĵ�����
	bool outer_call[FLOOR_COUNT + 1][2] = { 0 };//<¥���,�Ƿ�������(0��ʾ��,1��ʾ��)>��Է�Ŀ�ĵ�����
	
	
	
	EleState state;//״̬
	
	int PassageCount = 0;//��ǰ�ĳ˿�����

	double _t_controller = 0;//��������ʱ��
	double opendoor = 3.0;//���ŵĳ���ʱ��
	double closedoor = 3.0;//���ŵĳ���ʱ��
	double turn = 1.0;//ת��ĳ���ʱ��
	double passagetakeoff = 3.0;//�³˿͵ĳ���ʱ��
	double passagetakein = 3.0;//�ϳ˿͵ĳ���ʱ��

	//��¼��ǰ�Ƿ���Ҫ�ϳ˿ͻ����³˿�
	bool m_needoffpassage = false;
	bool m_needonpassage = false;
	
	
	

	//map<unsigned char, int> getouteraim(unsigned char from);

	unsigned int getinitdir();

	bool hasoutcall();

	bool hasinncall();

	bool taskIn(Passage * p,bool aim);

	bool hasSameDir();

	bool passageIn();


	bool passageOFF();

	bool needturn();

	bool needopendoor();


	void update(unsigned char dest);

};
