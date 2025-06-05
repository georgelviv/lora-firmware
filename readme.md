## Serial messages

### Ping
`PING` - ping 
`PING_ACK;DELAY=500,RSSI=1` - response to ping


### Config
`CONFIG;FQ=868.0`
`CONFIG;BW=125`
`CONFIG;FQ=867,BW=125` - config

`FQ` - Frequency
`BW` - Bandwidth

### Config Sync

`CONFIG_SYNC;FQ=868.0`
`CONFIG_SYNC_ACK`

`CONFIG_SYNC_CHECK`
`CONFIG_SYNC_CHECK_ACK`


## Ideas
- Replace String?
- Add build version