#include "PassageManage.h"
#include "greedy.h"

Passage::Passage(int PassageID, int WorkFloor, int FromFloor, int AimFloor, double ComeTime) {
	iPassageID = PassageID;
	iWorkFloor = WorkFloor;
	iFromFloor = FromFloor;
	iAimFloor = AimFloor;
	iComeTime = ComeTime;
	iLandTime = 0.0;
	iOffTime = 0.0;
	eleno = -1;
	is_vip = 0;
	is_disabled = 0;
	//������˿͵ķ���
	dir = getDirection(iFromFloor, iAimFloor);

}





bool  PassageManage::addWaitPassage(Passage* p) {
	map_wait_passage[p->iFromFloor].insert(p);
	return true;
}


//���ݶ�������֮��ȥɾ���Ѿ����ݵĳ˿�
bool PassageManage::delWaitPassageIn(int currentfloor) {
	map<int, set<Passage*>>::iterator passageItr2 = PassageManage::GetInstance()->map_wait_passage.find(currentfloor);
	if (passageItr2 != PassageManage::GetInstance()->map_wait_passage.end()) {
		set<Passage*>::iterator passage_itr = passageItr2->second.begin();
		for (; passage_itr != passageItr2->second.end(); ) {
			//����Ѿ��ϵ�����
			if ((*passage_itr)->eleno != -1) {
				passage_itr = passageItr2->second.erase(passage_itr);
			}
			else {
				passage_itr++;
			}
		}
		if (passageItr2->second.size() == 0) {
			PassageManage::GetInstance()->map_wait_passage.erase(currentfloor);
		}
	}
	return false;
}
