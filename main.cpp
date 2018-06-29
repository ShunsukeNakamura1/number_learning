#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <random>

#define INNUM 25
#define HIDDENNUM 10
#define OUTNUM 10
#define LEARN_RATE 0.005

struct TRAINDATA {
private:
	bool data[INNUM];//トレーニングデータデータ
	int y_hat; //トレーニングデータの正解 0 ～ 9
	int y_hat_arr[10];
public:
	TRAINDATA(bool *x, int y_hat);
	bool *getData();
};

struct tINPUT {
private:
	double data[INNUM]; //データ格納用int data[25]
public:
	void setData(bool *x);
	double *getData();
	void dispData();
};

struct tHIDDEN {
private:
	double data[HIDDENNUM];
	double bias[HIDDENNUM];
	double w[HIDDENNUM][INNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tHIDDEN();
	void forward(double *x);
	double *getData();
	void dispData();
};

struct tOUTPUT {
private:
	double data[OUTNUM];
	double bias[OUTNUM];
	double w[OUTNUM][HIDDENNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tOUTPUT();
	void forward(double *x);
	double *getData();
	void dispData();
};

int main() {
	HANDLE hPipe;
	std::random_device rand;
	std::vector<TRAINDATA> train;//教師データ
	bool x[INNUM] = { 0 };//入力データ
	tINPUT in;     //入力層
	tHIDDEN hidden;//中間層
	tOUTPUT out;   //出力層
	double *input_data;//入力層の出力
	double *hidden_data;//中間層の出力
	double *output_data;//出力層の出力

	//データセット
	std::cout << "教師データ入力" << std::endl;
	bool szBuff[INNUM] = { false };
	int train_number = 121212;
	DWORD dwNumberOfBytesRead;
	hPipe = CreateNamedPipe("\\\\.\\pipe\\mypipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_WAIT, 3, 0, 0, 100, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
		std::cout << "Pipe Error1" << std::endl;
		return 1;
	}
	if (!ConnectNamedPipe(hPipe, NULL)) {
		CloseHandle(hPipe);
		std::cout << "Pipe Error2" << std::endl;
		return 1;
	}
	while (1) {
		if (!ReadFile(hPipe, szBuff, sizeof(szBuff), &dwNumberOfBytesRead, NULL)) {
			break;
		}
		for (int i = 0; i<INNUM; i++) {
			printf(szBuff[i] ? "1" : "0");
			if ((i + 1) % 5 == 0) {
				printf("\n");
			}
		}
		for (int i = 0; i<INNUM; i++) {
			printf(szBuff[i] ? "■" : "□");
			if ((i + 1) % 5 == 0) {
				printf("\n");
			}
		}
		if (!ReadFile(hPipe, &train_number, sizeof(train_number), &dwNumberOfBytesRead, NULL)) {
			break;
		}
		printf("number: %d\n", train_number);
		train.push_back(TRAINDATA(szBuff, train_number));
	}

	//学習部分
	in.setData(train[0].getData());
	in.dispData();
	input_data = in.getData();

	hidden.forward(input_data);
	hidden.dispData();
	hidden_data = hidden.getData();
	
	out.forward(hidden_data);
	out.dispData();
	output_data = out.getData();

	int test;
	std::cin >> test;
	return 0;
}

/*--------------------*/
//
//TRAINDATA
//
TRAINDATA::TRAINDATA(bool *x, int y_hat) {
	for (int i = 0; i < INNUM; i++) {
		data[i] = x[i];
	}
	this->y_hat = y_hat;
	for (int i = 0; i < 10; i++) {
		y_hat_arr[i] = i == y_hat ? 1 : 0;
	}
}
bool* TRAINDATA::getData() {
	return data;
}
/*--------------------*/
//
//TINPUT
//
//bool *x  << bool x[INNUM]
void tINPUT::setData(bool *x) {
	for (int i = 0; i < INNUM; i++) {
		data[i] = x[i] ? 1 : 0;
	}
}
double* tINPUT::getData() {
	return data;
}
void tINPUT::dispData() {
	std::cout << "INPUT LAYER DATA" << std::endl;
	for (int i = 0; i < INNUM; i++) {
		std::cout << data[i] << " ";
		if ((i + 1) % 5 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}
/*--------*/
//
//tHIDDEN
//
tHIDDEN::tHIDDEN() {
	std::random_device rand;
	for (int i = 0; i < HIDDENNUM; i++) {
		data[i] = 0;
		bias[i] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
		for (int j = 0; j < INNUM; j++) {
			w[i][j] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
		}
	}
}

//return double sum[HIDDENNUM]
void tHIDDEN::sum(double *x, double *sum) {
	for (int i = 0; i < HIDDENNUM; i++) {
		for (int j = 0; j < INNUM; j++) {
			sum[i] += x[j] * w[i][j];
		}
	}
}

//set double sigmoid(sum[HIDDENNUM]) to data[HIDDENNUM] 
void tHIDDEN::sigmoid(double *sum) {
	for (int i = 0; i < HIDDENNUM; i++) {
		data[i] = 1.0 / (1.0 + exp(-sum[i]));
	}
}

void tHIDDEN::forward(double *x) {
	double sum_[HIDDENNUM];
	for (int i = 0; i < HIDDENNUM; i++) {
		sum_[i] = bias[i];
	}
	sum(x, sum_);
	sigmoid(sum_); //ここでdata[]に格納
}
double* tHIDDEN::getData() {
	return data;
}
void tHIDDEN::dispData() {
	std::cout << "HIDDEN LAYER DATA" << std::endl;
	for (int i = 0; i < HIDDENNUM; i++) {
		std::cout << data[i] << " ";
		if ((i + 1) % 5 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}
/*--------------------*/
//
//tOUTPUT
//
tOUTPUT::tOUTPUT() {
	std::random_device rand;
	for (int i = 0; i < OUTNUM; i++) {
		data[i] = 0;
		bias[i] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
		for (int j = 0; j < HIDDENNUM; j++) {
			w[i][j] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
		}
	}
}
void tOUTPUT::sum(double *x, double *sum) {
	for (int i = 0; i < OUTNUM; i++) {
		for (int j = 0; j < HIDDENNUM; j++) {
			sum[i] += x[j] * w[i][j];
		}
	}
}
void tOUTPUT::sigmoid(double *sum) {
	for (int i = 0; i < OUTNUM; i++) {
		data[i] = 1.0 / (1.0 + exp(-sum[i]));
	}
}
void tOUTPUT::forward(double *x) {
	double sum_[OUTNUM];
	for (int i = 0; i < OUTNUM; i++) {
		sum_[i] = bias[i];
	}
	sum(x, sum_);
	sigmoid(sum_); //ここでdata[]に格納
}
double* tOUTPUT::getData() {
	return data;
}
void tOUTPUT::dispData() {
	std::cout << "OUTPUT LAYER DATA" << std::endl;
	for (int i = 0; i < OUTNUM; i++) {
		std::cout << data[i] << " ";
		if ((i + 1) % 5 == 0) {
			std::cout << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}