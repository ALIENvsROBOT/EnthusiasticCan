
#include <Pixy2.h>
#include <Servo.h>

Servo motor_x;
Servo motor_y;
// This is the main Pixy object
Pixy2 pixy;
const int motor_x_pin = 3;  // horizontal
const int motor_y_pin = 5;  // vertical


void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();
  motor_x.attach(motor_x_pin);
  motor_y.attach(motor_y_pin);
  motor_x.write(90);
  motor_y.write(90);
  pixy.setLamp(255, 255);
}

void loop() {
  int i;
  // grab blocks!
  pixy.ccc.getBlocks();

  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks) {
    Serial.println(pixy.ccc.numBlocks);

    for (i = 0; i < pixy.ccc.numBlocks; i++) {
      int horizontal_deg = map(pixy.ccc.blocks[i].m_x, 0, 316, 20, 160);  // https://docs.pixycam.com/wiki/doku.php?id=wiki:v2:hooking_up_pixy_to_a_microcontroller_-28like_an_arduino-29
      int vertical_deg = map(pixy.ccc.blocks[i].m_y, 0, 208, 140, 40);
      int area_object = pixy.ccc.blocks[i].m_width * pixy.ccc.blocks[i].m_height;
      motor_x.write(horizontal_deg);
      motor_y.write(vertical_deg);
    }
  }
}
