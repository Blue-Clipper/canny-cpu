#include"io.hpp"

cv::Mat array2Mat(u_char **arr, int rows, int cols) {
    cv::Mat mat(rows, cols, CV_8UC1);
    if(!arr) {
        return mat;
    }
    for(int i = 0; i < rows; i ++) {
        for(int j = 0; j < cols; j ++) {
            mat.at<u_char>(i, j) = arr[i][j];
        }
    }
    return mat;
}

u_char** mat2Array(cv::Mat mat, int rows, int cols) {
    u_char **img = new u_char*[rows];
    for(int i = 0; i < rows; i ++) {
        img[i] = new u_char[cols];
    }
    for(int i = 0; i < rows; i ++) {
        for(int j = 0; j < cols; j ++) {
            img[i][j] = mat.at<u_char>(i, j);
        }
    }
    return img;
}