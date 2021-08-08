# OpenCV Playground
Collection of several sample apps for trying out opencv's functionalities.

## Prerequisite
Open CV install or build linked inside `external` folder
> In my case I built the opencv source (`4.5.3`) and linked the cmake build directory like this
```
<PROJECT'S PATH>/external/opencv -> <OPENCV's PATH>/build_opencv
```

## Building
```
# after cloning
mkdir build && cd build
cmake ..
make
```