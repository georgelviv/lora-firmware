# Serial messages

PING_ACK;

## Ping
`PING;ID=12` - ping 
`PING_ACK;ID=1,DELAY=544,RSSI=-12.00,SNR=12.75` - response to ping
`PING_NO_ACK;ID=1` - no response

## Config

`FQ` - Frequency `868.0`
`BW` - Bandwidth `7.8`, `10.4`, `15.6`, `20.8`, `31.25`, `41.7`, `62.5`, `125`, `250`, `500`
`CF` - Codding rate `6`
`SF` - Spreading factor `6` - `12`
`TP` - Transmit power `2` - `20`
`IH` - Implicit Header `0` or `1`
`HS` - Header size in bytes

### Update config 

`CONFIG;FQ=867.0`
`CONFIG;BW=125`
`CONFIG;SF=7`
`CONFIG;IH=0,HS=20,SF=7`
`CONFIG;FQ=867,BW=125` - config

### Config Sync

`CONFIG_SYNC;SF=6`
`CONFIG_SYNC;SF=6`

`CONFIG_SYNC_ACK`
`CONFIG_SYNC_NO_ACK`

`CONFIG_SYNC_CHECK`
`CONFIG_SYNC_CHECK_ACK`
`CONFIG_SYNC_CHECK_NO_ACK`

### Config Read
`CONFIG_GET`

## BUG
Update on 1 module CONFIG;SF=7
on other send ping

## Ideas
- Replace String?
- Add build version
- Retries?
- Non brodcast
- Security
- OTA?