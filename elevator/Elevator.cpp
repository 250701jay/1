#include "Elevator.h"
#include"ElevatorManage.h"
#include"PassageManage.h"
#include<assert.h>

//map<unsigned char, int> Elevator::getouteraim(unsigned char from) {
//	map<unsigned char, int> ret;
//	if (outer_people[from].size() > 0) {
//		for (int i = 0; i < outer_people[from].size(); i++) {
//			ret[outer_people[from][i]->iAimFloor]++;
//		}
//	}
//	return ret;
//}

unsigned int Elevator::getinitdir() {
	unsigned int dir = 1;
	//��ʼ����Ϊ�������������
	int mindiff = INT_MAX;
	//��������
	for (auto& inner : inner_people) {
		if (inner.second.size() > 0) {
			if (mindiff > abs(currentpos - inner.first)) {
				mindiff = abs(currentpos - inner.first);
				//��ȡ��ʼ����
				if (currentpos <= inner.first) {
					dir = 1;
				}
				else {
					dir = 2;
				}
			}
		}
	}
	if (mindiff != INT_MAX) {
		return dir;
	}
	//��������,�ҵ����������,��Է�Ŀ�ĵ�����
	for (auto outer : outer_people) {
		if (outer.second.size() > 0) {
			if (mindiff > abs(currentpos - outer.first)) {
				mindiff = abs(currentpos - outer.first);
				//��ȡ��ʼ����
				if (currentpos <= outer.first) {
					//cout << "???" << endl;
					dir = 1;
				}
				else {
					//cout << "!!!" << endl;
					dir = 2;
				}
			}
		}
	}
	//��Է�Ŀ�ĵ�����
	for (int i = 0; i < FLOOR_COUNT + 1; i++) {
		//�����������
		if (outer_call[i][0] || outer_call[i][1]) {
			if (mindiff > abs(currentpos - i)) {
				mindiff = abs(currentpos - i);
				//��ȡ��ʼ����
				if (currentpos <= i) {
					//cout << "???" << endl;
					dir = 1;
				}
				else {
					//cout << "!!!" << endl;
					dir = 2;
				}
			}
		}
	}

	return dir;
}

bool Elevator::hasoutcall() {
	//���Ŀ�ĵ�����
	if (outer_people.size() > 0) {
		for (auto outer : outer_people) {
			if (outer.second.size() > 0) {
				return true;
			}
		}
	}
	
	//��Է�Ŀ�ĵ�����
	for (int i = 0; i < FLOOR_COUNT + 1; i++) {
		if (outer_call[i][0] || outer_call[i][1]) {
			return true;
		}
	}
	return false;
}

bool Elevator::hasinncall() {
	for (auto& inner : inner_people) {
		if (inner.second.size() > 0) {
			return true;
		}
	}
	return false;
}

//��PassageManage��ɾ������ȴ�����,���뵽���ݵ�������
bool Elevator::taskIn(Passage* p,bool aim) {
	//�������м������
	inner_people[p->iAimFloor].push_back(p);
	p->iLandTime = currentTime;
	p->eleno = iEleID;
	return true;
}

//���к͵�ǰ������ͬ
bool Elevator::hasSameDir() {
	if (dir == 1) {
		if (outer_call[currentpos][0]) {
			return true;
		}
	}
	else if (dir == 2) {
		if (outer_call[currentpos][1]) {
			return true;
		}
	}
	return true;
}

