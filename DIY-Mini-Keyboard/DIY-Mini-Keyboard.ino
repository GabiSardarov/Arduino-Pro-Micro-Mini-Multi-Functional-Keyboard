#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- KEYPAD ----------
const int rows[3] = { 16, 14, 4 };
const int cols[3] = { 5, 6, 7 };
const int extraBtn = 8;

// To store currently pressed keys
bool keystate[3][3] = { false };
bool lastKey[3][3];
static bool lastBtn = HIGH;
int mode = 0;  // 0 and 1=calculator 2=to keyboard
String input = "";
void setup() {
  Keyboard.begin();
  // --- Keypad setup ---
  for (int r = 0; r < 3; r++) {
    pinMode(rows[r], OUTPUT);
    digitalWrite(rows[r], HIGH);
  }

  for (int c = 0; c < 3; c++) {
    pinMode(cols[c], INPUT_PULLUP);
  }

  pinMode(extraBtn, INPUT_PULLUP);

  // --- OLED setup ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED NOT FOUND");
    while (1)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);  // 128x32 — текст читаемый
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System ready");
  display.display();
}

void scanMatrix() {
  for (int r = 0; r < 3; r++) {
    digitalWrite(rows[r], LOW);

    for (int c = 0; c < 3; c++) {
      keystate[r][c] = (digitalRead(cols[c]) == LOW);
    }

    digitalWrite(rows[r], HIGH);
  }
}


void handleModeButton() {
  bool btn = digitalRead(extraBtn);

  if (btn == LOW && lastBtn == HIGH) {  // edge detection
    if (keystate[0][0]) {
      mode = (mode == 2) ? 0 : 2;
    } else {
      mode = (mode + 1) % 2;  
    }
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("Mode: ");
    display.print(mode);
  }

  lastBtn = btn;
}

void handleKeys() {
  if (keystate[2][1] && keystate[2][2]) {
    input = "";
    return;
  }
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      bool pressed = keystate[r][c];

      // edge detection
      if (pressed && !lastKey[r][c]) {

        if (mode == 0) {  // цифры
          char value = '1' + r * 3 + c;
          input += value;
        }

        else if (mode == 1) {  // calc map
          char mathMap[3][3] = {
            { '+', '-', '*' },
            { '/', '=', ' ' },
            { ' ', ' ', ' ' }
          };
          char value = mathMap[r][c];

          if (value == '=') {
            float result = calculate(input);
            input = String(result);
          } else if (value != ' ') {
            input += value;
          }
        }

        else if (mode == 2) {  // keyboard
          Keyboard.press(KEY_LEFT_GUI);
          Keyboard.press('r');
          Keyboard.releaseAll();
          delay(300);  
          Keyboard.print("browser");
          Keyboard.write(KEY_RETURN);
          delay(1000);  
          Keyboard.print("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
          Keyboard.write(KEY_RETURN);
        }
      }
      lastKey[r][c] = pressed;
    }
  }
}

float calculate(String input) {
  float numbers[10];  // max 10 nums
  char ops[10];       // max 10 ops
  int numCount = 0;
  int opCount = 0;

  String current = "";

  // split string into operations and numbers
  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c >= '0' && c <= '9') {
      current += c;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      numbers[numCount++] = current.toFloat();
      ops[opCount++] = c;
      current = "";
    }
  }

  // last number
  if (current.length() > 0) {
    numbers[numCount++] = current.toFloat();
  }

  // first * and / 
  for (int i = 0; i < opCount; i++) {
    if (ops[i] == '*' || ops[i] == '/') {
      if (ops[i] == '*') numbers[i] = numbers[i] * numbers[i + 1];
      if (ops[i] == '/') numbers[i] = numbers[i] / numbers[i + 1];

      // apply changes to arrays for future scan
      for (int j = i + 1; j < numCount - 1; j++) numbers[j] = numbers[j + 1];
      for (int j = i; j < opCount - 1; j++) ops[j] = ops[j + 1];

      numCount--;
      opCount--;
      i--;  
    }
  }

  // then + and -
  float result = numbers[0];
  for (int i = 0; i < opCount; i++) {
    if (ops[i] == '+') result += numbers[i + 1];
    if (ops[i] == '-') result -= numbers[i + 1];
  }

  return result;
}

void loop() {
  display.setCursor(0, 0);
  display.setTextSize(2);
  scanMatrix();
  handleModeButton();
  handleKeys();


  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.print(input);
  display.display();
}
