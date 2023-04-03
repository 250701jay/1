#include "ElevatorManage.h"
#include "Elevator.h"

ElevatorManage* ElevatorManage::GetInstance()
{
	static ElevatorManage Instance;
	return &Instance;
}
/*
//bool ElevatorManage::ResetRecord()
//{
//	memcpy(records_Yesterday, records,sizeof(records));
//	memset(records, 0, sizeof(records));
//
//	memcpy(records_yesterday_count, records_count, sizeof(records_count));
//	memset(records_count, 0, sizeof(records_count));
//	return true;
//}

//bool ElevatorManage::AddRecord(Record temp_recode,int currentfloor)
//{
//	//���뵽����ģ��
//	records[currentfloor][records_count[currentfloor]% MAX_RECORD_SIZE] = temp_recode;
//	records_count[currentfloor]++;
//	return true;
//}

////���ü�����Ϣ,ֻ��Է�Ŀ�ĵ�����
//void ElevatorManage::SetRecode() {
//	//���õ��ݵ�������
//	memset(recodeProbability, 0, sizeof(recodeProbability));
//	memset(probabilitySize, 0, sizeof(probabilitySize));
//	for (int f = 0; f < MAX_FLOOR; f++) {
//		for (int r = 0; r < MAX_RECORD_SIZE; r++) {
//			if (records_Yesterday[f][r].opendoorTime == 0) {
//				break;
//			}
//			recodeProbability[f][r].recodetime = records_Yesterday[f][r].opendoorTime;
//			recodeProbability[f][r].fromfloor = records_Yesterday[f][r].opendoorFloor;
//			recodeProbability[f][r].allProbability = records_Yesterday[f][r].inpassgeCount;
//			for (int i = 0; i < records_Yesterday[f][r].inpassgeCount; i++) {
//				for (int j = 0; j < MAX_FLOOR; j++) {
//					recodeProbability[f][r].offProbability[j] += 1.0 * records_Yesterday[f][r].p[i]->offProbability[j] / records_Yesterday[f][r].p[i]->allProbability;
//				}
//			}
//			probabilitySize[f]++;
//		}
//	}
//
//	return;
//}
*/

