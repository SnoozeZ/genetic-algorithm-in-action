#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <fstream>
using namespace std;

#define POPUNATION 100			//�ܼ�����
#define CROSS_PROB 0.85			//����ĸ���
#define BIT_AMOUNT 30			//ÿ�������λ��
#define VARIATION_PROB 0.05		//����ĸ���
#define TIMES 1000				//�Ŵ�����
#define INT_AMOUNT 3			//����λ��
#define DE_AMOUNT 27			//С��λ��
#define random(x) (rand()%x)

struct assumption{
	assumption(){
		fit = 0;
		chosen1 = false;
		chosen2 = false;
		prob = 0;
	}
	bool *bit;					//��ʡ�ռ� true����1 false����0
	double fit;					//��Ӧ��
	double prob;				//ѡ�и���
	bool chosen1;				//��ѡ��ֱ���Ŵ�����
	bool chosen2;				//��ѡ���������
};																					
void init();														//��ʼ������ �������ֵ
double bToD(bool *bit);												//������bool����ת��Ϊʮ����double
double calFit(assumption ass);										//������Ӧ��
void calAssProb(vector <assumption> &assSet);						//������豻ѡ��ĸ���
void crossAssump(assumption &ass1,assumption &ass2);				//�������轻��		
vector <assumption> chooseNextGeneration(vector<assumption> assSet);//������һ��
bool compare(const assumption & ass1,const assumption & ass2);		//sort()��������
void geneitcAlg();														//�Ŵ��㷨ִ��
vector <assumption> assSet(POPUNATION);								//����vector

