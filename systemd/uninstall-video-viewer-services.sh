#!/bin/bash
set -e

RUN_SERVICE=rpi-video-viewer
BUILD_SERVICE=rpi-video-viewer-build
RUN_UNIT=/etc/systemd/system/${RUN_SERVICE}.service
BUILD_UNIT=/etc/systemd/system/${BUILD_SERVICE}.service
ENV_FILE=/etc/default/${RUN_SERVICE}

PURGE=0
if [[ "$1" == "--purge" ]]; then
  PURGE=1
fi

echo "Stopping services if running..."
sudo systemctl stop ${RUN_SERVICE} || true
sudo systemctl stop ${BUILD_SERVICE} || true

echo "Disabling services..."
sudo systemctl disable ${RUN_SERVICE} || true
sudo systemctl disable ${BUILD_SERVICE} || true

echo "Removing unit files..."
if [[ -f "${RUN_UNIT}" ]]; then sudo rm -f "${RUN_UNIT}"; fi
if [[ -f "${BUILD_UNIT}" ]]; then sudo rm -f "${BUILD_UNIT}"; fi

if [[ ${PURGE} -eq 1 ]]; then
  echo "Purging env file ${ENV_FILE}..."
  if [[ -f "${ENV_FILE}" ]]; then sudo rm -f "${ENV_FILE}"; fi
fi

echo "Reloading systemd daemon..."
sudo systemctl daemon-reload

echo "Uninstall complete."
if [[ ${PURGE} -eq 1 ]]; then
  echo "Environment file removed."
else
  echo "Environment file kept at ${ENV_FILE}. Use --purge to remove it."
fi


