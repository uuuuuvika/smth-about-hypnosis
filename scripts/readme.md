# How to run

## Optimize video assets

- Install `ffpmeg` : `brew install ffmpeg`
- Set script permissions: `chmod +x optimize_videos.sh`
- Then, run asset optimizer script

```bash
./optimize_videos.sh
```

### Send video files to the Pi

```bash
rsync -avz /Users/userfriendly/Dropbox/Proyectos/vika-bday-screen/optimized-videos pi@raspberrypi.local:/home/pi/py
```

## Render videos on the screen

Install dependencies and build

```bash
sudo apt-get update
sudo apt-get install pkg-config libavcodec-dev libavformat-dev libswscale-dev libavdevice-dev
make video-viewer
```

Run the `video-viewer` utility. Based on [this](https://github.com/hzeller/rpi-rgb-led-matrix/tree/master/utils#video-viewer).

```bash
sudo ./video-viewer --led-chain=2 --led-cols=64 --led-rows=32 --led-gpio-mapping=adafruit-hat -T2 /home/pi/py/optimized-videos
```