//����������Ҫ�ϵĳ˿�
bool Elevator::passageIn() {
	map<int, vector<Passage*>>::iterator passageItr = outer_people.find(currentpos);
	//����Ŀ�ĵ������Ƿ�����Ҫ�ϵĳ˿�
	if (passageItr != outer_people.end()) {
		//�����л�������
		assert(currentpos == 1);
		//����Ŀ�ĵ�����Ӧ������
		//memset(peopleNeedOff, 0, sizeof(peopleNeedOff));
		
		for (Passage* passage_ptr : passageItr->second) {
			//�˿͵��������ܴ���������
			if (PassageCount + 1 > CAPACITY) {
				//�ٷ���һ̨���ݣ��ŵ���һ̨���ݵ�������
				ElevatorManage::GetInstance()->dispatchPassage(passage_ptr);
			}
			else {
				taskIn(passage_ptr,true);
				//����Ŀ�ĵ�����Ӧ������
				//peopleNeedOff[passage_ptr->iAimFloor]++;
				//��ǰȫ�����ڵ����ϵĳ˿�����
				landpassage++;
				PassageCount++;
			}
		}


		outer_people.erase(passageItr);
		PassageManage::GetInstance()->delWaitPassageIn(currentpos);

		return true;
	}
	//��Է�Ŀ�ĵ�����
	else {
		//������з��������һ�µĻ�
		if (hasSameDir() || needturn()) {
			map<int, set<Passage*>>::iterator passageItr2 = PassageManage::GetInstance()->map_wait_passage.find(currentpos);
			if (passageItr2 != PassageManage::GetInstance()->map_wait_passage.end()) {
				//��
				if (dir == 1) {
					outer_call[currentpos][0] = false;
					ElevatorManage::GetInstance()->outer_call_manage[currentpos][0] = false;
				}
				//��
				else if (dir == 2) {
					outer_call[currentpos][1] = false;
					ElevatorManage::GetInstance()->outer_call_manage[currentpos][1] = false;
				}
				//�ϵ��ݵĳ˿�����
				int passageIncount = 0;
				
				for (Passage* passage_ptr : passageItr2->second) {
					//����˿ͷ����
					if (passage_ptr->dir == (unsigned char)dir) {
						if (PassageCount + 1 > CAPACITY) {
							//�ٷ���һ̨����
							ElevatorManage::GetInstance()->dispatchPassage(passage_ptr);
							ElevatorManage::GetInstance()->outer_call_manage[currentpos][0] = false;
						}
						else {
							taskIn(passage_ptr, false);
							//��ǰȫ�����ڵ����ϵĳ˿�����
							landpassage++;
							//��ǰ���ݵĳ˿�����
							PassageCount++;

							passageIncount++;
						}
					}
				}


				//�ӵȴ�������ɾ��
				PassageManage::GetInstance()->delWaitPassageIn(currentpos);

				//��������и߷�Ļ�,��¼����������ʷ����
				if (currentMode == UP_PEAK_MODE ) {
					//����ϵ��ݵ�������Ϊ0�Ļ�
					if (passageIncount != 0) {
						//��¼ģ��
						ElevatorManage::GetInstance()->AddElevatorRecode(passageIncount, currentpos, dir, iEleID);
					}
				}

				return true;
			}
		}
		return false;
	}
	return false;
}




//����������Ҫ�µĳ˿�
bool Elevator::passageOFF() {
	map<int, vector<Passage*>>::iterator passageItr = inner_people.find(currentpos);
	//�����Ƿ�����Ҫ�µĳ˿�
	if (passageItr != inner_people.end()) {
		for (Passage* passage_ptr : passageItr->second) {
			//��ǰȫ�����ڵ����ϵĳ˿�����
			landpassage--;
			//ȫ���Ѿ��µ��ݵĳ˿�����
			offpassage++;
			//�����ϵĳ˿�����
			PassageCount--;
			passage_ptr->iOffTime = currentTime;
		}
	}
	passageItr = inner_people.find(currentpos);
	if (passageItr != inner_people.end()) {
		inner_people.erase(passageItr);
		return true;
	}
	return false;
}

