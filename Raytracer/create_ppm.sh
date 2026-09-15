#!/bin/bash
make fclean
make
rm -rf output.ppm
echo -e "\033[36mRunning raytracer...\033[0m"
echo -e "\033[33mGive a .cfg file as argument\033[0m"

if [ $# -ne 1 ]; then
    echo -e "\033[31mUsage: ./create_ppm.sh <scene.cfg>\033[0m"
    exit 1
fi

./raytracer "$1" > output.ppm

echo -e "\033[32mDone!\033[0m"