bool ElevatorManage::Init(unsigned char eleNum) {
	this->m_eleNum = eleNum;
	this->floorHight = FLOOR_HIGHT;
	for (int i = 0; i < m_eleNum; i++) {
		Elevator ele(i);
		vec_elevator.push_back(ele);
	}
	for (int i = 0; i < m_eleNum; i++) {
		inputParam[i][ID_INDEX] = i;
		inputParam[i][VELOCITY_INDEX] = 1500;//1.5m/s
		inputParam[i][ACCELERATION_INDEX] = 650;//���ٶ� 0.6m/s
		inputParam[i][DECELERATION_INDEX] = 650;//���ٶ�
		inputParam[i][OPEN_INDEX] = 30;//����ʱ�� 3��
		inputParam[i][CLOSE_INDEX] = 30;//����ʱ�� 3��
		inputParam[i][DOOR_TIME_INDEX] = 30;//�˿�������ʱ�� 3��
		inputParam[i][OUT_OF_GROUP_CONTROL_INDEX] = 0;//��Ⱥ�� TODO
		inputParam[i][CAPACITY_INDEX] = CAPACITY;	//��������
		inputParam[i][ACCELERATE_TIME_INDEX] = 13;//1.3��
		inputParam[i][DECELERATE_TIME_INDEX] = 13;//1.3��
		inputParam[i][FLOOR_TIME_INDEX] = 15;	//1.5��
		inputParam[i][FLOOR_NUM_INDEX] = FLOOR_COUNT;//¥������
		inputParam[i][SAVING_CONTROL_INDEX] = 0;
		inputParam[i][IS_DISABLED_INDEX] = 0;
		inputParam[i][IS_MANUAL_ASSIGNING_INDEX] = 0;
		inputParam[i][IS_AUTO_ASSIGNING_INDEX] = 0;
		inputParam[i][IS_VIP_MODE_INDEX] = 0;
		inputParam[i][DO_INDEX] = 0;
		inputParam[i][DISABLED_NUM_INDEX] = 0;
		inputParam[i][DECELERATE_DISTANCE] = 0;
		inputParam[i][CAN_NORMAL_ASSIGN] = 0;
		//�⼸�������ǿɱ��
		inputParam[i][IS_FULL_INDEX] = 0;
		inputParam[i][DIRECTION_INDEX] = 0;//����//����ȥ��
		inputParam[i][POS_INDEX] = 0;//λ��//����ȥ��
		inputParam[i][IS_MOVING_INDEX] = 1;
	}
	::memset(innerCall, INVALID_NUM, sizeof(innerCall));
	::memset(outerCall, INVALID_NUM, sizeof(outerCall));
	
	autoSetting.upPeakFloor = 1;
	autoSetting.upPeakCount = 10;
	autoSetting.upPeakDelayTime = 500;
	autoSetting.interPeakCount = 10;
	autoSetting.interPeakDelayTime = 500;
	autoSetting.savingCount = 1;
	autoSetting.savingBufferLen = 100;
	autoSetting.interPeakTime = (unsigned int *)malloc(sizeof(unsigned int));//���ģʽ������
	::memset(autoSetting.interPeakTime, 0, sizeof(unsigned int));//�Զ�����֮���Ŀ�ĵ�
	autoSetting.upPeakTime = (unsigned int *)malloc(sizeof(unsigned int));//����ģʽ������
	::memset(autoSetting.upPeakTime, 0, sizeof(unsigned int));//�Զ�����֮���Ŀ�ĵ�
	autoSetting.savingBuffer = (unsigned int *)malloc(sizeof(unsigned int));//����ģʽ������
	::memset(autoSetting.savingBuffer, 0, sizeof(unsigned int));//�Զ�����֮���Ŀ�ĵ�
	autoSetting.savingControl = (unsigned char *)malloc(sizeof(unsigned char) * eleNum);
	::memset(autoSetting.savingControl, 0, sizeof(unsigned char) * eleNum);//ȫ��û������Ⱥ��
	autoSetting.upPeakRecordDelayTime = 10;//���и߷��¼���ӳ�ʱ��
																		   //�������
	grpOutParam.elevatorMode = NONE_MODE;
	grpOutParam.dest = (unsigned char *)malloc(sizeof(unsigned char) * eleNum);
	memset(grpOutParam.dest, INVALID_NUM, sizeof(unsigned char) * eleNum);//�Զ�����֮���Ŀ�ĵ�
	grpOutParam.savingControl = (unsigned char *)malloc(sizeof(unsigned char) * eleNum);
	memset(grpOutParam.savingControl, 0, sizeof(unsigned char) * eleNum);//�Ƿ�����Ⱥ��
	memset(outer_call_manage, 0, sizeof(outer_call_manage));

	//���������Ϣ
	//MallocRecodeMemory(floorProbability, probabilitySize);
	return true;
}



//������������
void ElevatorManage::SetInnerOuterCall() {
	//��ʼ��
	memset(innerCall, INVALID_NUM, sizeof(unsigned char)* INNER_CALL_SIZE * 4);
	memset(outerCall, INVALID_NUM, sizeof(unsigned char)* OUTER_CALL_SIZE * 4 * 2);
	for (int e = 0; e < vec_elevator.size(); e++) {
		int c = 0;
		for (auto& inner : vec_elevator[e].inner_people) {
			if (inner.second.size() > 0) {
				innerCall[e][c] = inner.first;
			}
			c++;
			if (c >INNER_CALL_SIZE) {
				break;
			}
		}
	}
	for (int e = 0; e < vec_elevator.size(); e++) {
		int c = 0;
		if (vec_elevator[e].outer_people.size() > 0) {
			for (auto& outer : vec_elevator[e].outer_people) {
				//<Ŀ�ĵ�,���ٴ�>
				if (outer.second.size() > 0) {
					for (auto passage : outer.second) {
						outerCall[e][0][c] = passage->iFromFloor;
						outerCall[e][1][c] = passage->iAimFloor;
						c++;
						if (c >OUTER_CALL_SIZE) {
							break;
						}
					}
				}
			}
		}
		
		
		for (int f = 0; f < FLOOR_COUNT + 1;f++) {
			//<Ŀ�ĵ�,���ٴ�>
			if (vec_elevator[e].outer_call[f][0]) {
				outerCall[e][0][c] = f;
				outerCall[e][1][c] = UP_NUM;
				c++;
				if (c >OUTER_CALL_SIZE) {
					break;
				}
			}
			if (vec_elevator[e].outer_call[f][1]) {
				outerCall[e][0][c] = f;
				outerCall[e][1][c] = DOWN_NUM;
				c++;
				if (c >OUTER_CALL_SIZE) {
					break;
				}
			}
		}
	}
	return;
}




