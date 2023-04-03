#include "greedy.h"
#include"config.h"
#include"PassageManage.h"
#include"ElevatorManage.h"

using namespace std;
//�򵥵�ģ��������߷����ݲ���


//ȫ�ֱ���
int currentday = 0;
//��ǰʱ��
double currentTime = 0.0;
//��ǰʱ��
unsigned char currentMode = SAVING_MODE;
//�ϳ��ĳ˿�
int landpassage = 0;
//�³��ĳ˿�
int offpassage = 0;
//ģ�����ɵĳ˿�����
vector<Passage> vec_passages;//��һ��

//�ָ�
std::vector<std::string> stringSplit(const std::string& str, char delim) {
	std::size_t previous = 0;
	std::size_t current = str.find(delim);
	std::vector<std::string> elems;
	while (current != std::string::npos) {
		if (current > previous) {
			string temp = str.substr(previous, current - previous);
			elems.push_back(temp);
		}
		previous = current + 1;
		current = str.find(delim, previous);
	}
	if (previous != str.size()) {
		elems.push_back(str.substr(previous));
	}
	return elems;
}


int ReadPassage(string strline,Passage& p) {
	//�˿���Ϣ
	vector<string> vi;
	vi = stringSplit(strline, ',');
	int PassageID;
	int WorkFloor;
	int FromFloor;
	int AimFloor;
	double ComeTime;
	int day = stoi(vi[0]);
	PassageID = stoi(vi[1]);
	WorkFloor = stoi(vi[2]);
	FromFloor = stoi(vi[3]);
	AimFloor = stoi(vi[4]);
	ComeTime = stod(vi[5]);
	p = Passage(PassageID, WorkFloor, FromFloor, AimFloor, ComeTime);
	return day;
}

void resetDay(int d) {
	//����ȫ�ֱ���
	currentday = d;
	currentTime = 0.0;
	landpassage = 0;
	offpassage = 0;
	vec_passages.clear();
}

//��ʼ����
int Run() {
	int iPassageCount = vec_passages.size();
	//��ǰ��δ���ݵĳ˿�
	int currentIndex = 0;

	

	while (true) {
		if (currentIndex >= vec_passages.size()) {
			break;
		}

		//���������е��˵ĳ˿�
		if (vec_passages[currentIndex].iComeTime < currentTime) {
			//���ó˿ͷ������
			PassageManage::GetInstance()->addWaitPassage(&vec_passages[currentIndex]);
			int eleno = ElevatorManage::GetInstance()->dispatchPassage(&vec_passages[currentIndex]);
			currentIndex++;
		}
		else {
			//����Ѿ������������еĳ˿�,���µ���״̬������,һ���Ӵ���һ��ģʽ���
			if (abs(round(currentTime) - currentTime) < MIN_DIFF) {
				ElevatorManage::GetInstance()->ModeMonitor();
			}
			currentTime += TIME_INTERVAL;
			//���µ���״̬
			ElevatorManage::GetInstance()->Update();
		}
	}
	//ȫ��������ɣ��ȴ����ݵ���
	while (true) {
		//ȫ���˿��Ѿ�����
		if (offpassage >= iPassageCount) {
			break;
		}
		//ÿ���Ӽ��һ��
		if (abs(round(currentTime) - currentTime) < MIN_DIFF) {
			ElevatorManage::GetInstance()->ModeMonitor();
		}
		//ʱ���0.1��
		currentTime += TIME_INTERVAL;
		//���µ���״̬
		ElevatorManage::GetInstance()->Update();
	}
	return true;
}

double sum1 = 0;
double sum2 = 0;

int Result() {
	double avg_arrive_time = 0.0;
	double avg_wait_time = 0.0;
	for (auto passage_ptr : vec_passages) {
		avg_wait_time += passage_ptr.iLandTime - passage_ptr.iComeTime;
		avg_arrive_time += passage_ptr.iOffTime - passage_ptr.iComeTime;
	}
	avg_arrive_time /= vec_passages.size();
	avg_wait_time /= vec_passages.size();
	cout << "currentday: "<< currentday << endl;
	cout << "avg_wait_time:" << avg_wait_time << endl;
	cout << "avg_arrive_time:" << avg_arrive_time << endl;

	sum1 += avg_wait_time;
	sum2 += avg_arrive_time;

	return 0;
}


//ģ����
int main(){
	
	unsigned char eleNum = ELE_COUNT;
	//��ʼ�����ݵĲ���ģ��
	ElevatorManage::GetInstance()->Init(eleNum);


	//ͨ��gmock_main.cc����ģ��˿����ݣ����и߷�
	//����˿���Ϣ
	ifstream inFile;	//����ofstream����outFile
	inFile.open("./10day_up.csv");	//���ļ�
	string strline;
	while (getline(inFile, strline))
	{
		Passage p;
		int day = ReadPassage(strline, p);
		if (day == currentday) {
			vec_passages.push_back(p);
		}
		//�������һ��,�����굱��֮�������¸���ȫ�ֱ���
		else {
			Run();
			Result();
			cout << "reset:" << day << endl;
			resetDay(day);
			vec_passages.push_back(p);
		}
		
	}
	inFile.close();
	
	cout << "wait_time:" << sum1 << endl;
	cout << "arrive_time:" << sum2 << endl;
	return 0;
}