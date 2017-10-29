******************************************************************************
******************************************************************************
Ubuntu build steps. Focused on Ubuntu as it is the more popular platform for 
building public services wallets. The premise for this guide is based on real 
world experiences and not just documented requirements!

Notes: This guide it designed and tested for Ubuntu 14.04.x LTS. It may work 
well with newer or older version of Ubuntu but may need some user intervention 
to complete. Your PC or VM must have at least 1.5 GB of RAM or boostcoin will 
fail to compile. Where possible absolute paths have been specified to help 
give the user an understanding of the exact file / folder locations.

Requirements: This guide is based on the user having some (or basic) 
knowledge of using a terminal environment on Ubuntu. You may copy each of 
the commands line by line or in batches to speed up the build process. 
You may also run the supplied 'unix-build-all-automated.sh' script to attempt 
an automated build of both the daemon and the full Qt GUI wallet. Either 
way this has been tested to work on Ubuntu 14.04.x. If you run into problems, 
post a message in the forum or in the usual support channels explaining in 
as much detail as possible the issue. We or another kind person will be 
able to help :)

Each list of commands are listed below under the --------------- heading.
******************************************************************************
******************************************************************************


Update your Ubuntu OS repositories with the latest patches:
---------------------
sudo apt-get update


Upgrade your ubuntu OS to the latest release:
---------------------
sudo apt-get upgrade -y


Install dependencies needed to compile boostcoin: 
--------------------- 
sudo apt-get install -y qt4-qmake libqt4-dev build-essential libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libminiupnpc-dev libevent-dev git


Install Berkeley db 4.8 (Note 4.8 is no longer available via apt-get so we add the Bitcoin repository)
---------------------
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev


Daemon wallet build: (Skip this section if you only want to build the full Qt GUI wallet)
--------------------
mkdir $HOME/boostcoin
cd $HOME/boostcoin
git clone https://github.com/mammix2/boostcoin-core.git
cd boostcoin-core/src/leveldb
chmod +x ./build_detect_platform
cd ../
make -f makefile.unix
strip boostcoind


Qt GUI wallet build + extra dependencies: (if you have already built the daemon you wont need to clone the boostcoin repository)
--------------------
cd $HOME
mkdir $HOME/deps
cd $HOME/deps
wget http://zlib.net/zlib-1.2.8.tar.gz
wget http://download.sourceforge.net/libpng/libpng-1.6.21.tar.gz
tar xvfz zlib-1.2.8.tar.gz
tar xvfz libpng-1.6.21.tar.gz
cd $HOME/deps/zlib-1.2.8
./configure
make
sudo make install
cd $HOME/deps/libpng-1.6.21
./autogen.sh
./configure
make
sudo make install
sudo apt-get install -y libqrencode-dev

mkdir $HOME/boostcoin
cd $HOME/boostcoin
git clone https://github.com/mammix2/boostcoin-core.git
cd boostcoin-core/src/leveldb
chmod +x ./build_detect_platform
cd $HOME/boostcoin/boostcoin-core
qmake "RELEASE=1"
make -j$NPROC

