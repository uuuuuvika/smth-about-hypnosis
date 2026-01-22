# Optimize video assets

- Install `ffpmeg` : `brew install ffmpeg`
- Setup Directory:
    Create an `input_videos` folder and place your source files there:
    > **Note:** Ensure your videos are located at `../assets/input_videos/`

- Grant execution rights to the optimization script and run it:

1.  ## Set permissions:
    ```bash
    chmod +x conver _to_raw.sh
    ```
2.   ## Run the optimizer:
    ```bash
    ./convert_to_raw.sh
    ```

# Sync videos between Mac and Pi

```bash
rsync -azP --delete \
  -e ssh \
  ../assets/output_videos/ \
  raspberry@raspberrypi.local:/home/raspberry/screens_prjct/screens/assets/output_videos/
```

# Sync code between Mac and Pi

```bash
rsync -azP --delete \
  --filter=':- .gitignore' \
  --exclude='.git/' \
  --exclude='.DS_Store' \
  --exclude='*.mp4' \
  --exclude='*.mov' \
  --exclude='*.avi' \
  --exclude='*.mkv' \
  --exclude='*.m4v' \
  -e ssh \
  ../ \
  raspberry@raspberrypi.local:/home/raspberry/screens_prjct/screens/
```
