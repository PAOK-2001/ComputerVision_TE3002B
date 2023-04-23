#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

vector<cv::Mat> images;

void load_images(string path, string imageType){
    for(int i=0; i < 10; i++){
        string imagePath = path + "road" + to_string(i) + "." + imageType;
        cv::Mat readImg = cv::imread(imagePath);
        cvtColor(readImg,readImg,cv::COLOR_BGR2GRAY);
        images.push_back(readImg);
    }
}

void noisify(cv::Mat &img, cv::InputArray mean, cv::InputArray sigma){
    cv::Mat noise(img.size(),img.type());
    cv::randn(noise, mean , sigma); // Generate a normaly distributed pixel matrix
    img = img+noise; // Add the noise to the desired image
}

void denoisify(cv::Mat img, int ksize){
    cv::Mat gaussianFiltered, bilateralFiltered, medianFiltered, meanFiltered;
    cv::Mat noisy_img = img;
    cv::Size kernelSize = cv::Size(ksize,ksize); // Create kernel mask for mean filter
    cv::Mat kernel = cv::Mat::ones(ksize, ksize, CV_32F )/ (float)(ksize*ksize);
    cv::GaussianBlur(img,gaussianFiltered, kernelSize,img.type());
    cv::bilateralFilter(img,bilateralFiltered,2,15,15);
    cv::medianBlur(img,medianFiltered,ksize);
    cv::filter2D(img,meanFiltered,-1,kernel);

    cv::imshow("Noisy Image",img);
    cv::imshow("Gaussian Filter", gaussianFiltered);
    cv::imshow("Bilateral Filter",bilateralFiltered);
    cv::imshow("Median Filter",medianFiltered);
    cv::imshow("Mean Filter",meanFiltered);
}


int main(){
    load_images("RoadSigns/", "png");
    for(cv::Mat img : images){
        noisify(img, (10,15,3), (10,20,30));
        denoisify(img, 5);
        cv::waitKey(0);
    }
}