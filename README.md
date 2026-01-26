# RGB Matrix Comic Windows

A Raspberry Pi-based project for driving RGB LED matrices with custom visuals & storytelling. It sequence text and animations to tell a narrative... It's like a digital LED comic book!

> **Note:** The project name is currently a work in progress.

---
## Part 1: Development

### Prerequisites
This project depends on the [hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix) library. Both folders **must** reside in the same parent directory.

```bash
# Clone the library
git clone [https://github.com/hzeller/rpi-rgb-led-matrix.git](https://github.com/hzeller/rpi-rgb-led-matrix.git)

# Clone this project
git clone [https://github.com/your-username/smth-about-hypnosis.git](https://github.com/your-username/smth-about-hypnosis.git)

```
project/
├── rpi-rgb-led-matrix/
├── smth-about-hypnosis/
    └── README.md
```
Then, connect to the pi

```bash
ssh raspberry@raspberrypi.local
```

Git pull or rsync your local code with pi's (go to scripts/readme.md). Cool! All set up!

### Build and Run (pro is short for a project)

```bash
make pro && sudo ./pro
```

this will also build rpi-rgb-led-matrix library which is in our case located in the parent directory


## Part 2: Showcase 

### Auto start setup

```bash
cd scripts
chmod +x install.sh
./install.sh
```

### Check service logs

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
