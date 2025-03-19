#!/bin/bash
 
source /usr/local/qnx/env/bin/activate
source $HOME/qnx800/qnxsdp-env.sh
 
cd "$HOME/Desktop/projet/🧑🏾‍💻Software engineering/📝School/my-project/"
 
aarch64-unknown-nto-qnx8.0.0-g++ -o my-project -Igpio/aarch64/ my-project.cpp