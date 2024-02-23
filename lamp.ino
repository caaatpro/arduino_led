#include "FastLED.h"
#include "GyverButton.h"
#include "IRremote.h"

// IR
#define IR 4

// Лента
#define LED 3
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];
byte counter;
int bright = 255;
int effect = 0;
int r = 0;
int g = 0;
int b = 0;
int TOP_INDEX = int(NUM_LEDS / 2);
int BOTTOM_INDEX = 0;        // светодиод начала отсчёта
int EVENODD = NUM_LEDS % 2;

// Кнопка
#define BUTTON 2
boolean on = false;
GButton button(BUTTON);

void setBrightUp() {
  bright += 20;
  // Serial.println("setBrightUp");

  if (bright > 255) {
    bright = 255;
    return;
  }
}

void setBrightDown() {
  bright -= 20;
  // Serial.println("setBrightDown");

  if (bright < 20) {
    bright = 20;
    return;
  }
}

void setEffect(int value) {
  effect = value;
  // 0 - радугу
  // 1 - flash
  // 2 - strobe
  // 3 - fade

  // 4 - r
  if (value == 4) {
    r = 255;
    g = 0;
    b = 0;
  }

  // 5 - g
  if (value == 5) {
    r = 0;
    g = 255;
    b = 0;
  }

  // 6 - b
  if (value == 6) {
    r = 0;
    g = 0;
    b = 255;
  }

  // 7 - w
  if (value == 7) {
    r = 255;
    g = 255;
    b = 255;
  }

  // 8 - оранжевый
  if (value == 8) {
    r = 255;
    g = 89;
    b = 0;
  }

  // 9 - зеленый
  if (value == 9) {
    r = 60;
    g = 255;
    b = 0;
  }

  // 10 - синий
  if (value == 10) {
    r = 0;
    g = 72;
    b = 255;
  }

  // 11 - бледно оранжевый
  if (value == 11) {
    r = 255;
    g = 149;
    b = 0;
  }

  // 12 - совсем бледно оранжевый
  if (value == 12) {
    r = 186;
    g = 116;
    b = 19;
  }

  // 13 - желтый
  if (value == 13) {
    r = 255;
    g = 247;
    b = 0;
  }

  // 14 - голубой
  if (value == 14) {
    r = 0;
    g = 247;
    b = 255;
  }

  // 15 - зелёно-голубой
  if (value == 15) {
    r = 0;
    g = 227;
    b = 148;
  }

  // 16 - тёмно-синий
  if (value == 16) {
    r = 2;
    g = 26;
    b = 186;
  }

  // 17 - фиолетовый
  if (value == 17) {
    r = 131;
    g = 2;
    b = 186;
  }

  // 18 - бледно-фиолетовый
  if (value == 18) {
    r = 82;
    g = 0;
    b = 117;
  }

  // 19 - розовый
  if (value == 19) {
    r = 255;
    g = 0;
    b = 204;
  }
}

void setup() {
  // Serial.begin(9600);

  IrReceiver.begin(IR, ENABLE_LED_FEEDBACK);

  FastLED.addLeds<WS2811, LED, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(bright);
  pinMode(13, OUTPUT);
}

void loop() {
   if (IrReceiver.decode())
   {
      String ir_code = String(IrReceiver.decodedIRData.command, HEX);
      IrReceiver.resume();
      // Serial.println(ir_code);
     if (ir_code == "0") setBrightUp(); // яркость +
     if (ir_code == "1") setBrightDown(); // яркость -
     if (ir_code == "2") on = false; // выключить
     if (ir_code == "3") on = true; // включить
     if (ir_code == "17") setEffect(0); // smooth - радуга
     if (ir_code == "b") setEffect(1); // flash
     if (ir_code == "f") setEffect(2); // strobe
     if (ir_code == "13") setEffect(3); // fade
     if (ir_code == "4") setEffect(4); // r
     if (ir_code == "5") setEffect(5); // g
     if (ir_code == "6") setEffect(6); // b
     if (ir_code == "7") setEffect(7); // w
     if (ir_code == "8") setEffect(8); // оранжевый
     if (ir_code == "9") setEffect(9); // зеленый
     if (ir_code == "a") setEffect(10); // синий
     if (ir_code == "c") setEffect(11); // бледно оранжевый
     if (ir_code == "10") setEffect(12); // совсем бледно оранжевый
     if (ir_code == "14") setEffect(13); // желтый
     if (ir_code == "d") setEffect(14); // голубой
     if (ir_code == "11") setEffect(15); // зелёно-голубой
     if (ir_code == "15") setEffect(16); // тёмно-синий
     if (ir_code == "e") setEffect(17); // фиолетовый
     if (ir_code == "12") setEffect(18); // бледно-фиолетовый
     if (ir_code == "16") setEffect(19); // розовый
   }

  if (effect == 0) {
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(counter + i * 2, 255, 255);  // HSV. Увеличивать HUE (цвет)
      // умножение i уменьшает шаг радуги
    }
    counter++;        // counter меняется от 0 до 255 (тип данных byte)
    delay(5);
  } else if (effect == 1) {
    int random_bright = random(0, 255);
    int random_delay = random(10, 100);
    int random_bool = random(0, random_bright);
    if (random_bool < 10) {
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
        leds[i] = CHSV(160, 50, random_bright);
      }
      delay(random_delay);
    }
  } else if (effect == 2) {
    int idex = random(0, NUM_LEDS);
    int ihue = random(0, 255);
    one_color_all(0, 0, 0);
    leds[idex] = CHSV(ihue, 255, 255);
    delay(35);
  } else if (effect == 3) {
    int random_bright = random(0, 255);
    int random_delay = random(10, 100);
    int random_bool = random(0, random_bright);
    if (random_bool < 10) {
      for (int i = 0 ; i < NUM_LEDS; i++ ) {
        leds[i] = CHSV(160, 50, random_bright);
      }
      delay(random_delay);
    }
  } else {
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++ ) {
      leds[i].setRGB(r, g, b);
    }
  }

  button.tick();
  if (button.isHolded()) {
    on = !on;
  }

  if (on) {
    FastLED.setBrightness(bright);
  } else {
    FastLED.setBrightness(0);
  }

  FastLED.show();
}

//---FIND INDEX OF HORIZONAL OPPOSITE LED
int horizontal_index(int i) {
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX) {
    return BOTTOM_INDEX;
  }
  if (i == TOP_INDEX && EVENODD == 1) {
    return TOP_INDEX + 1;
  }
  if (i == TOP_INDEX && EVENODD == 0) {
    return TOP_INDEX;
  }
  return NUM_LEDS - i;
}

void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
}