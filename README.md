# rpi-64x64-led-matrix

## requirements
Requires openssl and curl to work
```
sudo apt install openssl

sudo apt install libcurl4-openssl-dev
```

## clone command
To automatically import needed libraries, make sure to include the `--recursive` command when cloning
```
clone --recursive https://github.com/II1302-Group4/rpi-64x64-led-matrix
```

## compile command
Be in the repo folder after cloning then use make
```
make pixled
```

## run command
To run the program, the sudo command is needed during setup but later not used
```
sudo ./pixled
```
