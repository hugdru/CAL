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
echo
if [[ "$answer" = "Y" ]]; then
  sudo apt-get install build-essential module-assistant
  sudo m-a prepare
  additions_folder="$(ls -dvr "/media/$USER/VBOXADDITIONS"* | head -n 1)"
  echo "Found $additions_folder"
  sudo sh "${additions_folder}/VBoxLinuxAdditions.run"
fi

packages=("git" "git-doc" "libc6" "g++-multilib" "gcc-multilib" "libc++1" "libc++abi1" "libcurl4-gnutls-dev" "libcurlpp-dev" "libboost-all-dev" "autoconf" "libtool-bin")
sudo apt-get install "${packages[@]}"

echo
echo "Downloading Curlpp"
pkgver="0.7.3"
curlpp_url="https://github.com/jpbarrette/curlpp/archive/v${pkgver}.tar.gz"
tmp_dir="/tmp/$USER/curlpp"
mkdir -p "$tmp_dir"

cd "$tmp_dir"
output_file="curlpp.tar.gz"
wget "$curlpp_url" -O "$output_file"
tar xfz "$output_file"
cd curlpp-*

echo
echo "Compiling Curlpp"
./autogen.sh
./configure --prefix=/usr/local
sudo make CXXFLAGS='-O2' install
cd ~
sudo rm -rf "$tmp_dir"

echo
echo 'Do you want to install codelite?'
echo 'Type Y for yes'
read install_codelite
if [[ "$install_codelite" = "Y" ]]; then
  sudo apt-get install "codelite" "codelite-plugins"
fi
