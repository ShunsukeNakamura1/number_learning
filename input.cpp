//�{�^���C�x���g�����������o�ɂ��Ď�������΂悩�����Ƃ����������������
#pragma once
#include "DxLib.h"
#include "options.h"

#define BUTTON_SIZE 50
#define BUTTON_SPACE 10
#define WIDTH 5
#define HEIGHT 5
#define NUMBER_SIZE 30
#define TOTAL WIDTH*HEIGHT

void setting();
void setInput(std::vector<Button> &buttons, bool *x);
void setNumber(std::vector<Button> &number_buttons, int *number);
void sendData(HANDLE hPipe, bool *x, int number);
void disp(std::vector<Button> &buttons, std::vector<Button> &number_buttons, Button post, Button finish);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setting();
	if (DxLib_Init() == -1)    // �c�w���C�u��������������
	{
		return -1;    // �G���[���N�����璼���ɏI��
	}

	//�ϐ���
	//----------�w�K�f�[�^�p�{�^��----------
	std::vector<Button> buttons; 
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			XY pos1(j * (BUTTON_SIZE + BUTTON_SPACE), i * (BUTTON_SIZE + BUTTON_SPACE));
			XY pos2(j * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SIZE, i * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SIZE);
			buttons.push_back(Button(pos1, pos2));
		}
	}
	//----------���t�f�[�^�p�{�^��----------
	std::vector<Button> number_buttons;
	for (int i = 0; i < 10; i++) {
		XY pos1(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE, i * NUMBER_SIZE);
		XY pos2(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE + NUMBER_SIZE, i * NUMBER_SIZE + NUMBER_SIZE);
		number_buttons.push_back(Button(pos1, pos2, std::to_string(i)));
	}
	number_buttons[0].select();
	//----------�f�[�^���M�p�{�^��----------
	XY pos1(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE + NUMBER_SIZE + 10, 0);
	XY pos2(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE + NUMBER_SIZE + 50, 30);
	Button send(pos1, pos2, "���M");
	//---------�I���{�^��----------
	pos1 = XY(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE + NUMBER_SIZE + 10, 40);
	pos2 = XY(WIDTH * (BUTTON_SIZE + BUTTON_SPACE) + BUTTON_SPACE + NUMBER_SIZE + 50, 70);
	Button finish(pos1, pos2, "�I��");

	bool x[TOTAL];	//���M�f�[�^(���͐M��)
	int number;		//���M�f�[�^(���t�M��)
	bool isChanged1;//���̓f�[�^�p
	bool isChanged2;//���M�{�^���p
	bool isChanged3;//���t�f�[�^�p
	bool isChanged4;//�I���{�^���p
	//�ʐM�֌W
	HANDLE hPipe;
	DWORD dwNumberOfBytesRead;
	DWORD dwNumberofBytesWritten;
	hPipe = CreateFile("\\\\.\\pipe\\mypipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == INVALID_HANDLE_VALUE) {
		DxLib_End();        // �c�w���C�u�����g�p�̏I������
		return 1;
	}
	//
	disp(buttons, number_buttons, send, finish);
	setInput(buttons, x);
	setNumber(number_buttons, &number);
	//���C�����[�v
	while (!CheckHitKey(KEY_INPUT_ESCAPE) && !ProcessMessage()) {
		isChanged1 = selectButtonEvent(buttons);//�{�^���̏�Ԃ��ω����Ă�����`�悷��
		isChanged2 = clickButtonEvent_send(send);
		isChanged3 = selectNumberButtonEvent(number_buttons);
		isChanged4 = clickButtonEvent_finish(finish);
		if (isChanged1 || isChanged2 || isChanged3 | isChanged4) {
			if (isChanged1) {
				setInput(buttons, x);
			}
			else if (isChanged2) {
				if (send.isClicked()) {
					sendData(hPipe, x, number);
				}
			}
			else if (isChanged3) {
				setNumber(number_buttons, &number);
			}
			else if (isChanged4) {
				CloseHandle(hPipe);
				break;
			}
			disp(buttons, number_buttons, send, finish);
		}
	}

	DxLib_End();        // �c�w���C�u�����g�p�̏I������

	return 0;        // �\�t�g�̏I��
}

void setting() {
	// �E�C���h�E���[�h�ɕύX
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(800, 600, 32);
}

//���̓f�[�^�Z�b�g
void setInput(std::vector<Button> &buttons, bool *x) {
	for (int i = 0; i < buttons.size(); i++) {
		x[i] = buttons[i].isSelected();
	}
}

//���t�f�[�^�Z�b�g
void setNumber(std::vector<Button> &number_buttons, int *number) {
	for (int i = 0; i < number_buttons.size(); i++) {
		if (number_buttons[i].isSelected()) {
			*number = i;
		}
	}
}

void sendData(HANDLE hPipe, bool *x, int number) {
	DWORD dwNumberofBytesWritten;
	if (!WriteFile(hPipe, x, 25, &dwNumberofBytesWritten, NULL)) {
		printfDx("data���M�G���[1\n");
		return;
	}
	if (!WriteFile(hPipe, &number, 4, &dwNumberofBytesWritten, NULL)) {
		printfDx("data���M�G���[2\n");
		return;
	}
}
	//�`��
void disp(std::vector<Button> &buttons, std::vector<Button> &number_buttons, Button post, Button finish) {
	clsDx();
	ClearDrawScreen();
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i].disp();
	}
	for (int i = 0; i < number_buttons.size(); i++) {
		number_buttons[i].disp();
	}
	post.disp();
	finish.disp();
	for (int i = 0; i < TOTAL; i++) {
		printfDx("%d", buttons[i].isSelected() ? 1 : 0);
		if ((i + 1) % WIDTH == 0) {
			printfDx("\n");
		}
	}
	ScreenFlip();
}