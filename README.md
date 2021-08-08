# OpenCV Playground
Collection of several sample apps for trying out opencv's functionalities.

## Prerequisite
Open CV install or build linked inside `external` folder
> In my case I built the opencv source (`4.5.3`) and made a soft link to the cmake build directory like this
```
<PROJECT'S PATH>/external/opencv -> <OPENCV's PATH>/build_opencv
```

It's also convenient to create a data directory that has a soft link to opencv's sample directory
```
<PROJECT'S PATH>/data/opencv_samples -> <OPENCV's PATH>/opencv/samples/data/
```

## Building
```
# after cloning
mkdir build && cd build
cmake ..
make
```