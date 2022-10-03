#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "defines.h"

#define HID_CUSTOM_LAYOUT
#define LAYOUT_US_ENGLISH

#include <HID-Project.h>

int platform = PLATFORM_WINDOWS;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &WIRE);
Adafruit_GFX_Button numLock;
Adafruit_GFX_Button capsLock;

#define NUMLOCK_ON_COLOR      SSD1306_WHITE 
#define NUMLOCK_OFF_COLOR     SSD1306_BLACK
#define NUMLOCK_BORDER_COLOR  SSD1306_WHITE 

bool numlock_state = false;
bool caps_state = false;

unsigned long t;

void setup() {
  Serial.begin(9600);
  Consumer.begin();
  BootKeyboard.begin();
    
  numLock.initButtonUL(&display, 0, 0, 32, 32, NUMLOCK_BORDER_COLOR, NUMLOCK_ON_COLOR, NUMLOCK_OFF_COLOR, "NUM", 1);
  capsLock.initButtonUL(&display, 33, 0, 32, 32, NUMLOCK_BORDER_COLOR, NUMLOCK_ON_COLOR, NUMLOCK_OFF_COLOR, "CAP", 1);

  t = millis();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer and display initial button states
  display.clearDisplay();
  numlock_state = BootKeyboard.getLeds() & LED_NUM_LOCK;
  caps_state = BootKeyboard.getLeds() & LED_CAPS_LOCK;
  numLock.drawButton(!numlock_state);
  capsLock.drawButton(!caps_state);

  display.display();

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
}

void getKeyboardLEDs() {
  if (millis() > t + 10) {
    t = millis();
    const int led_state = BootKeyboard.getLeds();
    bool new_state = led_state & LED_NUM_LOCK;
    if (new_state != numlock_state) {
      numlock_state = new_state;
      numLock.drawButton(!numlock_state);
    }
    new_state = led_state & LED_CAPS_LOCK;
    if (new_state != caps_state) {
      caps_state = new_state;
      capsLock.drawButton(!caps_state);
    }
  }
}

void loop() {
  if(!digitalRead(BUTTON_A)) display.clearDisplay();
  getKeyboardLEDs();
  delay(10);
  yield();
  display.display();
}
