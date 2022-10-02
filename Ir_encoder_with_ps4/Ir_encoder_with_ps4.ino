/*
 Example sketch for the PS4 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS4USB.h>

#define m1_dir 41
#define m2_dir 43
#define m1_pwm 5
#define m2_pwm 6

int M1_PWM = 150;
int M2_PWM = 150;

#define ir 2

int led_ctr = 0;
int irStatus = 0;
long long current_millis = 0;

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
PS4USB PS4(&Usb);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
#if !defined(_MIPSEL_)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  pinMode(ir,INPUT);
  pinMode(m1_dir, OUTPUT);
  pinMode(m1_pwm, OUTPUT);
  pinMode(m2_dir, OUTPUT);
  pinMode(m2_pwm, OUTPUT);
  digitalWrite(m1_dir, HIGH);
  digitalWrite(m2_dir, LOW);
  Serial.print(F("\r\nPS4 USB Library Started"));
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    int interval = 1000;
    
    if (millis() - current_millis > interval){
        Serial.println(led_ctr);
        led_ctr = 0;
        current_millis = millis();
    }
    
    if (PS4.getButtonClick(CIRCLE)) {
      analogWrite(m1_pwm, M1_PWM);
      analogWrite(m2_pwm, M2_PWM);
      Serial.println("Start");
    }
    
    if (PS4.getButtonClick(CROSS)) {
      analogWrite(m1_pwm, 0);
      analogWrite(m2_pwm, 0);
      Serial.println("Stop");
    }

    if (PS4.getButtonClick(UP)) {
      M1_PWM+=20;
      M2_PWM+=20;
      Serial.print("PWM - ");
      Serial.println(M1_PWM);
    }
    
    if (PS4.getButtonClick(DOWN)) {
      M1_PWM-=20;
      M2_PWM-=20;
      Serial.print("PWM - ");
      Serial.println(M1_PWM);
    }
    
    if (digitalRead(ir) == 1 && irStatus == 0){
      led_ctr++;
//      Serial.println(led_ctr);
    }

//    if (PS4.getButtonClick(TRIANGLE)) {
//      led_ctr=0;
//      Serial.println(led_ctr);
//    }
    
    irStatus = digitalRead(ir);
  }
}
