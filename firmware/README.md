# Firmware variants

| Sketch | Source file | Purpose |
| --- | --- | --- |
| `plc_parking_controller` | `plc작동하는거.txt` | 실제 PLC 연동 최종본 |
| `parking_counter_demo` | `카운트 되는 시뮬용.txt` | 3대 주차 카운트 시뮬레이션 |
| `plc_signal_test` | `plc 연결.txt` | 릴레이, X10, X11 연결 시험 |
| `arduino_hardware_test` | `아두이노 돌리기용.txt` | 센서와 서보 하드웨어 시험 |
| `simulation` | `시뮬용.txt` | 초기 시뮬레이션 |

Arduino IDE는 스케치 폴더와 `.ino` 파일명이 같아야 하므로 각 버전을 별도 폴더로 구성했습니다.

`simulation.ino`는 원본 메모장 제목을 주석으로 바꾸고 마지막의 중복 중괄호 하나를 제거했습니다. 원본 내용은 첫 번째 커밋 `90fa719`에서 확인할 수 있습니다.
