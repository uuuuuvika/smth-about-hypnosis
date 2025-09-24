#!/bin/bash
set -e

SERVICE=rpi-video-viewer-build
UNIT=/etc/systemd/system/${SERVICE}.service
CODE_DIR=/home/pi/py/smth-about-hypnosis/video-viewer

# Write oneshot build unit
sudo tee "${UNIT}" >/dev/null <<EOF
[Unit]
Description=Build video viewer

[Service]
Type=oneshot
User=root
Group=root
ExecStart=/usr/bin/make -s -C ${CODE_DIR}
TimeoutStartSec=600
StandardOutput=journal
StandardError=inherit
EOF

# Reload systemd to pick up the new unit
sudo systemctl daemon-reload

echo "Installed ${SERVICE}. It will be triggered by the run service."
echo "To build immediately, run: sudo systemctl start ${SERVICE}"


