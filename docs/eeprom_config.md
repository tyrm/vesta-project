# EEPROM Config Layout

## Standard Locations in EEPROM
Max 512 Bytes

| Bytes   | Len | Description   |
| ------- | --- | ------------- |
| 0-7     | 8   | Config Version (string literal: VST001) |
| 8-39    | 32  | Device Name   |
| 40-71   | 32  | Wifi SSID     |
| 72-103  | 32  | Wifi Password |
| 104-135 | 32  | Server URL    |
| 136-137 | 2   | Device Type   |
| 254-255 | 2   | Config CRC16  |
| 256-511 | 256 | Device Type Config |

## Device Types
### 1 - MStickC Gateway
