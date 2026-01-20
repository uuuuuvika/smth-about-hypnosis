# Optimize video assets

- Install `ffpmeg` : `brew install ffmpeg`
- Set script permissions: `chmod +x convert_to_raw.sh`
- Then, run asset optimizer script

```bash
./convert_to_raw.sh
```

## Sync videos between Mac and Pi

```bash
rsync -azP --delete \
  -e ssh \
  /Users/userfriendly/code/smth-about-hypnosis/assets/output_videos/ \
  raspberry@raspberrypi.local:/home/raspberry/screens_prjct/screens/assets/output_videos/
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
