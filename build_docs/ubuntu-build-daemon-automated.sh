sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get install -y qt4-qmake libqt4-dev build-essential libboost-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libminiupnpc-dev git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev
mkdir $HOME/boostcoin
cd $HOME/boostcoin
git clone https://github.com/mammix2/boostcoin-core.git
cd boostcoin-core/src/leveldb
chmod +x ./build_detect_platform
cd ../
NPROC=$(nproc)
echo "nproc: $NPROC"
make -f makefile.unix -j$NPROC
strip boostcoind
