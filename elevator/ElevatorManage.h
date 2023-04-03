#pragma once
#include"config.h"
#include"greedy.h"
#include"PassageManage.h"
#include"Elevator.h"
//���ݹ���ģ��




class ElevatorManage
{
private:
	ElevatorManage() {}  //˽�еĹ��캯��
public:
	//��������
	unsigned char m_eleNum;
	//¥��
	unsigned int floorHight;
	//����ʵ��
	vector<Elevator> vec_elevator;
	//��ʾ��Ŀ�ĵ������Ƿ��ѷ������
	bool outer_call_manage[FLOOR_COUNT + 1][2] = { 0 };//<¥���,�Ƿ�������(0��ʾ��,1��ʾ��)>ֻ��Է�Ŀ�ĵ�����
													   //���ݲ���
	unsigned int inputParam[4][IN_PARAM_NUM] = { 0 };
	//�����������
	unsigned int outputParam[4][OUT_PARAM_NUM] = { 0 };
	//�����������ź�
	unsigned char innerCall[4][INNER_CALL_SIZE] = { 0 };
	unsigned char outerCall[4][2][OUTER_CALL_SIZE] = { 0 };
	
	//����ä������
	unsigned char invalidFloor[4][MAX_FLOOR] = { 0 };
	//Ȩ�����ʱ��ͳ���ʱ��Ĳ���,Ĭ��Ϊ8
	unsigned int adjustParam = 8;
	//��VIPģʽ
	unsigned char vip_mode = 0;
	//���ݳ˿�����
	unsigned char passengerNum = 1;
	//ָ��ʱ��� grpInParam[n][CAN_NORMAL_ASSIGN]=1�ĵ��ݿ���Ӧ��ͨ����
	AssignSetting assignSetting = { 0 };
	//ʱ�����,��������Ӧ��ͨ���У���������Ϊ0
	unsigned char assignSize = 0;

	//ģʽ��ز���
	unsigned char triggerMode = 1; //1��ʾ������0��ʾ������
	ManualSetting manualSettings = { 0 };
	unsigned char manualSize = 0;
	AutoSetting autoSetting = { 0 };

	GrpOutParam grpOutParam = { 0 };

	

	//����ģ��<¥��ţ��������>
	//Record records_Yesterday[MAX_FLOOR][MAX_RECORD_SIZE] = {0};
	//int records_yesterday_count[MAX_FLOOR] = { 0 };
	//����ļ�¼
	//Record records[MAX_FLOOR][MAX_RECORD_SIZE] = { 0 };
	//int records_count[MAX_FLOOR] = { 0 };


	static ElevatorManage * GetInstance();

	//ÿ����賿��¼�ѽ���ļ�¼���浽����ͬʱ��ս���ļ�¼
	//bool ResetRecord();

	//bool AddRecord(Record temp_recode, int currentfloor);

	bool Init(unsigned char eleNum);
	
	void SetInnerOuterCall();

	
	

	void SetEnv();

	int dispatch(Passage * p,bool aim);

	
	
	int dispatchPassage(Passage * p);
	
	int ModeMonitor();
	bool Update();

	//������Ϣ
	FloorProbability(*floorProbability)[MAX_RECORD_SIZE];
	unsigned int(*probabilitySize);
	void AddElevatorRecode(int peoplecount, int curF, int dir, int e_id);
	bool FindBestRecord(FloorProbability & bestRecord, unsigned char fromFloor, double currentTime);
	//void SetRecode();
};