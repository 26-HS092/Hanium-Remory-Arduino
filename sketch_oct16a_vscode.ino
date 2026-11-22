#include <Servo.h>
#include <stdio.h>

Servo panServo;
Servo tiltServo;

// 측정한 중앙값과 안전 범위
const int PAN_CENTER = 86;
const int PAN_MIN = 66;   // 왼쪽 한계
const int PAN_MAX = 116;  // 오른쪽 한계

const int TILT_CENTER = 72;
const int TILT_MIN = 42;  // 위쪽 한계
const int TILT_MAX = 72;  // 아래쪽 한계

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);

  panServo.write(PAN_CENTER);
  tiltServo.write(TILT_CENTER);

  panServo.attach(9);
  tiltServo.attach(10);
}

void loop() {
  if (Serial.available() == 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');
  command.trim();

  if (command.length() == 0) {
    return;
  }

  int pan;
  int tilt;
  char extra;

  // "86,72" 형식 확인
  if (sscanf(
        command.c_str(),
        "%d,%d%c",
        &pan,
        &tilt,
        &extra
      ) != 2) {
    Serial.println("ERROR: FORMAT");
    return;
  }

  // 안전 범위 밖의 명령은 실행하지 않음
  if (pan < PAN_MIN || pan > PAN_MAX ||
      tilt < TILT_MIN || tilt > TILT_MAX) {
    Serial.println("ERROR: RANGE");
    return;
  }

  panServo.write(pan);
  tiltServo.write(tilt);

  Serial.print("RECEIVED: ");
  Serial.print(pan);
  Serial.print(",");
  Serial.println(tilt);
}
