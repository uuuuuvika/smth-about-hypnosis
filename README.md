# RGB Matrix Comic Windows

A Raspberry Pi-based project for driving RGB LED matrices with custom visuals & storytelling. It sequence text and animations to tell a narrative... It's like a digital LED comic book!

(the project name is currently a work in progress...)

<img width="2016" height="1008" alt="screens" src="https://github.com/user-attachments/assets/f77d65ed-3c7e-4749-bbfa-b8eccfb43ed2" />



## Part 1: Development

This project depends on the [hzeller/rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix) library. Both folders **must** reside in the same parent directory.

```bash
# Clone the library
git clone [https://github.com/hzeller/rpi-rgb-led-matrix.git](https://github.com/hzeller/rpi-rgb-led-matrix.git)

# Clone this project
git clone [https://github.com/your-username/smth-about-hypnosis.git](https://github.com/your-username/smth-about-hypnosis.git)

```
Directory Structure:
```
project/
├── rpi-rgb-led-matrix/
├── smth-about-hypnosis/
    └── README.md
```

### Connect to your Raspberry Pi via SSH
```bash
ssh raspberry@raspberrypi.local
```
To rsync your local code with the Pi, refer to the `scripts/readme.md` 


### Build and Run
This will also build `rpi-rgb-led-matrix` library

```bash
make pro && sudo ./pro
```

## Part 2: Showcase 

### Auto start setup
Run the installation script to set up the project as a systemd service:

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
