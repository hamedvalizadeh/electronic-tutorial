

#### Master Transmitter Mode

| Hex Code | Status Description                             |
| -------- | ---------------------------------------------- |
| `0x08`   | START condition transmitted                    |
| `0x10`   | Repeated START condition transmitted           |
| `0x18`   | SLA+W transmitted, ACK received                |
| `0x20`   | SLA+W transmitted, NACK received               |
| `0x28`   | Data byte transmitted, ACK received            |
| `0x30`   | Data byte transmitted, NACK received           |
| `0x38`   | Arbitration lost in SLA+W or data transmission |



#### Master Receiver Mode

| Hex Code | Status Description                |
| -------- | --------------------------------- |
| `0x40`   | SLA+R transmitted, ACK received   |
| `0x48`   | SLA+R transmitted, NACK received  |
| `0x50`   | Data byte received, ACK returned  |
| `0x58`   | Data byte received, NACK returned |



#### Slave Receiver Mode

| Hex Code | Status Description                              |
| -------- | ----------------------------------------------- |
| `0x60`   | Own SLA+W received, ACK returned                |
| `0x68`   | Arbitration lost; own SLA+W received, ACK       |
| `0x70`   | General call address received, ACK returned     |
| `0x78`   | Arbitration lost; general call received, ACK    |
| `0x80`   | Data received (SLA+W), ACK returned             |
| `0x88`   | Data received (SLA+W), NACK returned            |
| `0x90`   | Data received (General Call), ACK returned      |
| `0x98`   | Data received (General Call), NACK returned     |
| `0xA0`   | STOP or repeated START received while addressed |



#### Slave Transmitter Mode

| Hex Code | Status Description                             |
| -------- | ---------------------------------------------- |
| `0xA8`   | Own SLA+R received, ACK returned               |
| `0xB0`   | Arbitration lost, SLA+R received, ACK returned |
| `0xB8`   | Data byte transmitted, ACK received            |
| `0xC0`   | Data byte transmitted, NACK received           |
| `0xC8`   | Last data byte transmitted, ACK received       |



#### Miscellaneous Conditions

| Hex Code | Status Description                       |
| -------- | ---------------------------------------- |
| `0x00`   | Bus error due to illegal START/STOP      |
| `0xF8`   | No relevant state information; TWINT = 0 |