#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

vector<cv::Mat> images;

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

void get_edges(cv::Mat img)
{
    cv::Mat filtered;
    cv::GaussianBlur(img, filtered, cv::Size(5, 5), img.type());
    cv::Mat sobel, canny, prewitx, prewity, robertsx, robertsy;

    cv::filter2D(filtered, prewitx, -1, prewit_kernelx);
    cv::filter2D(filtered, prewity, -1, prewit_kernely);

    cv::filter2D(filtered, robertsx, -1, roberts_kernelx);
    cv::filter2D(filtered, robertsy, -1, roberts_kernely);

    cv::Sobel(filtered, sobel, CV_8U, 1, 1, 5);
    cv::Canny(img, canny, 100, 200, 3, false);

    cv::imshow("Original", img);
    cv::imshow("Prewit", (prewitx + prewity));
    cv::imshow("Roberts", (robertsx + robertsy));
    cv::imshow("Sobel", sobel);
    cv::imshow("Canny", canny);
}

int main()
{
    load_images("Buildings/", "jpg");
    for (cv::Mat img : images)
    {
        get_edges(img);
        cv::waitKey(0);
    }
}