#!/bin/bash
set -euo pipefail
sudo systemctl stop rpi-video-viewer
sudo systemctl status rpi-video-viewer --no-pager || true
