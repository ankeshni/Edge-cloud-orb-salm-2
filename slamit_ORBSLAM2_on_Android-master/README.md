## An Android version of ORB-SLAM2

# Introduction

This has some updates to the original slamit app. Some build issues related to C++ modules were fixed.                                                  
The main slam UI has been reorganized. Background color for the point cloud mapping has been changed to black instead of white. Mapping points are blue and red instead of black and red.

![alt text][logo]

[logo]: https://github.com/aivijay/slamit_ORBSLAM2_on_Android/blob/master/images/slam-samsung-s6edge.png "SMAL Screenshot"

# Notes on running

1. You will have to create a folder named SLAM on the internal storage (eg., /storage/emulated/0/SLAM)
2. ORBvoc.txt file needs to be copied to this folder (refer ORB_SLAM2 and get the zip file <a href="https://github.com/raulmur/ORB_SLAM2/blob/master/Vocabulary/ORBvoc.txt.tar.gz">ORBvoc.txt.tar.gz</a> and untar it and put the ORBvoc.txt file in the SLAM folder). Without this the app will crash.
3. Calibrate your camera using OpenCV or some calibration tool. Use the TUM1.yaml file in ORB_SLAM2 C++ application under examples and sync your calibrated data for fx, fy, cx, cy, k1, k2, p1, p2, and p3.
4. Put the calibrated file into the SLAM folder. By default slamit uses the file named List3.yaml as the calibration file if its not slected during the run of the application
5. Once the calibration file is selected and ORBvoc.txt is in your SLAM folder. The applicastion should start initialize SLAM and after a few seconds, show the tracker on your right in a black screen. A preview of the video fed will be shown on the left side. The SLAM initialization takes almost close to a minute on a normal phone (tested on Samsung S6 edge, HTC m8 and LG G3). Depending on the phones processing power and memory, it will take some time.
6. Once the tracking starts which will happens after a few seconds after SLAM initialization, the point cloud should start showing up on the right side in blue and red. Tracking usually starts around 20 seconds or so or even earlier provided proper camera calibration is done and updated in the .yaml file which is loaded. 
7. As you move around, it should continue tracking and plotting the point cloud with the camera preview moving for each tracked frame as appropriate.

# Building

If you want to build it from scratch, you will need to setup Android studio, ndk-build and a host of others especially if your planning on building the C++ modules and edit it for your own needs.
If there are problems during build with dependent libs for ORB_SLAM2, refer <a href="https://github.com/raulmur/ORB_SLAM2">ORB_SLAM2</a> github page and install them as needed.

1. Setup Android studio and import the project.
2. Setup ndk on your machine
3. In order to build ORBSlam2 C++ shared object, you can build it from the terminal from the oRB_SLAM2_Android/src/main directory. Just run ndk-build and it should show some warnings but build. Note you will have to have an ndk setup on your machine and be in your path to build. Once its successful, it will create a bunch of .so files under libs/arm.../ folder. Just copy them into jniLibs/arm.../folder. 
4. In android studio, do a clean build and then build the apk. Finally run the oRB_SMAL2_Android project and it should install the apk to your android device and run it. You should be able to run the app and select your calibration file and run SLAM.

# Credits

Raul Mur-Artal for getting ORB_SLAM2 released.

The capstone design project for UM-SJTU Joint Institute in Shanghai Jiao Tong Univeristy sponsored by Huawei Technologies

Thanks to the teamwork of Luwei Yu, Xiaobai Ma, Zhenkai Wang, Zhi Zhang and myself, we made an Android App named *slamit*, which could achieve realtime Simutaneous Localization and Mappint (SLAM) using the ORB-SLAM2 algorithm. It worked successfully on Huawei P9.
