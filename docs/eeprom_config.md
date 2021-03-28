# EEPROM Config Layout

## Standard Locations in EEPROM
Max 512 Bytes

| Bytes   | Len | Description        |
| ------- | --- | ------------------ |
| 0-7     | 8   | Config Version (string literal: VST001) |
| 8-39    | 32  | Device Name        |
| 40-41   | 2   | Device Type        |
| 42-73   | 32  | Wifi SSID          |
| 74-105  | 32  | Wifi Password      |
| 106-137 | 32  | Server URL         |
| 138-169 | 32  | Server Username    |
| 170-201 | 32  | Server Password    |
| 254-255 | 2   | Config CRC16       |
| 256-511 | 256 | Device Type Config |

## Device Types
### 1 - MStickC Gateway
