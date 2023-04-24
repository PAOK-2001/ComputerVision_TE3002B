
#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

vector<cv::Mat> images, gray_thrsholded, colour_thrsholded;

void load_images(string path, string imageType){
    for(int i=0; i < 10; i++){
        string imagePath = path + "road" + to_string(i) + "." + imageType;
        images.push_back(cv::imread(imagePath));
    }

}

void grayscale_thresholding(cv::Mat loadedImage){
    cv::Mat display, grayscale, thresholded;
    cvtColor(loadedImage,grayscale,cv::COLOR_BGR2GRAY);
    cv::threshold(grayscale, thresholded, 130, 255, cv::THRESH_BINARY);
    cv::imshow("Original Image",loadedImage);
    cv::imshow("Grayscale",grayscale);
    cv::imshow("Thresholded",thresholded);
    gray_thrsholded.push_back(thresholded);

}

void threshold_over_color(cv::Mat img){
    cv::Mat thresholded;
    inRange(img, cv::Scalar(0, 150, 0), cv::Scalar(255, 255, 255), thresholded);
    cv::imshow("Thresholded (color)",thresholded);
    colour_thrsholded.push_back(thresholded);

}

void save_results(){
    vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    string binarization_path = "Results/Binarization/Binarization";
    for(int i = 0; i < images.size(); i++){
        cv::imwrite(binarization_path+to_string(i) +".png",gray_thrsholded[i],compression_params);
        cv::imwrite(binarization_path+"_colour_channel"+to_string(i) +".png",colour_thrsholded[i],compression_params);
    }
    
}

int main(){
    load_images("RoadSigns/", "png");
    for(cv::Mat image : images){
        grayscale_thresholding(image);
        threshold_over_color(image);
        cv::waitKey(0);
    }
    save_results();
}