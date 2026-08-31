#include <Servo.h>

// 핀 정의
#define TRIG_PIN 13                 // 초음파 센서 트리거 핀
#define ECHO_PIN 12                 // 초음파 센서 에코 핀
#define RED_LED_PIN 9               // 빨간 LED
#define GREEN_LED_PIN 11            // 초록 LED
#define PIEZO_PIN 8                 // 피에조 부저
#define PIR1_PIN 2                  // 첫 번째 적외선 센서 핀
#define PIR2_PIN 4                  // 두 번째 적외선 센서 핀
#define SERVO1_PIN 3                // 첫 번째 서보 모터 핀
#define SERVO2_PIN 5                // 두 번째 서보 모터 핀

Servo servo1;
Servo servo2;
unsigned long pir1LastDetected = 0; // 첫 번째 적외선 센서 감지 시간
unsigned long pir2LastDetected = 0; // 두 번째 적외선 센서 감지 시간
bool pir1Active = false; // 첫 번째 적외선 센서 활성 상태
bool pir2Active = false; // 두 번째 적외선 센서 활성 상태
int servo1Counter = 3;   // 첫 번째 서보 모터 작동 카운트

void setup() {
  // 핀 모드 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);

  // 서보 모터 초기화
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // 초기 상태 설정
  servo1.write(0); // 첫 번째 서보 모터 차단 상태( 시뮬레이션 전용 초기화 로직)
  servo2.write(0); // 두 번째 서보 모터 차단 상태( 시뮬레이션 전용 초기화 로직)

  digitalWrite(GREEN_LED_PIN, HIGH);  // 초록 LED 켜짐
  digitalWrite(RED_LED_PIN, LOW);     // 빨간 LED 꺼짐
  digitalWrite(PIEZO_PIN, LOW);       // 피에조 부저 꺼짐
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
  if (distance <= 70) {
    digitalWrite(RED_LED_PIN, HIGH);    // 빨간 LED 켜짐
    digitalWrite(GREEN_LED_PIN, LOW);   // 초록 LED 꺼짐
    tone(PIEZO_PIN, 1000);              // 피에조 부저 1000Hz로 울림
  } else {
    digitalWrite(RED_LED_PIN, LOW);     // 빨간 LED 꺼짐
    digitalWrite(GREEN_LED_PIN, HIGH);  // 초록 LED 켜짐
    noTone(PIEZO_PIN);                  // 피에조 부저 꺼짐
  }

  // 적외선 센서 기반 서보 모터 제어 (물체 인식 시 즉시 회전하고, 사라지면 3초 후 원래대로 복귀)
  if (digitalRead(PIR1_PIN) == HIGH && servo1Counter > 0) {
    if (!pir1Active) {
      servo1.write(90);  // 첫 번째 서보 모터 90도 회전
      pir1Active = true;
      servo1Counter--;   // 서보 모터 작동 카운트 감소
    }
    pir1LastDetected = millis(); // 마지막 감지 시간 업데이트
  } else if (pir1Active && millis() - pir1LastDetected > 3000) {
    servo1.write(0);   // 첫 번째 서보 모터 0도 회전 (물체 사라지고 3초 후)
    pir1Active = false;
  }

  if (digitalRead(PIR2_PIN) == HIGH) {
    if (!pir2Active) {
      servo2.write(90);  // 두 번째 서보 모터 90도 회전
      pir2Active = true;
      if (servo1Counter < 3) {
        servo1Counter++; // 두 번째 서보 모터 작동 시 첫 번째 모터 카운트 증가
      }
    }
    pir2LastDetected = millis(); // 마지막 감지 시간 업데이트
  } else if (pir2Active && millis() - pir2LastDetected > 3000) {
    servo2.write(0);   // 두 번째 서보 모터 0도 회전 (물체 사라지고 3초 후)
    pir2Active = false;
  }

  delay(100);  // 100ms 대기
}
