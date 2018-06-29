#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

struct XY {
private:
	int x, y;
public:
	XY();
	XY(int x, int y);
	void set(int x, int y);
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
};

struct Button {
private:
	XY pos1;
	XY pos2;
	bool selected;
	bool pushed;
	bool clicked;
	unsigned int white;
	unsigned int red;
	unsigned int black;
	std::string message;
	void printMessage();
public:
	Button(XY pos1, XY pos2, std::string message="");
	void set(XY pos1, XY pos2);
	XY getPos1();
	XY getPos2();
	int getX1();
	int getX2();
	int getY1();
	int getY2();
	bool isIn();
	bool isSelected();
	bool isPushed();
	bool isClicked();
	void disp();
	void select();
	void unselect();
	void toggleSelect();
	void push();
	void unpush();
	void click();
	void unclick();
};

bool selectButtonEvent(std::vector<Button> &buttons);

bool clickButtonEvent_send(Button &button);

bool clickButtonEvent_finish(Button &button);

bool selectNumberButtonEvent(std::vector<Button> &number_buttons);