#include <Button.h>

Button::Button(Diablo_Serial_4DLib* display, word  x, word  y, word  width, word  height, word  buttonColor, word  textColor, word  font, word textSize, char* text) {
  _disabled = false;
  _display = display;
  _x = x, _y = y;
  _width = width, _height = height;
  _buttonColor = buttonColor, _textColor = textColor;
  _font = font;
  _texteSize = textSize;
  _text = text;
}

void Button::disable(bool disabled) {
  _disabled = disabled;
}

void Button::draw() {
  _display.gfx_Button(0, _x, _y, _buttonColor, 0, _font, _width, _height, "");
  _display.txt_Set(FONT_SIZE, _textSize);
  _display.txt_Set(FONT_ID, _font);
  _display.txt_Set(TEXT_COLOUR, _textColor);
  _display.txt_Set(TEXT_BACKGROUND, _buttonColor);
  _display.putstr(_text);
}

bool Button::isPressed() {
  return !_disabled && 
         _display.touch_Get(TOUCH_PRESSED) && 
         _x <= _display.touch_Get(TOUCH_GETX) <= _x+_width && 
         _y <= _display.touch_Get(TOUCH_GETY) <= _y+_height;
}
