# Lora Firmware

Very simple lora firmware. Developed for SX1276 bases on esp32

## Formats

`{COMMAND};` - send command
`{COMMAND};PARAM=VAL,PARAM2=VAL2` - send command with params

`{COMMAND}_NO_ACK;ID=1` - no ack
`{COMMAND}_ACK;ID=1` - ack


## Ping
`PING;ID=12` - ping or 
`PING`

## Flow

[SENDER]    PING     ->  [RECEIVER]
[SENDER] <- PING_ACK     [RECEIVER]

### Ping response
`PING_ACK;ID=1,DELAY=544,RSSI=-12.00,SNR=12.75б,TOA=25,RTOA=2033,BPS=11,CHC=1,ATT=1,ETX=134.18` - response to ping

## Send

`SEND;ID=12,DATA="Test Data Hello"`

`SEND;ID=12,DATA="Sending long message to check how lora works. Even larger than you thought. Maybe one day you will receive this message, but this is not guarantee. Transmission over LoRa is slow, but stable for long distances. However, bandwidth is limited, so optimization is key when designing communication protocols. Sending long message to check how lora works. Even larger than you thought. Maybe one day you will receive this message, but this is not guarantee. Transmission over LoRa is slow, but stable for long distances. However, bandwidth is limited, so optimization is key when designing communication protocols. Sending long message to check how lora works. Even larger than you thought. Maybe one day you will receive this message, but this is not guarantee. Transmission over LoRa is slow, but stable for long distances. However, bandwidth is limited, so optimization is key when designing communication protocols. Sending long message to check how lora works. Even larger than you thought. Maybe one day you will receive this message, but this is not guarantee. Transmission over LoRa is slow, but stable for long distances. However, bandwidth is limited, so optimization is key when designing communication protocols."`

## Config

`FQ` - Frequency `868.0` mhz
`BW` - Bandwidth `7.8`, `10.4`, `15.6`, `20.8`, `31.25`, `41.7`, `62.5`, `125`, `250`, `500` khz
`CR` - Codding rate `5` - `8`
`CL` - Current limit (OCP over current protection) `45` - `240` mA with step `5` (`45` - `120`) or `10` (`120` - `240`)
`SF` - Spreading factor `6` - `12`
`TP` - Transmit power `2` - `20` (`18` and `19` not allowed) dBm
`IH` - Implicit Header `0` or `1`
`HS` - Header size in bytes
`SW` - Sync Word `171`
`PL` - Preamble length `6` to `65535`. (It's not recommend to have > `100`)
`RT` - Retry `0` to `10`.  `0` means no retries
 
### Update config 

`CONFIG;FQ=869.0`
`CONFIG;BW=125`
`CONFIG;TP=20`
`CONFIG;PL=10`
`CONFIG;CR=6`
`CONFIG;SF=9`
`CONFIG;CL=45`
`CONFIG;RT=3`

### Config Sync

`CONFIG_SYNC;ID=12,FQ=869`
`CONFIG_SYNC;ID=12,IH=0,SF=8`

#### Flow

                [SENDER]    CONFIG_SYNC           ->  [RECEIVER]
                [SENDER] <- CONFIG_SYNC_ACK           [RECEIVER] Update Settings
Update Settings [SENDER]                              [RECEIVER]
                [SENDER]    CONFIG_SYNC_CHECK     ->  [RECEIVER]
                [SENDER] <- CONFIG_SYNC_CHECK_ACK     [RECEIVER]


### Config Read
`CONFIG_GET`

### Config Reset
`CONFIG_RESET`

### Config SYNC Reset
`CONFIG_RESET_SYNC`


## Status

`ID` - Message ID  
`DELAY` - Time difference or latency in ms  
`RSSI` - Received Signal Strength Indicator  
`SNR` - Signal-to-Noise Ratio  
`TOA` - Time on Air in ms (estimated by current configuration, details in datasheet 4.1.1.7. Time on air)
`RTOA` - Real time on air
`BPS` - Bytes per second (throughput)  
`CHC` - Number of chunks transmitted  
`ATT` - Attempt number on which the message was successfully sent (1 = first try, 2 = second try, etc.)
`ETX` - Energy per Transmission in mJ




