# piswitch

## Cross Compiling

Ubuntu:
```
sudo apt-get install gcc-arm-linux-gnueabi make ncurses-dev
```

```
make
```

## Switch with curl

```
curl -H "Content-Type: application/json" -d '{"type":5,"p5":{"network":2720, "address": 1, "broadcast": 0, "state":0, "dimmer":0}}' http://192.168.0.180:8080/switch
```
