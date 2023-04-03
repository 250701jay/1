#include<vector>
#include<iostream>
#include<random>
#include<string>
#include<time.h>
#include<map>
#include<algorithm>
#include<fstream>
using namespace std;
//�˿�

//ʱ��
#define M 60
#define H 60*60


struct RecordTime
{
public:
	int iFrom;
	int iTo;
	double iComeTime;//����ʱ��
	double iLandTime;//���ϵ��ݵ�ʱ��
	double iOffTime;//�µ��ݵ�ʱ��
};

class Passage {
public:
	Passage() {}
	Passage(int PassageID, int WorkFloor) {
		iPassageID = PassageID;
		iWorkFloor = WorkFloor;
	}
	int iPassageID;
	int iWorkFloor;
	map<int, vector<RecordTime> > recored;//<�ڼ��죬��¼��ʱ��>
};

int main() {
	//���ɷֲ�ģ�����
	std::minstd_rand engine(time(NULL));
	int uptime = 5;
	std::poisson_distribution<int> distribution(uptime);
	//���ȷֲ����ʣ��˿͵�������1Сʱ�ڲ��ɷֲ�,5��������ȷֲ�
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	vector<Passage> vec_passages;
	//3��32��ֱ�����ɾ��ȷֲ��ĳ˿�������2¥��Ϊʳ��
	int personID = 1;
	for (int i = 3; i <= 32; i++) {
		//����ÿ��¥�㶼��10��30���ˣ���Ϊ����ͬ¥�����Զ��໥��ʶ�ĸ���Ϊ0.8����������һ����
		int personcount = rand() % 20 + 10;
		for (int p = 0; p < personcount; p++) {
			Passage passage(personID, i);
			vec_passages.push_back(passage);
			personID++;
		}
	}
	ofstream outFile;
	outFile.open("10day_up.csv");	//���ļ�
	for (auto& passage : vec_passages) {
		
		//outFile << passage.iPassageID << "," << passage.iWorkFloor << "\n";
		

		for (int d = 0; d < 10; d++) {
			//һ��һ��Сʱ����Ϊ12�Σ�ÿ5����֮����ȷֲ�
			//ÿ5���ӳ˿͵�����
			//����ȥ��λ�ϰ�
			double iComeTime = 8 * H +  5 * M * distribution(engine) + dis(engine) * 5 * M;
			RecordTime r1;
			r1.iFrom = 1;
			r1.iTo = passage.iWorkFloor;
			r1.iComeTime = iComeTime;
			r1.iLandTime = 0;
			r1.iOffTime = 0;
			passage.recored[d].push_back(r1);

			//outFile << d << "," << r1.iFrom <<"," << r1.iTo <<","<< r1.iComeTime << "\n";

			////����ȥʳ�óԷ����������߷�
			//RecordTime r2;
			//iComeTime = 11 * H + 5 * M * distribution(engine) + dis(engine) * 5 * M;
			//r2.iFrom = passage.iWorkFloor;
			//r2.iTo = 2;
			//r2.iComeTime = iComeTime;
			//r2.iLandTime = 0;
			//r2.iOffTime = 0;
			//passage.recored[d].push_back(r2);

			////outFile << d << "," << r2.iFrom << "," << r2.iTo << "," << r2.iComeTime << "\n";

			////ȥʳ�ó��꣬�ع�λ˯�����������߷�
			//RecordTime r3;
			//iComeTime = 12 * H + 5 * M * distribution(engine) + dis(engine) * 5 * M;
			//r3.iFrom = 2;
			//r3.iTo = passage.iWorkFloor;
			//r3.iComeTime = iComeTime;
			//r3.iLandTime = 0;
			//r3.iOffTime = 0;
			//passage.recored[d].push_back(r3);

			////outFile << d << "," << r3.iFrom << "," << r3.iTo << "," << r3.iComeTime << "\n";

			////18���°࣬�������и߷�
			//RecordTime r4;
			//iComeTime = 18 * H + 5 * M * distribution(engine) + dis(engine) * 5 * M;
			//r4.iFrom = passage.iWorkFloor;
			//r4.iTo = 1;
			//r4.iComeTime = iComeTime;
			//r4.iLandTime = 0;
			//r4.iOffTime = 0;
			//passage.recored[d].push_back(r4);

			//outFile << d << "," << r4.iFrom << "," << r4.iTo << "," << r4.iComeTime << "\n";
			
		}
	}
	


	for (int d = 0; d < 10; d++) {
		for (int j = 0; j < 1; j++) {
			//��������˳������
			sort(vec_passages.begin(), vec_passages.end(), [=](Passage a, Passage b) {
				return a.recored[d][j].iComeTime < b.recored[d][j].iComeTime;
			});
			//��������

			//�죬id������¥�㣬��ʼ¥�㣬Ŀ��¥�㣬ʱ��
			for (auto& p : vec_passages) {
				outFile << d << "," << p.iPassageID<<"," << p.iWorkFloor << "," << p.recored[d][j].iFrom << "," << p.recored[d][j].iTo << "," << p.recored[d][j].iComeTime << "\n";
			}
		}
		
	}
	outFile.close();	//�ر��ļ�
	return 0;
}