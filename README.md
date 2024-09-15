# POC - esp32 mqtt pubsub

Turns the led on and off by pub/sub events.

## Requirements

```bash
sudo apt install mosquitto-clients
```

### Set WIFI Credentials

- `wifi_ssid`
- `wifi_password`

## ON
```bash
mosquitto_pub -h broker.hivemq.com -t test-esp32/led -m "on"
```

## OFF
```bash
mosquitto_pub -h broker.hivemq.com -t test-esp32/led -m "off"
```

## Subscribe test
```bash
mosquitto_sub -h broker.hivemq.com -t test-esp32/led
```

