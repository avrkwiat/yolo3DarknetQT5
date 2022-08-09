## yolo3, Darknet AI Detector, QT5.15.2, Jetson Xavier NX
Program use Darknet Yolo in QT5.15.2. Test on Jetson Xavier NX. <BR/>
#I) OPENCV:
Use scrypt from here:
https://github.com/mdegans/nano_build_opencv/blob/master/build_opencv.sh
```sh
./build_opencv.sh 4.6.0
```
#II) QT5.15.2
Jetson use QT5 max You can compile 5.15.2. But you can try higher ;)
Here is https://forums.developer.nvidia.com/t/jetson-nano-and-qt5/76870/13 

#III) Darnet with YOLO:
author:
https://pjreddie.com/darknet/yolo/ <BR/>
Install from: https://github.com/AlexeyAB/darknet
```sh
git clone https://github.com/AlexeyAB/darknet
cd darknet
```
Edite Makefile:
```sh
sudo gedit Makefile
```
Change Makefile for this. Use ARCH 72 in Xavier :
```sh
GPU=1
CUDNN=1
CUDNN_HALF=1
OPENCV=1
OPENMP=1
LIBSO=1
ARCH= -gencode arch=compute_72,code=[sm_72,compute_72]
```
Now You can compile!
```sh
make
```
Test Darknet. 
Download model:
```sh
wget https://pjreddie.com/media/files/yolov3.weights
```
Test model image dog:
```sh
./darknet detect cfg/yolov3.cfg yolov3.weights data/dog.jpg
```
Download tiny fast model:
```sh
wget https://pjreddie.com/media/files/yolov3-tiny.weights
```
Test usb Camera:
```sh
./darknet detector demo cfg/coco.data cfg/yolov3.cfg yolov3.weights
```
Test IP camera:
```sh
./darknet detector demo cfg/coco.data cfg/yolov3-tiny.cfg yolov3-tiny.weights rtsp://admin:password@192.168.88.65:554/Streaming/channels/1/?transportmode=multicast
```

# IV) Configure Project:
You change Your camera IP adress and fun. I test on HiKVision Camera. But sametimes You need change rtsp stream on : 
```sh
rtsp://192.168.x.x:554/live/0/Main
```
