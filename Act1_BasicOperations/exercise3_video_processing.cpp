#include <bits/stdc++.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

cv::Mat frame2grayscale(cv::Mat frame){
    cv::Mat grayscale;
    cv::cvtColor(frame,grayscale,cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayscale, grayscale, cv::COLOR_GRAY2RGB);
    return grayscale;
}

cv::Mat overlay_edges(cv::Mat frame){
    cv::Mat overlay, canny, filtered;
    cv::GaussianBlur(frame,filtered, cv::Size(3,3),frame.type());
    cv::Canny(filtered, canny, 80, 200, 3, false);
    cv::cvtColor(canny, canny, cv::COLOR_GRAY2RGB);
    canny = canny.mul(cv::Scalar(150, 150, 0), 1);
    overlay = frame + canny;
    return overlay;

}

cv::Mat overlay_shapes(cv::Mat frame){
    cv::Mat overlay, shapes;
    shapes = frame.clone();
    shapes = cv::Scalar(0,0,0);
    cv::Mat canny;
    cv::Canny(frame, canny, 80, 200, 3, false);
    vector<cv::Vec4i> lines;
    cv::HoughLinesP(canny,lines,3,CV_PI/180,100,10,5);
    for (auto line: lines){
        cv::line(shapes, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(50,150,0), 3, cv::LINE_AA);
    }
    cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
    vector<cv::Vec3f> circles;
    cv::HoughCircles(frame, circles, cv::HOUGH_GRADIENT_ALT, 1.5, frame.rows/16, 300, 0.84, 1, 1000);
    for (auto circ : circles){
        cv::Point center = cv::Point(circ[0], circ[1]);
        cv::circle(shapes, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        int radius = circ[2];
        circle( shapes, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
    }
    cv::cvtColor(frame, frame, cv::COLOR_GRAY2RGB);
    cv::addWeighted(frame,1,shapes,0.4,0,overlay);
    return overlay;

}

int main() {
    cv::Mat frame, row1, row2, disp;
    cv::VideoCapture camera(1);

    if(!camera.isOpened()){
        cout<<"Error reading camera feed";
        return -1;
    }
    for (;;){
        camera.read(frame);
        if(frame.empty()){
            cout<<"NULL frame ";
            break;
        }
        cv::Mat gray = frame2grayscale(frame);
        cv::Mat edges = overlay_edges(gray);
        cv::Mat shapes = overlay_shapes(gray);
        // Concatanate images to display in single window
        cv::hconcat(frame, edges,row1);
        cv::hconcat(frame, shapes, row2);
        cv::vconcat(row1,row2, disp);
        imshow("Camera Feed", disp);     
        if(cv::waitKey(5)== 27){
             break;
        }
    }
    
    return 0;
}

