# Serial messages

## Ping
`PING` - ping 
`PING_ACK;DELAY=500,RSSI=1` - response to ping

## Config

`FQ` - Frequency `868.0`
`BW` - Bandwidth `7.8`, `10.4`, `15.6`, `20.8`, `31.25`, `41.7`, `62.5`, `125`, `250`, `500`
`CF` - Codding rate `6`
`SF` - Spreading factor `6` - `12`
`TP` - Transmit power `2` - `20`

### Update config 

`CONFIG;FQ=868.0`
`CONFIG;BW=125`
`CONFIG;SF=8`
`CONFIG;FQ=867,BW=125` - config

### Config Sync

`CONFIG_SYNC;FQ=868.0`
`CONFIG_SYNC_ACK`

`CONFIG_SYNC_CHECK`
`CONFIG_SYNC_CHECK_ACK`

### Config Read
`CONFIG_GET`

## Ideas
- Replace String?
- Add build version
- OTA?