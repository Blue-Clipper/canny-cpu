#include"canny.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;

int main() {
    
    string input_path = "/home/gao/canny-cpu/input/sunflower.jpg";
    string output_path = "/home/gao/canny-cpu/output/edge_sunflower.jpg";
    // mat to uchar[][]
    cv::Mat grayscale_image = cv::imread(input_path, cv::IMREAD_GRAYSCALE);
    int img_rows = grayscale_image.rows;
    int img_cols = grayscale_image.cols;
    u_char **img = new u_char*[img_rows];
    for(int i = 0; i < img_rows; i ++) {
        img[i] = new u_char[img_cols];
    }
    for(int i = 0; i < img_rows; i ++) {
        for(int j = 0; j < img_cols; j ++) {
            img[i][j] = grayscale_image.at<u_char>(i, j);
        }
    }

    // find edge
    canny(img, img_rows, img_cols);

    // uchar[][] to mat
    cv::Mat edge_image(img_rows, img_cols, CV_8UC1);
    for(int i = 0; i < img_rows; i ++) {
        for(int j = 0; j < img_cols; j ++) {
            edge_image.at<u_char>(i, j) = img[i][j];
        }
    }
    cv::imwrite(output_path, edge_image);
    for(int i = 0; i < img_rows; i ++) {
        delete img[i];
    }
    delete img;
    
    return 0;


}
