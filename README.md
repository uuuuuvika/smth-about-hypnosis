## Usage

1. connect to the pi

```bash
ssh pi@raspberrypi.local
```

2. rsync or sync your local code with pi's

### Build and Run

```bash
make pro && sudo ./pro
```

this will also build rpi-rgb-led-matrix library which is in our case located in the parent directory

## Auto start setup

```bash
chmod +x install.sh
./install.sh
```

Check service logs;

```bash
journalctl -u smth-about-hypnosis -f
```

### Start service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl start smth-about-hypnosis
sudo systemctl status smth-about-hypnosis --no-pager || true
```

### Stop service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl stop smth-about-hypnosis
sudo systemctl status smth-about-hypnosis --no-pager || true
```

### Disable service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl stop smth-about-hypnosis
sudo systemctl disable smth-about-hypnosis
```