//���õ��ݻ���
void ElevatorManage::SetEnv() {
	//���õ��ݵ�������
	SetInnerOuterCall();
	//���õ��ݵ�ǰλ�ã����ݵ�ǰ���򣬵����Ƿ�����
	for (int e = 0; e < vec_elevator.size(); e++) {
		inputParam[e][POS_INDEX] = vec_elevator[e].currentpos;
		inputParam[e][DIRECTION_INDEX] = vec_elevator[e].dir;
		if (vec_elevator[e].PassageCount >= inputParam[e][CAPACITY_INDEX]) {
			inputParam[e][IS_FULL_INDEX] = 1;
		}
		else {
			inputParam[e][IS_FULL_INDEX] = 0;
		}
		//����֮���ʾ�Ѿ��ƶ��ˣ��޷������ϳ˿�
		if (vec_elevator[e].state == CLOSE_DOOR || vec_elevator[e].state == TURN || vec_elevator[e].state == MOVE) {
			inputParam[e][IS_MOVING_INDEX] = 1;
		}
		else {
			inputParam[e][IS_MOVING_INDEX] = 0;
		}
		//ģ����ݵ�ǰ����
		//inputParam[e][WEIGHT] = vec_elevator[e].PassageCount * WEIGHT_PER_PASSAGE;
	}
	return;
}

//���ݽӿ�
int ElevatorManage::dispatch(Passage *p,bool aim) {
	unsigned char fromFloor = p->iFromFloor;
	unsigned int is_vip = p->is_vip;
	unsigned int is_disabled = p->is_disabled;
	unsigned char aimFloor;
	if (!aim) {
		if (p->dir == 1) {
			aimFloor = UP_NUM;
		}
		else if (p->dir == 2) {
			aimFloor = DOWN_NUM;
		}
	}
	else {
		aimFloor = p->iAimFloor;
	}
	
	//���õ��ݻ�������
	SetEnv();
	int eleno = compassInterface(fromFloor, aimFloor, is_vip, is_disabled, m_eleNum, floorHight, inputParam, outputParam, innerCall, outerCall, invalidFloor, adjustParam, vip_mode, passengerNum, &assignSetting, assignSize, (unsigned int)currentTime);
	//int eleno = 0;
	return eleno;
}


bool ElevatorManage::FindBestRecord(FloorProbability& bestRecord, unsigned char fromFloor, double currentTime) {
	//ADD by YJH
	//�ж��Ƿ������ҵ���ʷ��Ϣ
	bool find = false;

	if (probabilitySize == NULL) {
		return find;
	}
	//�ҵ�����ļ�¼��Ϣ,���ʱ�䲻����5����
	double MinDiffTime = 300;
	if (probabilitySize[fromFloor] > 20) {
		for (int r = 0; r < min(int(probabilitySize[fromFloor]), MAX_RECORD_SIZE); r++) {
			//���ǵ�ǰ��Ĳ�������
			if (currentday == floorProbability[fromFloor][r].recodeday) {
				continue;
			}
			double recodetime = floorProbability[fromFloor][r].recodetime;
			double diff = fabs(recodetime - currentTime);
			if (MinDiffTime>diff) {
				find = true;
				MinDiffTime = diff;
				bestRecord = floorProbability[fromFloor][r];
			}
		}
	}
	return find;
}