//�ж��Ƿ���Ҫת��
bool Elevator::needturn() {
	//�����ǰ������
	if (dir == 1) {
		//������г˿�����Ҫ���ϵ�
		for (auto& vec_pepple : inner_people) {
			if (vec_pepple.first > currentpos) {
				return false;
			}
		}
		//���Ŀ�ĵ�����
		for (auto vec_pepple : outer_people) {
			if (vec_pepple.first > currentpos) {
				return false;
			}
			else if (vec_pepple.first == currentpos) {
				for (auto people : vec_pepple.second) {
					if (people->iAimFloor > currentpos) {
						return false;
					}
				}
			}
			
		}
		//��Է�Ŀ�ĵ����ݣ����������Ҫ���ϵ�
		for (int f = currentpos + 1; f < FLOOR_COUNT + 1; f++) {
			if (outer_call[f][0] || outer_call[f][1]) {
				return false;
			}
		}
		if (outer_call[currentpos][0]) {
			return false;
		}

		return true;
	}
	else if (dir == 2) {
		//������г˿�����Ҫ���µ�
		for (auto& vec_pepple : inner_people) {
			if (vec_pepple.first < currentpos) {
				return false;
			}
		}
		for (auto vec_pepple : outer_people) {
			if (vec_pepple.first < currentpos) {
				return false;
			}
			else if (vec_pepple.first == currentpos) {
				for (auto people : vec_pepple.second) {
					if (people->iAimFloor < currentpos) {
						return false;
					}
				}
			}
		}
		//��Է�Ŀ�ĵ�����,���������Ҫ���µ�
		for (int f = 1; f < currentpos; f++) {
			if (outer_call[f][0] || outer_call[f][1]) {
				return false;
			}
		}
		if (outer_call[currentpos][1]) {
			return false;
		}
		return true;
	}
	return false;
}


//�ж��Ƿ���Ҫ����
bool Elevator::needopendoor() {
	//��������ź�
	//����г˿͵���Ŀ�ĵ�
	if (inner_people.find(currentpos) != inner_people.end()) {
		m_needoffpassage = true;
	}
	
	//��������ź�
	//�����Ҫת��,���������еķ���ֻҪ�г˿;���Ҫ����
	if (needturn()) {
		//���Ŀ�ĵ�����
		if (outer_people.find(currentpos) != outer_people.end()) {
			m_needonpassage = true;
		}
		//��Է�Ŀ�ĵ�����
		if (outer_call[currentpos][0] || outer_call[currentpos][1]) {
			m_needonpassage = true;;
		}
	}

	//�����ǰ������
	if (dir == 1) {
		//���Ŀ�ĵ�����
		map<int, vector<Passage*> >::iterator it = outer_people.find(currentpos);
		if (it != outer_people.end()) {
			for (Passage* p : it->second) {
				if (p->dir == 1) {
					m_needonpassage = true;
				}
			}
		}
		//��Է�Ŀ�ĵ�����
		if (outer_call[currentpos][0]) {
			m_needonpassage = true;
		}
	}
	//�����ǰ������
	else if (dir == 2) {
		//���Ŀ�ĵ�����
		map<int, vector<Passage*> >::iterator it = outer_people.find(currentpos);
		if (it != outer_people.end()) {
			for (Passage* p : it->second) {
				if (p->dir == 2) {
					m_needonpassage = true;
				}
			}
		}
		//��Է�Ŀ�ĵ�����
		if (outer_call[currentpos][1]) {
			m_needonpassage = true;
		}
	}
	return m_needoffpassage || m_needonpassage;
}




