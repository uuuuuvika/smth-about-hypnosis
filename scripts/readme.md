## Custom video viewer

Install dependencies

```bash
sudo apt-get update
sudo apt-get install pkg-config libavcodec-dev libavformat-dev libswscale-dev libavdevice-dev
```

### Run video viewer

Run the `video-viewer` utility. Based on [this](https://github.com/hzeller/rpi-rgb-led-matrix/tree/master/utils#video-viewer).

```bash
sudo ./video-viewer --led-chain=2 --led-cols=64 --led-rows=32 --led-gpio-mapping=adafruit-hat -T2 /home/pi/py/optimized-videos
```

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

## Sync code between Mac and Pi

```bash
rsync -azP --delete \
  --filter=':- .gitignore' \
  --exclude='.git/' --exclude='.DS_Store' \
  --exclude='.mp4/' --exclude='.env/' \
  --exclude='.gif/' \
  -e ssh \
  /Users/userfriendly/code/smth-about-hypnosis/ \
  pi@raspberrypi.local:/home/pi/py/smth-about-hypnosis/
```

## Sync videos between Mac and Pi

```bash
rsync -azP --delete \
  -e ssh \
  /Users/userfriendly/code/smth-about-hypnosis/assets/output_videos/ \
  raspberry@raspberrypi.local:/home/raspberry/screens_prjct/screens/assets/output_videos/
```