//���ݽӿ�
int ElevatorManage::dispatchPassage(Passage *p) {
	int eleno = -1;
	unsigned char fromFloor = p->iFromFloor;
	unsigned char aimFloor = p->iAimFloor;

	//����Ƿ�Ŀ�ĵ�����
	if (fromFloor != 0) {
		unsigned char dir = p->dir;
		//�����û�з�����ݵĻ�
		if (dir == 1) {
			//����Ѿ��ò��Ѿ����˷����˵��ݣ���������
			if (!outer_call_manage[fromFloor][0]) {
				//��������ǰ�����и߷�Ļ�ȥ����һϵ�е�Ԥ�����ݣ�ѭ������
				if ((int)currentMode == UP_PEAK_MODE) {
					FloorProbability bestRecord = {0};
					bool find = FindBestRecord(bestRecord, fromFloor ,currentTime);
					//bool find = false;
					if (find) {
						//cout << "find!!" << endl;
						for (int f = fromFloor + 1; f < MAX_FLOOR; f++) {
							//�����ʷ���ݸò�����Ҫȥ,���Ŀ�ĵ�����
							if (bestRecord.offProbability[f]) {
								//ģ�������Ԥ����������ĳ˿ͣ�idΪ-1��ʾ������ʵ��
								Passage passage(-1, f, fromFloor, f, currentTime);
								//ת��ΪĿ�ĵ�����
								eleno = dispatch(&passage, true);
								//�����ݼ�������
								vec_elevator[eleno].outer_call[fromFloor][0] = true;
							}
						}
					}
					else {
						//printf("s:%d", (int)currentMode == UP_PEAK_MODE);
						eleno = dispatch(p, false);
						//�����ݼ�������
						vec_elevator[eleno].outer_call[fromFloor][0] = true;
					}
				}
				else {
					
					eleno = dispatch(p, false);
					//�����ݼ�������
					vec_elevator[eleno].outer_call[fromFloor][0] = true;
				}
				outer_call_manage[fromFloor][0] = true;
				
				
			}
		}
		else if (dir == 2) {
			if (!outer_call_manage[fromFloor][1]) {
				eleno = dispatch(p, false);
				outer_call_manage[fromFloor][1] = true;
				//�����ݼ�������
				vec_elevator[eleno].outer_call[fromFloor][1] = true;
			}
		}
	}
	//�����Ŀ�ĵ�����
	else {
		eleno = dispatch(p,true);
		//cout << "ele:" << eleno << endl;
		//�����ݼ�������
		vec_elevator[eleno].outer_people[p->iFromFloor].push_back(p);
	}
	return eleno;
}


//��Ӽ�����Ϣ
void ElevatorManage::AddElevatorRecode(int peoplecount, int curF, int dir, int e_id) {
	SetEnv();
	elevatorRecode(0, peoplecount*WEIGHT_PER_PASSAGE, currentday, currentTime, curF, dir, innerCall, outerCall, m_eleNum, invalidFloor, floorProbability, probabilitySize, e_id);
}

int ElevatorManage::ModeMonitor() {
	//���õ��ݻ�������
	SetEnv();
	//ģʽ���
	elevatorModeMonitor(triggerMode, &manualSettings, manualSize, autoSetting, currentTime, innerCall, outerCall, inputParam, m_eleNum, &grpOutParam, invalidFloor);
	//ģʽת��

	if (currentMode != grpOutParam.elevatorMode) {
		currentMode = grpOutParam.elevatorMode;
		switch (currentMode)
		{
			case(UP_PEAK_MODE): {
				//��������и߷�Ļ���ÿ��һ��ʱ���¼�����е�����
				cout << " currentTime:" << currentTime << " �������и߷�" << endl;
				break;
			}
			case(DOWN_PEAK_MODE): {
				cout << " currentTime:" << currentTime << " �������и߷�" << endl;
				break;
			}
			case(INTER_PEAK_MODE): {
				cout << " currentTime:" << currentTime << " ������߷�߷�" << endl;
				break;
			}
			case(SAVING_MODE): {
				cout << " currentTime:" << currentTime << " �������ģʽ" << endl;
				break;
			}
			case(NONE_MODE): {
				cout << " currentTime:" << currentTime << " �����޸߷�" << endl;
				break;
			}
		}
	}
	
	return true;
}

//���µ���״̬
bool ElevatorManage::Update() {
	for (int e = 0; e < vec_elevator.size(); e++) {
		vec_elevator[e].update(INVALID_NUM);
		//vec_elevator[e].update(grpOutParam.dest[e]);
	}
	return true;
}