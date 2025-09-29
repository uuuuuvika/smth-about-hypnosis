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

## Utils

### Start service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl start rpi-video-viewer
sudo systemctl status rpi-video-viewer --no-pager || true
```

### Stop service

```bash
#!/bin/bash
set -euo pipefail
sudo systemctl stop rpi-video-viewer
sudo systemctl status rpi-video-viewer --no-pager || true
```
