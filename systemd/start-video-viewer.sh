#!/bin/bash
set -euo pipefail
sudo systemctl start rpi-video-viewer
sudo systemctl status rpi-video-viewer --no-pager || true
