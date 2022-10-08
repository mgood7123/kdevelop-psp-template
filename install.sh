sudo apt install libkf5syntaxhighlighting-dev gettext kdevplatform-dev

if [[ ! -e build ]]
    then
        mkdir build
fi

cd build
cmake ..
make
sudo make install
