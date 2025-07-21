# Serial messages

PING_ACK;

## Ping
`PING;ID=12` - ping 
`PING_ACK;ID=1,DELAY=544,RSSI=-12.00,SNR=12.75б,TOA=25,BPS=400` - response to ping
`PING_NO_ACK;ID=1` - no response

## Send

`SEND;ID=12,DATA="Test Data Hello"`

`SEND;ID=12,DATA="Sending long message to check how lora works. Even larger than you thought. Maybe one day you will receive this message, but this is not guarantee. Transmission over LoRa is slow, but stable for long distances. However, bandwidth is limited, so optimization is key when designing communication protocols."`

## Config

`FQ` - Frequency `868.0`
`BW` - Bandwidth `7.8`, `10.4`, `15.6`, `20.8`, `31.25`, `41.7`, `62.5`, `125`, `250`, `500`
`CR` - Codding rate `5` - `8`
`SF` - Spreading factor `6` - `12`
`TP` - Transmit power `2` - `20`
`IH` - Implicit Header `0` or `1`
`HS` - Header size in bytes
`SW` - Sync Word `171`
`PL` - Preamble length `6` to `65535`

### Update config 

`CONFIG;FQ=867.0`
`CONFIG;BW=125`
`CONFIG;IH=1`
`CONFIG;IH=0,HS=20,SF=7`
`CONFIG;FQ=868,BW=125,SF=7,CR=6,TP=10,IH=0,HS=20,SW=171` - default config

### Config Sync

`CONFIG_SYNC;BW=500`
`CONFIG_SYNC;IH=0,SF=8`

`CONFIG_SYNC_ACK`
`CONFIG_SYNC_NO_ACK`

`CONFIG_SYNC_CHECK`
`CONFIG_SYNC_CHECK_ACK`
`CONFIG_SYNC_CHECK_NO_ACK`

### Config Read
`CONFIG_GET`


## BUG

## Ideas
- Replace String?
- Add build version
- Retries?
- Non brodcast
- Security
- OTA?