//���ݵ�״̬��Ǩ�ƺ���
void Elevator::update(unsigned char dest) {
	if (state == IDLE) {
		//���������һ�����л�������
		if (hasoutcall() || hasinncall()) {
			//TODO�ҵ������һ������,���ı䷽��
			//��ȡ��ʼ����
			dir = getinitdir();
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:IDLE=>MOVE" << endl;
			}
			state = MOVE;
		}
		else {
			//����ǿ���״̬�����˸߷�ģʽ�Ļ��������ڿ���״̬�Զ��ƶ�����Ӧ��λ�ã�����Ȼ�ǿ���״̬
			if (dest != INVALID_NUM) {
				unsigned char dir = getDirection((unsigned char)currentpos, dest);
				if (dir == 1) {
					currentheight += velocity * TIME_INTERVAL;//����0.3m
					currentpos = currentheight / 3 + 1;//���µ�ǰ¥��
				}
				if (dir == 2) {
					currentheight -= velocity * TIME_INTERVAL;//����0.3m
					currentpos = currentheight / 3 + 1;//���µ�ǰ¥��
				}
				if (iEleID == DEBUG_ENO) {
					cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << (int)dir << " c:" << PassageCount << "  STATE:IDLE=>IDLE" << endl;
				}
			}
		}
	}
	else if (state == OPEN_DOOR) {
		//���������������
		if (_t_controller > opendoor) {
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:OPEN_DOOR=>OFF_PASSENAGE" << endl;
			}
			state = OFF_PASSENAGE;
			_t_controller = 0;
		}
	}
	else if (state == OFF_PASSENAGE) {
		//�����µĳ˿�
		if (!m_needoffpassage) {
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:OFF_PASSENAGE=>LAND_PASSENAGE" << endl;
			}
			state = LAND_PASSENAGE;
			_t_controller = 0;
		}
		else {
			//���������������
			if (_t_controller > passagetakeoff) {
				passageOFF();
				//�³˿����
				m_needoffpassage = false;
				if (iEleID == DEBUG_ENO) {
					cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:OFF_PASSENAGE=>LAND_PASSENAGE" << endl;
				}
				state = LAND_PASSENAGE;
				_t_controller = 0;
				
			}
		}
	}
	else if (state == LAND_PASSENAGE) {
		//���������������
		//�����ϵĳ˿�
		if (!m_needonpassage) {
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:LAND_PASSENAGE=>CLOSE_DOOR" << endl;
			}
			state = CLOSE_DOOR;
			_t_controller = 0;
		}
		else {
			if (_t_controller > passagetakeoff) {
				passageIn();
				//�ϳ˿����
				m_needonpassage = false;
				//ȡ������
				if (dir == 1) {
					outer_call[currentpos][0] = false;
				}
				else if (dir == 2) {
					outer_call[currentpos][1] = false;
				}
				if (iEleID == DEBUG_ENO) {
					cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:LAND_PASSENAGE=>CLOSE_DOOR" << endl;
				}
				state = CLOSE_DOOR;
				_t_controller = 0;
			}
		}
	}
	else if (state == CLOSE_DOOR) {
		//���������������
		if (_t_controller > closedoor) {
			//����������
			if (hasoutcall() || hasinncall()) {
				//�ж��Ƿ���Ҫת��
				if (needturn()) {
					if (iEleID == DEBUG_ENO) {
						cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:CLOSE_DOOR=>TURN" << endl;
					}

					state = TURN;
					_t_controller = 0;
				}
				else {
					if (iEleID == DEBUG_ENO) {
						cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:CLOSE_DOOR=>MOVE" << endl;
					}
					state = MOVE;
				}

			}
			else {
				if (iEleID == DEBUG_ENO) {
					cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:CLOSE_DOOR=>IDLE" << endl;
				}
				state = IDLE;
			}
		}
	}
	else if (state == TURN) {
		//���������������
		if (_t_controller > turn) {

			if (dir == 1) {
				dir = 2;
			}
			else if (dir == 2) {
				dir = 1;
			}
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:TURN=>MOVE" << endl;
			}

			state = MOVE;
		}
	}

	else if (state == MOVE) {
		//�ж��Ƿ���Ҫ����
		if (needopendoor()) {
			//�еĻ�����״̬
			if (iEleID == DEBUG_ENO) {
				cout << "t:" << currentTime << " eno: " << iEleID << " p:" << currentpos << " d:" << dir << " c:" << PassageCount << "  STATE:MOVE=>OPEN_DOOR" << endl;
			}

			state = OPEN_DOOR;
			_t_controller = 0;
		}
		else {
			//����Ҫ�Ļ����µ�ǰλ��
			if (dir == 1) {
				currentheight += velocity * TIME_INTERVAL;//����0.3m
			}
			if (dir == 2) {
				currentheight -= velocity * TIME_INTERVAL;//����0.3m
			}
			//��ǰ¥��
			currentpos = currentheight / 3 + 1;//���µ�ǰ¥��
		}
	}

	//�����
	else if (state == STOP) {
		//TODO�ж��Ƿ����������ڵ�ǰ¥��
		//�еĻ����³˿ͣ������³˿ͣ������ϳ˿ͣ���ɾ��������
		//�ϳ˿͵�ʱ��ȥ��¼<����ʱ�䣬�Ͽ��������Ͽͺ�˿Ͱ������а�ť֮�󣬸ò�����зֲ����
		//���ޱ��¥���������
		//�޵Ļ�״̬�ĳ�IDLE
		//�еĻ�״̬�ĳ�MOVE
	}
	//cout << "eleno:" << this->iEleID << "  currentpos:" << this->currentpos << "  STATE:" << state << endl;
	_t_controller += TIME_INTERVAL;
}