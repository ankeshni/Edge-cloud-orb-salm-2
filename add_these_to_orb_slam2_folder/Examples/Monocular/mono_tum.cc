/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
    if(argc != 4)
    {
        cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings path_to_sequence" << endl;
        return 1;
    }

    // Retrieve paths to images
    vector<string> vstrImageFilenames;
    vector<double> vTimestamps;
    string strFile = string(argv[3])+"/rgb.txt";
    LoadImages(strFile, vstrImageFilenames, vTimestamps);

    int nImages = vstrImageFilenames.size();

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);

    // Vector for tracking time statistics
    vector<float> vTimesTrack;
    vTimesTrack.resize(nImages);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;
    cout << "Images in the sequence: " << nImages << endl << endl;

    // Main loop
    cv::Mat im;
    for(int ni=0; ni<nImages; ni++)
    {
        // Read image from file
        im = cv::imread(string(argv[3])+"/"+vstrImageFilenames[ni],CV_LOAD_IMAGE_UNCHANGED);
        double tframe = vTimestamps[ni];

        if(im.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(argv[3]) << "/" << vstrImageFilenames[ni] << endl;
            return 1;
        }

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

        // Pass the image to the SLAM system
        SLAM.TrackMonocular(im,tframe);

#ifdef COMPILEDWITHC11
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
        std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif

        double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

        vTimesTrack[ni]=ttrack;

        // Wait to load the next frame
        double T=0;
        if(ni<nImages-1)
            T = vTimestamps[ni+1]-tframe;
        else if(ni>0)
            T = tframe-vTimestamps[ni-1];

        if(ttrack<T)
            usleep((T-ttrack)*1e6);
    }

    // Stop all threads
    SLAM.Shutdown();

    // Tracking time statistics
    sort(vTimesTrack.begin(),vTimesTrack.end());
    float totaltime = 0;
    for(int ni=0; ni<nImages; ni++)
    {
        totaltime+=vTimesTrack[ni];
    }
    cout << "-------" << endl << endl;
    cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
    cout << "mean tracking time: " << totaltime/nImages << endl;

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
    ifstream f;
    f.open(strFile.c_str());

    // skip first three lines
    string s0;
    getline(f,s0);
    getline(f,s0);
    getline(f,s0);

    while(!f.eof())
    {
        string s;
        getline(f,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenames.push_back(sRGB);
        }
    }
}
// #include "curl/curl.h" // has to go before opencv headers

// #include<iostream>
// #include<algorithm>
// #include<fstream>
// #include<chrono>

// #include<opencv2/core/core.hpp>

// #include<System.h>

// #include <iostream>
// #include <vector>
// using namespace std;

// #include <opencv2/opencv.hpp>
// using namespace cv;

// //curl writefunction to be passed as a parameter
// // we can't ever expect to get the whole image in one piece,
// // every router / hub is entitled to fragment it into parts
// // (like 1-8k at a time),
// // so insert the part at the end of our stream.
// size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
// {
//     vector<uchar> *stream = (vector<uchar>*)userdata;
//     size_t count = size * nmemb;
//     stream->insert(stream->end(), ptr, ptr + count);
//     return count;
// }

// //function to retrieve the image as cv::Mat data type
// cv::Mat curlImg(const char *img_url, int timeout=10)
// {
//     vector<uchar> stream;
//     CURL *curl = curl_easy_init();
//     curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
//     curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
//     CURLcode res = curl_easy_perform(curl); // start curl
//     curl_easy_cleanup(curl); // cleanup
//     return imdecode(stream, -1); // 'keep-as-is'
// }

// int main(int argc, char **argv)
// {
//     if(argc != 4)
//     {
//         cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings streamIP" << endl;
//         return 1;
//     }
//     ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);
//     int i=0;
//     while(1){
//         double tframe=i;
//         Mat im = curlImg(("http://"+string(argv[3])+"/shot.jpg").c_str());
//         if (im.empty()){
//             cout<<"No Video Stream Found at"+string(argv[3]); // load fail
//             break;
//         }
//         // namedWindow( "Image output", CV_WINDOW_AUTOSIZE );
//         // imshow("Image output",image); // here's your car ;)
//         // waitKey(1); // 
//         #ifdef COMPILEDWITHC11
//         std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
// #else
//         std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
// #endif

//         // Pass the image to the SLAM system
//         SLAM.TrackMonocular(im,tframe);

// #ifdef COMPILEDWITHC11
//         std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
// #else
//         std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
// #endif

//         double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

//         // vTimesTrack[ni]=ttrack;

//         // Wait to load the next frame
//         double T=0;
//         // if(ni<nImages-1)
//         //     T = i+1-tframe;
//         // else if(ni>0)
//         T = tframe-i-1;

//         if(ttrack<T)
//             usleep((T-ttrack)*1e6);
//         i++;
//     }
//     SLAM.Shutdown();
//     SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

//     return 0;
// }
