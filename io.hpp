#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>

cv::Mat array2Mat(u_char **arr, int rows, int cols);

u_char** mat2Array(cv::Mat mat, int rows, int cols);