# Hardware and pin map

최종 `plc_parking_controller.ino`에 정의된 Arduino 핀입니다.

| Arduino pin | Symbol | Connected component |
| ---: | --- | --- |
| D13 | `TRIG_PIN` | 초음파 센서 TRIG |
| D12 | `ECHO_PIN` | 초음파 센서 ECHO |
| D9 | `RED_LED_PIN` | 빨간 LED |
| D11 | `GREEN_LED_PIN` | 초록 LED |
| D8 | `PIEZO_PIN` | 피에조 부저 |
| D2 | `PIR1_PIN` | 첫 번째 적외선 센서 |
| D4 | `PIR2_PIN` | 두 번째 적외선 센서 |
| D3 | `SERVO1_PIN` | 첫 번째 차단기 서보 |
| D5 | `SERVO2_PIN` | 두 번째 차단기 서보 |
| D6 | `RELAY_PIN` | PLC 전원 제어 릴레이 |
| D7 | `X10_PIN` | PLC 첫 번째 제어 신호 |
| D10 | `X11_PIN` | PLC 두 번째 제어 신호 |

## Final sketch behavior

- 초음파 측정값이 70cm 이하이면 빨간 LED와 1000Hz 부저가 켜집니다.
- 70cm를 초과하면 초록 LED가 켜지고 부저가 꺼집니다.
- 첫 번째 적외선 센서가 차량을 감지하면 첫 번째 서보가 90도로 이동하고 릴레이와 X10 신호가 활성화됩니다.
- 두 번째 적외선 센서가 차량을 감지하면 두 번째 서보가 90도로 이동하고 X11 신호가 활성화됩니다.
- 감지가 끝난 뒤 3초가 지나면 해당 서보가 0도로 복귀합니다.
