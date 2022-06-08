#include <Diablo_Serial_4DLib.h>

#ifndef __BUTTON_H__
#define __BUTTON_H__

class Button {
  public:
    Button(Diablo_Serial_4DLib* display, word  x, word  y, word  width, word  height, word  buttonColor, word  textColor, word  font, word textSize, char* text);

    void disable(bool disabled);
    void draw();
    bool isPressed();

  private:
    bool _disabled;
    word _x, 
         _y, 
         _width, 
         _height, 
         _textColor,
         _buttonColor,
         _font,
         _textSize;
    char* _text;
    Diablo_Serial_4DLib* _display;

}

#endif // __BUTTON_H__
