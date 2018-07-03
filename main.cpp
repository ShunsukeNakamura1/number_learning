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
#define LEARN_RATE 0.1
#define ALPHA 0.1

struct tTRAINDATA {
private:
	bool data[INNUM];//�g���[�j���O�f�[�^�f�[�^
	int y_hat; //�g���[�j���O�f�[�^�̐��� 0 �` 9
	int y_hat_arr[OUTNUM];
public:
	tTRAINDATA();
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
	double delta_w_last[HIDDENNUM][INNUM];
	double delta_bias_last[HIDDENNUM];
	double u[HIDDENNUM];
	double ��[HIDDENNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tHIDDEN();
	void forward(double *x);
	void back(double *��, double **w);
	void update(double *x);
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
	double delta_w_last[OUTNUM][HIDDENNUM];
	double delta_bias_last[OUTNUM];
	double u[OUTNUM];
	double ��[OUTNUM];
	void sum(double *x, double *sum);
	void sigmoid(double *sum);
public:
	tOUTPUT();
	void forward(double *x);
	void back(tTRAINDATA train);
	void update(double *x);
	double *getData();
	double (*getW())[HIDDENNUM];
	double *get��();
	void dispData();
	void dispW();
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
	std::cout << "�w�K�J�n" << std::endl;
	//�w�K��
	for (int i = 0; i < 10000; i++) {//�Ƃ肠����1���񃋁[�v
		for (int j = 0; j < train.size(); j++) {//�g���[�j���O�f�[�^�����������s
			//�f�[�^�̃Z�b�g
			in.setData(train[j].getData());//in.dispData()�ŕ\����
			input_data = in.getData();

			//���`�d
			hidden.forward(input_data);//hidden.dispData()�ŕ\����
			hidden_data = hidden.getData();
			out.forward(hidden_data);//out.dispData()�ŕ\����

			//�t�`�d
			out.back(train[j]);
			double *buf_address[HIDDENNUM];
			for (int i = 0; i < HIDDENNUM; i++) {
				buf_address[i] = out.getW()[i];
			}
			hidden.back(out.get��(), buf_address);

			//�׏d�̍X�V
			hidden.update(in.getData());
			out.update(hidden.getData());
			//out.dispW();
		}
	}
	std::cout << "�w�K�I��" << std::endl;
	std::cout << "�e�X�g�f�[�^����" << std:: endl;
	tTRAINDATA test;
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
		test = tTRAINDATA(szBuff, train_number);
		in.setData(test.getData());
		input_data = in.getData();
		hidden.forward(input_data);
		hidden_data = hidden.getData();
		out.forward(hidden_data);
		out.dispData();
	}
	hidden.forward(input_data);
	hidden_data = hidden.getData();
	out.forward(hidden_data);
	out.dispData();
	int wait;
	std::cin >> wait;
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
tTRAINDATA::tTRAINDATA(){}
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
		delta_bias_last[i] = 0;
		for (int j = 0; j < INNUM; j++) {
			w[i][j] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
			delta_w_last[i][j] = 0;
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
//double *x < x[INNUM] ���͑w�̏o��
void tHIDDEN::update(double *x) {
	for (int i = 0; i < HIDDENNUM; i++) {
		for (int j = 0; j < INNUM; j++) {
			double delta_w = LEARN_RATE * ��[i] * x[j] + ALPHA * delta_w_last[i][j];
			w[i][j] += delta_w;
			delta_w_last[i][j] = delta_w;
		}
		double delta_bias = LEARN_RATE * ��[i] + ALPHA * delta_bias_last[i];
		bias[i] += delta_bias;
		delta_bias_last[i] = delta_bias;
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
		delta_bias_last[i] = 0;
		for (int j = 0; j < HIDDENNUM; j++) {
			w[i][j] = (double)(rand() % 100) / 100.0 * (rand() % 2 ? 1 : -1);
			delta_w_last[i][j] = 0;
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
//double *x < x[HIDDENNUM] ���ԑw�̏o��
void tOUTPUT::update(double *x) {
	for (int i = 0; i < OUTNUM; i++) {
		for (int j = 0; j < HIDDENNUM; j++) {
			double delta_w = LEARN_RATE * ��[i] * x[j] + ALPHA * delta_w_last[i][j];
			w[i][j] += delta_w;
			delta_w_last[i][j] = delta_w;
		}
		double delta_bias = LEARN_RATE * ��[i] + ALPHA * delta_bias_last[i];
		bias[i] += delta_bias;
		delta_bias_last[i] = delta_bias;
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
void tOUTPUT::dispW() {
	std::cout << "OUTPUT LAYER W" << std::endl;
	for (int i = 0; i < OUTNUM; i++) {
		for (int j = 0; j < HIDDENNUM; j++) {
			std::cout << w[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-----------" << std::endl;
}