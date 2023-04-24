#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

vector<cv::Mat> images, canny_list, sobel_list, prewit_list, roberts_list, enhanced_sobel, enhanced_roberts;
vector<int> compression_params;
    
int prewit_veritcal[3][3] = {
    {-1, 0, 1},
    {-1, 0, 1},
    {-1, 0, 1}};
int prewit_horizontal[3][3]{
    {1, 1, 1},
    {0, 0, 0},
    {-1, -1, -1}};
int roberts_vertical[2][2] = {
    {0, 1},
    {-1, 0}};
int roberts_horizontal[2][2]{
    {1, 0},
    {0, -1}};

cv::Mat prewit_kernelx = (cv::Mat_<int>(3,3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
cv::Mat prewit_kernely = (cv::Mat_<int>(3,3) << -1,-1,-1, 0, 0, 0, 1, 1, 1);

cv::Mat roberts_kernelx = (cv::Mat_<int>(2,2) << 0, 1, -1, 0);
cv::Mat roberts_kernely = (cv::Mat_<int>(2,2) << 1,0,0,-1);

void load_images(string path, string imageType)
{
    for (int i = 0; i < 5; i++)
    {
        string imagePath = path + "Building" + to_string(i) + "." + imageType;
        cv::Mat readImg = cv::imread(imagePath);
        cvtColor(readImg, readImg, cv::COLOR_BGR2GRAY);
        images.push_back(readImg);
    }
}

void write_label(string text, cv::Mat &img){
    cv::putText(img, text, cv::Point(5, img.rows -40), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255), 4);
}

void get_edges(cv::Mat img, bool with_labels){
    cv::Mat filtered;
    cv::GaussianBlur(img, filtered, cv::Size(3, 3), img.type());
    cv::Mat sobel, sobelx, sobely, canny, prewitx, prewity, robertsx, robertsy;

    cv::filter2D(filtered, prewitx, -1, prewit_kernelx);
    cv::filter2D(filtered, prewity, -1, prewit_kernely);

    cv::filter2D(filtered, robertsx, -1, roberts_kernelx);
    cv::filter2D(filtered, robertsy, -1, roberts_kernely);

    cv::Sobel(filtered, sobelx, CV_8U, 1, 0, 3);
    cv::Sobel(filtered, sobely, CV_8U, 0, 1, 3);
    sobel = sobelx + sobely;
    cv::Canny(filtered, canny, 100, 200, 3, false);

    cv::Mat prewit = (prewitx + prewity);
    cv::Mat roberts =  (robertsx + robertsy);

    if(with_labels){
        write_label("Canny Edges", canny);
        write_label("Sobel Edges", sobel);
        write_label("Prewit Edges", prewit);
        write_label("Roberts Edges", roberts);
    }


    canny_list.push_back(canny);
    prewit_list.push_back(prewit);
    sobel_list.push_back(sobel);
    roberts_list.push_back(roberts);

}

void display_edges(bool write_result){
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    for(int i=0; i < images.size(); i++){
        cv::Mat disp, row1, row2;
        cv::hconcat(images[i], canny_list[i], row1);
        cv::hconcat(row1,sobel_list[i], row1);
        cv::hconcat(images[i], prewit_list[i], row2);
        cv::hconcat(row2, roberts_list[i], row2);
        cv::vconcat(row1, row2,disp);
        if(write_result){
            cv::imwrite("Results/Edges_Raw/Edges of building" +to_string(i) +".png",disp,compression_params);
        }
        cv::imshow("Edges of building" + to_string(i), disp);
    }
     cv::waitKey(0);
}

void enhance_sobel_eddges(cv::Mat img, int kernelSize){
    cv::Mat enhancedImg;
    cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(img,enhancedImg, cv::MORPH_GRADIENT,structuringElement);
    enhanced_sobel.push_back(enhancedImg);
}

void enhance_robert_edges(cv::Mat img, int kernelSize){
    cv::Mat enhancedImg;
    cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(kernelSize, kernelSize));
    cv::morphologyEx(img,enhancedImg, cv::MORPH_GRADIENT,structuringElement,cv::Point(-1,-1),1);
    cv::morphologyEx(img,enhancedImg, cv::MORPH_CLOSE,structuringElement,cv::Point(-1,-1),2);
    enhanced_roberts.push_back(enhancedImg);
}

void display_enhaced_edges(bool write_result){
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);
    for(int i=0; i < images.size(); i++){
        cv::Mat disp, row1, row2;
        cv::hconcat(sobel_list[i], enhanced_sobel[i], row1);
        cv::hconcat(roberts_list[i],enhanced_roberts[i], row2);
        cv::vconcat(row1, row2,disp);
        if(write_result == true){
            cout << "Writing";
            cv::imwrite("Results/Edges_Enhanced/Edges of building" +to_string(i) +".png",disp,compression_params);
        }
        cv::imshow("Enhanced_Edges of building" + to_string(i), disp);
    }
     cv::waitKey(0);
}

int main(){
    load_images("Buildings/", "jpg");
    for (cv::Mat img : images){
        get_edges(img, false);
    }
    display_edges(false);

    for(int i=0; i<images.size(); i++){
        enhance_sobel_eddges(sobel_list[i],3);
        enhance_robert_edges(roberts_list[i],3);
    }
    display_enhaced_edges(true);


}