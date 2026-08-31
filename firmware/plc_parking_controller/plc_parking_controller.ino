#include <Servo.h>

// 핀 정의
#define TRIG_PIN 13                 // 초음파 센서 trig
#define ECHO_PIN 12                 // 초음파 센서 echo
#define RED_LED_PIN 9               // 빨간 LED
#define GREEN_LED_PIN 11            // 초록 LED
#define PIEZO_PIN 8                 // 부저
#define PIR1_PIN 2                  // 첫 번째 적외선 센서
#define PIR2_PIN 4                  // 두 번째 적외선 센서
#define SERVO1_PIN 3                // 첫 번째 서보 모터
#define SERVO2_PIN 5                // 두 번째 서보 모터
#define RELAY_PIN 6                 // PLC 전원을 제어할 릴레이 ( srd 05vdc sl c )
#define X10_PIN 7                   // 첫 번째 서보모터 제어 신호 (X10)
#define X11_PIN 10                   // 두 번째 서보모터 제어 신호 (X11)

Servo servo1;
Servo servo2;
unsigned long pir1LastDetected = 0; // 첫 번째 적외선 센서 감지 시간
unsigned long pir2LastDetected = 0; // 두 번째 적외선 센서 감지 시간
bool pir1Active = false; // 첫 번째 적외선 센서 활성 상태 (초기 비활성)
bool pir2Active = false; // 두 번째 적외선 센서 활성 상태 (초기 비활성)
bool relayOn = false;    // PLC 전원 상태 (초기 비활성)

void setup() {
  // 핀 모드 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);  // 릴레이 핀
  pinMode(X10_PIN, OUTPUT);    // X10 출력
  pinMode(X11_PIN, OUTPUT);    // X11 출력

  // 서보 모터 초기화
  servo1.attach(SERVO1_PIN); // 상단 모터
  servo2.attach(SERVO2_PIN); // 하단 모터

  // 초기 상태 설정
  digitalWrite(GREEN_LED_PIN, HIGH);  // 초록 LED 켜짐
  digitalWrite(RED_LED_PIN, LOW);     // 빨간 LED 꺼짐
  digitalWrite(PIEZO_PIN, LOW);       // 부저 꺼짐
  digitalWrite(RELAY_PIN, LOW);       // PLC 전원 꺼짐 (릴레이 비활성화)
  digitalWrite(X10_PIN, LOW);         // X10 초기 상태
  digitalWrite(X11_PIN, LOW);         // X11 초기 상태
}

long getDistance() {
  // 초음파 센서를 이용해 거리 측정
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;

  return distance;
}

void loop() {
  long distance = getDistance();

  // 거리 기반 LED 및 피에조 제어
  if (distance <= 70) {                      // 70cm 이하의 거리에서 작동
    digitalWrite(RED_LED_PIN, HIGH);    // 빨간 LED 켜짐
    digitalWrite(GREEN_LED_PIN, LOW);   // 초록 LED 꺼짐
    tone(PIEZO_PIN, 1000);              //  부저 켜짐 ( 1000Hz로 작동 )
  } else {
    digitalWrite(RED_LED_PIN, LOW);     // 빨간 LED 꺼짐
    digitalWrite(GREEN_LED_PIN, HIGH);  // 초록 LED 켜짐
    noTone(PIEZO_PIN);                  // 부저 꺼짐
  }

  // IR센서 기반 서보 모터 제어 (차량 인식 시 즉시 회전, 3초 후 원래대로 복귀)
  if (digitalRead(PIR1_PIN) == LOW) {
    if (!pir1Active) {
      servo1.write(90);  // 첫 번째 서보 모터 90도 회전
      pir1Active = true;
      if (!relayOn) {
        digitalWrite(RELAY_PIN, HIGH);  // PLC 전원 켜짐 (릴레이 활성화)
        relayOn = true;  // 릴레이가 켜진 상태로 유지
      }
      digitalWrite(X10_PIN, HIGH);  // X10 신호 활성화 (첫 번째 서보 모터 회전 시)
    }
    pir1LastDetected = millis(); // 마지막 감지 시간 업데이트
  } else if (pir1Active && millis() - pir1LastDetected > 3000) {
    servo1.write(0);   // 첫 번째 서보 모터 0도 회전 (물체 사라지고 3초 후)
    pir1Active = false;
    // X10 신호 유지 (첫 번째 서보 모터 원위치 시)
  }

  if (digitalRead(PIR2_PIN) == LOW) {
    if (!pir2Active) {
      servo2.write(90);  // 두 번째 서보 모터 90도 회전
      pir2Active = true;
      digitalWrite(X11_PIN, HIGH);  // X11 신호 활성화 (두 번째 서보 모터 회전 시)
    }
    pir2LastDetected = millis(); // 마지막 감지 시간 업데이트
  } else if (pir2Active && millis() - pir2LastDetected > 3000) {
    servo2.write(0);   // 두 번째 서보 모터 0도 회전 (물체 사라지고 3초 후)
    pir2Active = false;
    // X11 신호 유지 (두 번째 서보 모터 원위치 시)
  }

  delay(100);  // 100ms 대기 (실행속도 조절)
}
