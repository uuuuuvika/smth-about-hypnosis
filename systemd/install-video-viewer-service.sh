#!/bin/bash
set -euo pipefail

SERVICE_NAME=rpi-video-viewer
UNIT_DEST=/etc/systemd/system/${SERVICE_NAME}.service
ENV_DEST=/etc/default/${SERVICE_NAME}
BIN_DEST=/usr/local/bin/video-viewer

# Resolve repo root from this script's location
REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
UNIT_SRC="${REPO_DIR}/utils/systemd/${SERVICE_NAME}.service"
ENV_EXAMPLE_SRC="${REPO_DIR}/utils/systemd/${SERVICE_NAME}.env.example"
BIN_SRC_DEFAULT="${REPO_DIR}/utils/video-viewer"
BIN_SRC="${BIN_SRC:-${1:-${BIN_SRC_DEFAULT}}}"

echo "Installing binary to ${BIN_DEST} (source: ${BIN_SRC})"
if [[ ! -x "${BIN_SRC}" ]]; then
  echo "Error: binary not found or not executable at ${BIN_SRC}" 1>&2
  echo "Build it first, e.g.: make -C ${REPO_DIR}/utils video-viewer" 1>&2
  exit 1
fi
sudo install -m 0755 "${BIN_SRC}" "${BIN_DEST}"

echo "Installing systemd unit to ${UNIT_DEST}"
sudo install -m 0644 "${UNIT_SRC}" "${UNIT_DEST}"

if [[ -f "${ENV_DEST}" ]]; then
  echo "Env file exists at ${ENV_DEST} (leaving unchanged)."
else
  echo "Installing default env to ${ENV_DEST}"
  sudo install -m 0644 "${ENV_EXAMPLE_SRC}" "${ENV_DEST}"
fi

echo "Reloading systemd daemon"
sudo systemctl daemon-reload

echo "Enabling service ${SERVICE_NAME}"
sudo systemctl enable ${SERVICE_NAME}

echo "Starting service ${SERVICE_NAME}"
sudo systemctl restart ${SERVICE_NAME}

echo "${SERVICE_NAME} installed and started. Edit ${ENV_DEST} and run: sudo systemctl restart ${SERVICE_NAME} to apply changes."
