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

struct tTRAINDATA {
private:
	bool data[INNUM];//�g���[�j���O�f�[�^�f�[�^
	int y_hat; //�g���[�j���O�f�[�^�̐��� 0 �` 9
	int y_hat_arr[OUTNUM];
public:
	tTRAINDATA(bool *x, int y_hat);
	bool *getData();
	int getY_hat();
	int *getY_hat_arr();
};

struct tINPUT {
private:
	double data[INNUM]; //�f�[�^�i�[�pint data[25]
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
	double u[HIDDENNUM];
	double ��[HIDDENNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tHIDDEN();
	void forward(double *x);
	void back(double *��, double **w);
	double *getData();
	double (*getW())[INNUM];
	double *get��();
	void dispData();
};

struct tOUTPUT {
private:
	double data[OUTNUM];
	double bias[OUTNUM];
	double w[OUTNUM][HIDDENNUM];
	double u[OUTNUM];
	double ��[OUTNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tOUTPUT();
	void forward(double *x);
	void back(tTRAINDATA train);
	double *getData();
	double (*getW())[HIDDENNUM];
	double *get��();
	void dispData();
};

double sigmoid(double x);
double sigmoid_dash(double x);

int main() {
	HANDLE hPipe;
	std::random_device rand;
	std::vector<tTRAINDATA> train;//���t�f�[�^
	bool x[INNUM] = { 0 };//���̓f�[�^
	tINPUT in;     //���͑w
	tHIDDEN hidden;//���ԑw
	tOUTPUT out;   //�o�͑w
	double *input_data;//���͑w�̏o��
	double *hidden_data;//���ԑw�̏o��
	double *output_data;//�o�͑w�̏o��

	//�f�[�^�Z�b�g
	std::cout << "���t�f�[�^����" << std::endl;
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
			printf(szBuff[i] ? "��" : "��");
			if ((i + 1) % 5 == 0) {
				printf("\n");
			}
		}
		if (!ReadFile(hPipe, &train_number, sizeof(train_number), &dwNumberOfBytesRead, NULL)) {
			break;
		}
		printf("number: %d\n", train_number);
		train.push_back(tTRAINDATA(szBuff, train_number));
	}

	//�w�K����
	in.setData(train[0].getData());
	in.dispData();
	input_data = in.getData();

	hidden.forward(input_data);
	hidden.dispData();
	hidden_data = hidden.getData();
	
	out.forward(hidden_data);
	out.dispData();
	output_data = out.getData();
	//back
	out.back(train[0]);
	double *buf_address[HIDDENNUM];
	for (int i = 0; i < HIDDENNUM; i++) {
		buf_address[i] = out.getW()[i];
	}
	hidden.back(out.get��(), buf_address);

	int test;
	std::cin >> test;
	return 0;
}


double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
}
double sigmoid_dash(double x) {
	return (1 - sigmoid(x)) * sigmoid(x);
}
/*--------------------*/
//
//TRAINDATA
//
tTRAINDATA::tTRAINDATA(bool *x, int y_hat) {
	for (int i = 0; i < INNUM; i++) {
		data[i] = x[i];
	}
	this->y_hat = y_hat;
	for (int i = 0; i < OUTNUM; i++) {
		y_hat_arr[i] = (i == y_hat ? 1 : 0);
	}
}
bool* tTRAINDATA::getData() {
	return data;
}
int tTRAINDATA::getY_hat() {
	return y_hat;
}
int* tTRAINDATA::getY_hat_arr() {
	return y_hat_arr;
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
	for (int i = 0; i < HIDDENNUM; i++) {
		u[i] = bias[i];
	}
	sum(x, u);
	sigmoid(u); //������data[]�Ɋi�[
}
void tHIDDEN::back(double *��, double **w) {
	for (int i = 0; i < HIDDENNUM; i++) {
		double buf = 0;
		for (int j = 0; j < OUTNUM; j++) {
			buf += ��[j] * w[j][i];
		}
		this->��[i] = sigmoid_dash(u[i]) * buf;
	}
}
double* tHIDDEN::getData() {
	return data;
}
double* tHIDDEN::get��() {
	return ��;
}
double(*tHIDDEN::getW())[INNUM] {
	return w;
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
	for (int i = 0; i < OUTNUM; i++) {
		u[i] = -bias[i];
	}
	sum(x, u);
	sigmoid(u); //������data[]�Ɋi�[
}
void tOUTPUT::back(tTRAINDATA train) {
	for (int i = 0; i < OUTNUM; i++) {
		��[i] = (train.getY_hat_arr()[i] - data[i]) * sigmoid_dash(u[i]);
	}
}
double* tOUTPUT::getData() {
	return data;
}
double(* tOUTPUT::getW())[HIDDENNUM] {
	return w;
}
double* tOUTPUT::get��() {
	return ��;
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