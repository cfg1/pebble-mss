#!/bin/bash
# Install NPM 3
npm install npm@3 -g

set -e
echo 'pBuild 1.5'
echo 'Installing Pebble SDK and its Dependencies...'

cd ~

mkdir -p ~/pebble-dev
mkdir -p ~/.pebble-sdk
touch ~/.pebble-sdk/ENABLE_ANALYTICS

# Get the Pebble SDK and toolchain
if [ ! -d $HOME/pebble-dev/${PEBBLE_SDK} ]; then
  wget https://s3.amazonaws.com/assets.getpebble.com/pebble-tool/${PEBBLE_SDK}.tar.bz2

  # Extract the SDK
  tar -jxf ${PEBBLE_SDK}.tar.bz2 -C ~/pebble-dev/
  # Install the Python library dependencies locally
  cd ~/pebble-dev/${PEBBLE_SDK}
  sed -i '286s/.*/        return requests.get("{}{}".format(self.DOWNLOAD_SERVER, path), *args, verify=False)/' pebble-tool/pebble_tool/sdk/manager.py
  virtualenv --no-site-packages .env
  source .env/bin/activate
  pip install -r requirements.txt
  deactivate
  pip install -U pyopenssl==0.13.1 pyasn1 ndg-httpsclient
  ~/pebble-dev/${PEBBLE_SDK}/bin/pebble sdk install https://github.com/aveao/PebbleArchive/raw/master/SDKCores/sdk-core-4.3.tar.bz2
fi
