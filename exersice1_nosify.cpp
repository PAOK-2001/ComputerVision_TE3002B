#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

vector<cv::Mat> images, filtered;

void load_images(string path, string imageType){
    for(int i=0; i < 10; i++){
        string imagePath = path + "road" + to_string(i) + "." + imageType;
        cv::Mat readImg = cv::imread(imagePath);
        cvtColor(readImg,readImg,cv::COLOR_BGR2GRAY);
        images.push_back(readImg);
    }
}

void write_label(string text, cv::Mat &img){
    cv::putText(img, text, cv::Point(5, img.rows -40), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255), 3);
}

void noisify(cv::Mat &img, cv::InputArray mean, cv::InputArray sigma){
    cv::Mat noise(img.size(),img.type());
    cv::randn(noise, mean , sigma); // Generate a normaly distributed pixel matrix
    img = img+noise; // Add the noise to the desired image
}

void denoisify(cv::Mat img, int ksize){
    cv::Mat gaussianFiltered, bilateralFiltered, medianFiltered, meanFiltered;
    cv::Mat row1, row2, disp;
    cv::Mat noisy_img = img;
    cv::Size kernelSize = cv::Size(ksize,ksize); // Create kernel mask for mean filter
    cv::Mat kernel = cv::Mat::ones(ksize, ksize, CV_32F )/ (float)(ksize*ksize);
    cv::GaussianBlur(img,gaussianFiltered, kernelSize,img.type());
    cv::bilateralFilter(img,bilateralFiltered,ksize,75,75);
    cv::medianBlur(img,medianFiltered,ksize);
    cv::filter2D(img,meanFiltered,-1,kernel);

    write_label("Gaussian", gaussianFiltered);
    write_label("Bilateral", bilateralFiltered);
    write_label("Median", medianFiltered);
    write_label("Mean", meanFiltered);
    
    cv::hconcat(img,gaussianFiltered,row1);
    cv::hconcat(row1,medianFiltered,row1);
    cv::hconcat(img, meanFiltered,row2);
    cv::hconcat(row2,bilateralFiltered, row2);
    cv::vconcat(row1,row2, disp);

    cv::imshow("Filters", disp);
    filtered.push_back(disp);
}

void save_results(){
    vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    string path = "Results/Noise_Filtering/Road";
    for(int i=0;i <filtered.size(); i++){
        cv::imwrite(path+to_string(i) +".png",filtered[i],compression_params);

    }
}


int main(){
    load_images("RoadSigns/", "png");
    for(cv::Mat img : images){
        noisify(img, (10,15,3), (10,20,30));
        denoisify(img, 7);
        cv::waitKey(0);
    }
    save_results();
}