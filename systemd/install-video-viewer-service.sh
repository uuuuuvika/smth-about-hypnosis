#!/bin/bash
set -e

SERVICE=rpi-video-viewer
BIN=/home/pi/py/smth-about-hypnosis/video-viewer/video-viewer
UNIT=/etc/systemd/system/${SERVICE}.service
ENV=/etc/default/${SERVICE}

# 1) Create env file if missing (edit later to fit your matrix and videos)
if [[ ! -f "${ENV}" ]]; then
  sudo tee "${ENV}" >/dev/null <<'EOF'
# LED matrix options
VIDEO_VIEWER_OPTS="--led-chain=2 --led-cols=64 --led-rows=32 --led-gpio-mapping=adafruit-hat"
# Decoder threads
VIDEO_THREADS=2
# Files or directories (space-separated). Directories are expanded.
VIDEO_INPUTS="/home/pi/py/optimized-videos"
EOF
  echo "Created ${ENV}. Edit it and restart the service to apply changes."
fi

# 2) Write unit (ExecStart uses your exact binary path)
sudo tee "${UNIT}" >/dev/null <<EOF
[Unit]
Description=RGB LED Matrix video viewer
After=multi-user.target

[Service]
Type=simple
User=root
Group=root
EnvironmentFile=-${ENV}
ExecStart=/bin/bash -lc '${BIN} \${VIDEO_VIEWER_OPTS:-} -T\${VIDEO_THREADS:-1} \${VIDEO_INPUTS:-}'
Restart=always
RestartSec=2s
TimeoutStopSec=5s
StandardOutput=journal
StandardError=inherit

[Install]
WantedBy=multi-user.target
EOF

# 3) Enable + start
sudo systemctl daemon-reload
sudo systemctl enable ${SERVICE}
sudo systemctl restart ${SERVICE}

echo "Installed and started ${SERVICE}."
echo "Edit ${ENV} then run: sudo systemctl restart ${SERVICE}"