void init(){
	cout<<"��ʼ��ʼ��"<<endl;

	
	for(int i=0;i<POPUNATION;i++){
		bool *bit = new bool[BIT_AMOUNT];
		for(int j=0;j<BIT_AMOUNT;j++)
			bit[j] = (random(2)==1?true:false);
		assSet[i].bit = bit;
		assSet[i].fit = calFit(assSet[i]);
		assSet[i].chosen1 = assSet[i].chosen2 =false;
	}
	calAssProb(assSet);
	cout<<"��ʼ������"<<endl;
}
double calFit(assumption ass){
	double x = bToD(ass.bit);
	double fit = x+10*sin(5*x)+7*cos(4*x);
	return fit;
}
double bToD(bool *bit){
	double num = 0;
	for(int i=0;i<INT_AMOUNT;i++)
		num += pow((double)2,(INT_AMOUNT-i-1))*(bit[i]==true?1:0);
	for(int i=INT_AMOUNT;i<BIT_AMOUNT;i++)
		num += pow((double)2,(INT_AMOUNT-1-i))*(bit[i]==true?1:0);
	if(num<-100)
		int j=0;
	return num;
}
void calAssProb(vector <assumption> &assSet){
	double total = 0;
	double minFit = 1000;
	//�ҵ���С��fit,������total fit
	for(int i=0;i<POPUNATION;i++){
		if(assSet[i].fit<minFit)
			minFit = assSet[i].fit;
		total += assSet[i].fit; 
	}
	if(minFit<0){
	minFit = 0-minFit;
	for(int i=0;i<POPUNATION;i++)
		assSet[i].prob = (assSet[i].fit+minFit)/(total+POPUNATION*minFit);
	}
	else
		for(int i=0;i<POPUNATION;i++)
			assSet[i].prob = (assSet[i].fit)/(total);
}
void crossAssump(assumption &ass1,assumption &ass2){
	//��ȡ���Ƚ���
	assumption newAss1,newAss2;
	bool *bit1 = new bool[BIT_AMOUNT];
	bool *bit2 = new bool[BIT_AMOUNT];

	for(int i=0;i<BIT_AMOUNT;i++){
		int randNum = random(2);
		if(0 ==randNum){
			bit1[i] = ass1.bit[i];
			bit2[i] = ass2.bit[i];
		}
		else{
			bit1[i] = ass2.bit[i];
			bit2[i] = ass1.bit[i];
		}
	}
	ass1.bit = bit1;
	ass2.bit = bit2;
	ass1.fit = calFit(ass1);
	ass2.fit = calFit(ass2);
}
vector <assumption> chooseNextGeneration(vector<assumption> assSet){
	sort(assSet.begin(),assSet.end(),compare);
	vector <assumption> newAssSet;
	double randNum;
	double accuNum;
	int cross = CROSS_PROB*POPUNATION/2;
	int direct = POPUNATION-cross*2;
	//ѡֱ���Ŵ���direct������
	while(direct!=0){
		randNum = (double)random(10000)/10000;
		accuNum =0;
		for(int i=0;i<POPUNATION;i++){
			accuNum += assSet[i].prob;
			if(randNum<accuNum){
				if(assSet[i].chosen1)
					break;				//�Ѿ���ѡ�����
				assSet[i].chosen1 = true;
				newAssSet.push_back(assSet[i]);
				direct--;
				break;
			}
		}
	}
	//ѡ�����cross��
	while(cross!=0){
		bool find1=false,find2=false;
		assumption ass1,ass2;
		//�ҵ�һ��
		while(true){
			randNum = (double)random(10000)/10000;
			accuNum =0;
			for(int i=0;i<POPUNATION;i++){
				accuNum += assSet[i].prob;
				if(randNum<accuNum){
					if(assSet[i].chosen2)
						break;
					assSet[i].chosen2 = true;
					ass1 = assSet[i];
					find1 = true;
					break;
				}
			}
			if(true == find1)
				break;
		}
		//�ҵڶ���
		while(true){
			randNum = (double)random(10000)/10000;
			accuNum =0;
			for(int i=0;i<POPUNATION;i++){
				accuNum += assSet[i].prob;
				if(randNum<accuNum){
					if(assSet[i].chosen2)
						break;
					assSet[i].chosen2 = true;
					ass2 = assSet[i];
					find2 = true;
					break;
				}
			}
			if(true == find2)
				break;
		}
		//���������ҵ��ˣ�����
		crossAssump(ass1,ass2);

		newAssSet.push_back(ass1);
		newAssSet.push_back(ass2);
		cross--;
	}
	//���쿩
	int variation = VARIATION_PROB*POPUNATION;
	for(int i=0;i<variation;i++){
		int a = random(POPUNATION);
		int b = random(BIT_AMOUNT);
		newAssSet[a].bit[b] = (1 == newAssSet[a].bit[b])?0:1;
	}
	
	for(int i=0;i<POPUNATION;i++){
		newAssSet[i].chosen1 = false;
		newAssSet[i].chosen2 = false;
		newAssSet[i].fit = calFit(newAssSet[i]);
	}
	calAssProb(newAssSet);

	return newAssSet;
}
void geneitcAlg(){
	ofstream out("output.txt");
	double maxFit=-999;
	double maxX=0;
	for(int time=0;time<TIMES;time++){	
		for(int i=0;i<POPUNATION;i++){
			if(assSet[i].fit>maxFit){
				maxFit = assSet[i].fit;
				maxX = bToD(assSet[i].bit);
			}

		}
		assSet = chooseNextGeneration(assSet);
	//	cout<<"��"<<time<<"�α��죬��ǰ����yֵΪ: "<<setprecision(10)<<maxFit<<",xΪ: "<<setprecision(10)<<maxX<<endl;
	//	out<<setprecision(10)<<maxFit<<endl;
	}
	out.close();
	cout<<"��"<<TIMES<<"���Ŵ�����ǰ����yֵΪ: "<<setprecision(10)<<maxFit<<",xΪ: "<<setprecision(10)<<maxX<<endl;
}
bool compare(const assumption & ass1,const assumption & ass2){
	return ass1.prob>ass2.prob;
}

int main(){
	 srand((int)time(0));
	 init();
	 geneitcAlg();

	 system("pause");
}