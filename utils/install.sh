#!/bin/bash
set -euo pipefail

# Service name
SERVICE=smth-about-hypnosis
UNIT=/etc/systemd/system/${SERVICE}.service

# Resolve project root (assumes this script is in project root)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CODE_DIR="${SCRIPT_DIR}"
BIN="${CODE_DIR}/pro"

echo "Project directory: ${CODE_DIR}"
echo "Binary: ${BIN}"

# Stop and disable any existing unit, and remove old file to avoid stale content
sudo systemctl stop ${SERVICE} || true
sudo systemctl disable ${SERVICE} || true
if [[ -f "${UNIT}" ]]; then
  sudo rm -f "${UNIT}"
fi

# Create/overwrite the unit file
sudo tee "${UNIT}" >/dev/null <<EOF
[Unit]
Description=Run smth-about-hypnosis (rpi-rgb-led-matrix + GIF/text)
After=multi-user.target

[Service]
Type=simple
User=root
Group=root
# Keep relative file paths working (fonts/, src/text_display/, etc.)
WorkingDirectory=${CODE_DIR}
# Run the program (prebuilt)
ExecStart=${BIN}
Restart=always
RestartSec=2s
TimeoutStopSec=5s
StandardOutput=journal
StandardError=inherit

[Install]
WantedBy=multi-user.target
EOF

echo "Reloading systemd and enabling service..."
sudo systemctl daemon-reload
sudo systemctl enable ${SERVICE}
sudo systemctl restart ${SERVICE}

echo
echo "Installed and started ${SERVICE}."
echo "Check logs: sudo journalctl -u ${SERVICE} -f"
echo "Disable/stop if needed: sudo systemctl disable --now ${SERVICE}"