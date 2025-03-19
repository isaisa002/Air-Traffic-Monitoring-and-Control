#!/bin/bash
echo "========================================================================"
echo "Activate QNX environments"
echo "Current directory content:"
ls

source /usr/local/qnx/env/bin/activate
source "$HOME/qnx800/qnxsdp-env.sh"

# Adjust directory to your project relative to the container's volume mount.
cd "$HOME/Desktop/projet/🧑🏾‍💻Software engineering/📝School/my-project/"

echo "========================================================================"
echo "Check if qemu-aarch64 exists; if not, try to install it via apt-get."
echo "Current directory content:"
ls

if ! command -v qemu-aarch64 &> /dev/null; then
    echo "========================================================================"
    echo "qemu-aarch64 not found; attempting to install qemu-user-static..."
    echo "Current directory content:"
    ls
    apt-get update && apt-get install -y qemu-user-static
    if ! command -v qemu-aarch64 &> /dev/null; then
       echo "========================================================================"
       echo "qemu-aarch64 installation failed. Please ensure qemu-user-static is installed."
       exit 1
    fi
fi

echo "========================================================================"
echo "Compile your QNX project"
echo "Current directory content:"
ls
aarch64-unknown-nto-qnx8.0.0-g++ -o my-project -Igpio/aarch64/ my-project.cpp

echo "========================================================================"
echo "Run the binary using Qemu"
echo "Current directory content:"
ls
qemu-aarch64 -L /path/to/qnx/sysroot ./my-project