#include <shell.h>

void Shell::init(){
  rawInputsItr = rawInputs.begin();
  chrScreen->putChar(x ++, y, L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(x ++, y, L' ', TFT_WHITE, TFT_BLACK);
}
void Shell::onkeydown(char key, char c, bool ctrl){
  switch(key){
    case 0x2A: // BS
      backSpace();
    break;
    case 0x50: // <-
      left();
    break;
    case 0x4F: // ->
      right();
    break;
    case 0x52: // ^
      up();
    break;
    case 0x51: // v
      down();
    break;
    default:
      if(c == '\r'){
        enter();
      }else{
        onChar(c);
      }
  }
}
void Shell::backSpace(){
  rawInputs.pop_back();
  x -= 1;
  chrScreen->putChar(x, y, 0, TFT_WHITE, TFT_BLACK);
}
void Shell::right(){
}
void Shell::left(){
}
void Shell::up(){
}
void Shell::down(){
}
void Shell::enter(){
  x = 0;
  nextLine();
  int i = 0;
  wchar_t cmd[256];
  for(vector<wchar_t>::iterator itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
    chrScreen->putChar(x ++, y, *itr, TFT_WHITE, TFT_BLACK);
    cmd[i] = *itr;
    i ++;
  }
  cmd[i] = 0;

  if(wcsncmp(cmd, L"ls", 256) == 0){
#ifdef ESP32
    File root = SPIFFS.open("/");
    File f = root.openNextFile();
    while(f){
      chrScreen->putString(0, y, (char*)f.name(), TFT_WHITE, TFT_BLACK);
      f = root.openNextFile();
      nextLine();
    }
#endif
  }else if(wcsncmp(cmd, L"clear", 256) == 0){
    for(int i = 0; i < chrScreen->getMaxLine(); i ++){
      chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
    }
    x = 0;
    y = 0;
  }else if(wcsncmp(cmd, L"edit", 256) == 0){
    setNextTask(editor);
    for(int i = 0; i < chrScreen->getMaxLine(); i ++){
      chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
    }
    return;
  }else{
    chrScreen->putString(0, y, L"unknown command", TFT_WHITE, TFT_BLACK);
  }

  x = 0;
  rawInputs.clear();
  rawInputsItr = rawInputs.begin();

  nextLine();
  chrScreen->putChar(x ++, y, L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(x ++, y, L' ', TFT_WHITE, TFT_BLACK);
}
void Shell::onChar(wchar_t c){
  chrScreen->putChar(x ++, y, c, TFT_WHITE, TFT_BLACK);
  rawInputs.push_back(c);
}
void Shell::nextLine(){
  y ++;
  if(chrScreen->getMaxLine() == y){
    y = 0;
  }
}