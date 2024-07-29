
#include <Pixy2.h>
#include <Servo.h>
// Include FastLED library by Daniel Garcia
#include <FastLED.h>
#include "pitches.h"

int melody[] = {
  NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6,
  NOTE_C6, NOTE_C6, NOTE_G5, NOTE_E5,
  NOTE_C5
};

int noteDurations[] = {
  8, 8, 8, 4,
  4, 4, 8, 8,
  2
};

int buzzerPin = 8;

// Common naming from FastLED demos
#define DATA_PIN 10
#define NUM_LEDS 24

int handle_bar_status = 0;
int handle_else = 1;




// Array of led data
CRGB leds[NUM_LEDS];
CRGBPalette256 my_gradient = CRGBPalette256(
  CRGB::Green,
  CRGB::Blue,
  CRGB::Red);


Servo motor_x;
Servo motor_y;
Servo motor_mouth;
// This is the main Pixy object
Pixy2 pixy;
const int motor_x_pin = 3;      // horizontal
const int motor_y_pin = 5;      // vertical
const int servo_mouth_pin = 9;  // mouth servo


void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);
  FastLED.clear();




  pixy.init();
  pixy.setLamp(255, 255);
  Serial.println("pixy light");
  motor_x.attach(motor_x_pin);
  motor_y.attach(motor_y_pin);
  motor_mouth.attach(servo_mouth_pin);
  motor_x.write(90);
  motor_y.write(90);
  motor_mouth.write(0);
  pixy.setLamp(255, 255);
  pinMode(buzzerPin, OUTPUT);
}

uint8_t pos = 0;

void loop() {
  int i;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i = 0; i < pixy.ccc.numBlocks; i++) {
      int horizontal_deg = map(pixy.ccc.blocks[i].m_x, 0, 316, 20, 160);  // https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
      int vertical_deg = map(pixy.ccc.blocks[i].m_y, 0, 208, 140, 40);
      motor_x.write(horizontal_deg);
      motor_y.write(vertical_deg);
      int area_object = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
      Serial.println(area_object);
      int servo_mouth_value = map(area_object, 3000, 12000, 0, 80);
      motor_mouth.write(servo_mouth_value);

      int map_LED_loop = map(area_object, 4000, 10000, 0, 14);
      if (map_LED_loop < 14) {
        for (int i = 0; i < map_LED_loop; i++) {
          leds[0 + i] = CRGB::White;
          leds[23 - i] = CRGB::White;
        }
        FastLED.show();



        
      }

      else {
        delay(500);
        for (int i = 0; i < 20; i++) {
          delay(10);
          {
            for (uint8_t i = 0; i < NUM_LEDS; i++) {
              // Calculate position with map() then use pos to cycle it around the ring a bit
              uint8_t this_position = map(i, 0, NUM_LEDS - 1, 0, 255) + pos;
              leds[i] = ColorFromPalette(my_gradient, this_position);
              FastLED.show();
              pos++;
            }
          }
        }
        delay(1000);
        for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
          int noteDuration = 1000 / noteDurations[thisNote];
          tone(buzzerPin, melody[thisNote], noteDuration);

          int pauseBetweenNotes = noteDuration * 1.30;
          delay(pauseBetweenNotes);

          noTone(buzzerPin);
        }
        delay(500);
        FastLED.clear();
        delay(500);
        FastLED.clear();
        motor_mouth.write(0);
      }
    }
  } 
  
  else {
    FastLED.clear();
  }
}