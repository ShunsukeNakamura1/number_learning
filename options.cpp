#include "options.h"

XY::XY() {}
XY::XY(int x, int y) {
	this->x = x;
	this->y = y;
}
void XY::set(int x, int y) {
	this->x = x;
	this->y = y;
}
void XY::setX(int x) {
	this->x = x;

}
void XY::setY(int y) {
	this->y = y;
}
int XY::getX() {
	return x;
}
int XY::getY() {
	return y;
}

Button::Button(XY pos1, XY pos2, std::string message) {
	this->pos1 = pos1;
	this->pos2 = pos2;
	this->message = message;
	selected = false;
	pushed = false;
	clicked = false;
	white = GetColor(255, 255, 255);
	red = GetColor(255, 0, 0);
	black = GetColor(0, 0, 0);
}
void Button::set(XY pos1, XY pos2) {
	this->pos1 = pos1;
	this->pos2 = pos2;
}
XY Button::getPos1() {
	return pos1;
}
XY Button::getPos2() {
	return pos2;
}
int Button::getX1() {
	return pos1.getX();
}
int Button::getX2() {
	return pos2.getX();
}
int Button::getY1() {
	return pos1.getY();
}
int Button::getY2() {
	return pos2.getY();
}
bool Button::isIn() {
	int mouse_x, mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	if (pos1.getX() <= mouse_x && mouse_x <= pos2.getX() && pos1.getY() <= mouse_y && mouse_y <= pos2.getY()) {
		return true;
	}
	else {
		return false;
	}
}
bool Button::isSelected() {
	return selected;
}
bool Button::isPushed() {
	return pushed;
}
bool Button::isClicked() {
	return clicked;
}
void Button::disp() {
	if (pushed) {
		DrawBox(getX1(), getY1(), getX2(), getY2(), red, FALSE);
	}
	else if (selected) {
		DrawBox(getX1(), getY1(), getX2(), getY2(), white, TRUE);
	}
	else if (clicked) {
		DrawBox(getX1(), getY1(), getX2(), getY2(), red, TRUE);
	}
	else {
		DrawBox(getX1(), getY1(), getX2(), getY2(), white, FALSE);
	}
	printMessage();
}
void Button::printMessage() {
	DrawString(getX1(), getY1(), message.c_str(), isSelected() ? black : white);
}
void Button::select() {
	selected = true;
}
void Button::unselect() {
	selected = false;
}
void Button::toggleSelect() {
	selected = !selected;
}
void Button::push() {
	pushed = true;
}
void Button::unpush() {
	pushed = false;
}
void Button::click() {
	clicked = true;
}
void Button::unclick() {
	clicked = false;
}

//–ß‚è’l‚Íó‘Ô‚ª•Ï‰»‚µ‚½‚©‚Ç‚¤‚©
bool selectButtonEvent(std::vector<Button> &buttons) {
	static bool click = false;
	bool isChanged = false;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !click) {
		for (int i = 0; i < (int)buttons.size(); i++) {
			if (buttons[i].isIn()) {
				buttons[i].push();
				isChanged = true;
			}
		}
		click = true;
	}
	else if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && click) {}
	else {
		for (int i = 0; i < (int)buttons.size(); i++) {
			if (buttons[i].isPushed()) {
				if (buttons[i].isIn()) {
					buttons[i].toggleSelect();
				}
				buttons[i].unpush();
				isChanged = true;
			}
		}
		click = false;
	}
	return isChanged;
}

//–ß‚è’l‚Íó‘Ô‚ª•Ï‰»‚µ‚½‚©‚Ç‚¤‚©
bool clickButtonEvent_send(Button &button) {
	static bool click = false;
	bool isChanged = false;

	if (button.isClicked()) {
		button.unclick();
		isChanged = true;
	}
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !click) {
		if (button.isIn()) {
			button.push();
			isChanged = true;
		}
		click = true;
	}
	else if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && click) {}
	else {
		if (button.isPushed()) {
			if (button.isIn()) {
				button.click();
			}
			button.unpush();
			isChanged = true;
		}
		click = false;
	}
	return isChanged;
}

bool clickButtonEvent_finish(Button &button) {
	static bool click = false;
	bool isChanged = false;

	if (button.isClicked()) {
		button.unclick();
		isChanged = true;
	}
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !click) {
		if (button.isIn()) {
			button.push();
			isChanged = true;
		}
		click = true;
	}
	else if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && click) {}
	else {
		if (button.isPushed()) {
			if (button.isIn()) {
				button.click();
			}
			button.unpush();
			isChanged = true;
		}
		click = false;
	}
	return isChanged;
}

bool selectNumberButtonEvent(std::vector<Button> &number_buttons) {
	static bool click = false;
	bool isChanged = false;
	static int selected_number = 0;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !click) {
		for (int i = 0; i < (int)number_buttons.size(); i++) {
			if (number_buttons[i].isIn()) {
				number_buttons[i].push();
				isChanged = true;
			}
		}
		click = true;
	}
	else if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && click) {}
	else {
		for (int i = 0; i < (int)number_buttons.size(); i++) {
			if (number_buttons[i].isPushed()) {
				if (number_buttons[i].isIn()) {
					if(number_buttons[i].isSelected()){

					}
					else {
						number_buttons[i].select();
						number_buttons[selected_number].unselect();
						selected_number = i;
					}
				}
				number_buttons[i].unpush();
				isChanged = true;
			}
		}
		click = false;
	}
	return isChanged;
}