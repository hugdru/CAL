#/usr/bin/env bash

set -e

sudo apt-get update
sudo apt-get upgrade

echo "*********************************************"
echo 'If you want to configure Virtual Box do the following:'
echo 'In the Virtual Box menu go to Devices->Insert Guest Additions CD Image...'
echo 'Type Y when done'
echo "*********************************************"
read answer
if [[ "$answer" = "Y" ]]; then
	sudo apt-get install build-essential module-assistant
	sudo m-a prepare
	additions_folder="$(ls -dvr "/media/$USER/VBOXADDITIONS"* | head -n 1)"
	echo "Found $additions_folder"
	sudo sh "${additions_folder}/VBoxLinuxAdditions.run"
fi

packages=("git" "git-doc" "libc6" "g++-multilib" "gcc-multilib" "libc++1" "libc++abi1" "libcurl4-gnutls-dev" "libcurlpp-dev" "libboost-all-dev" "eclipse-cdt" "clang" "clang-format" "clang-tidy")
sudo apt-get install "${packages[@]}"
