## Usage

1. connect to the pi

```bash
ssh pi@raspberrypi.local
```

2. rsync or sync your local code with pi's

### Build and Run

```bash
make all && sudo ./pro
```

or

```bash
make pro && sudo ./pro
```

or

```bash
make clean && make all && sudo ./pro pro
```

## Auto start setup

```bash
chmod +x install.sh
./install.sh
```

Check service logs;

```bash
journalctl -u smth-about-hypnosis -f
```

### Stop service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl stop smth-about-hypnosis
sudo systemctl status smth-about-hypnosis --no-pager || true
```

### Start service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl start smth-about-hypnosis
sudo systemctl status smth-about-hypnosis --no-pager || true
```
