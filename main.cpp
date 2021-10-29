#include"canny.hpp"
#include"io.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;

int main() {
    string pojectPath = "/home/gao/canny-cpu";
    string input_path = pojectPath + "/input/sunflower.jpg";
    string output_path = pojectPath + "/output/edge_sunflower.jpg";

    cv::Mat grayscale_image = cv::imread(input_path, cv::IMREAD_GRAYSCALE);
    int rows = grayscale_image.rows;
    int cols = grayscale_image.cols;
    cout << rows << " rows, " << cols << " cols"<<endl;
    u_char **input = mat2Array(grayscale_image, rows, cols);
    Canny c(input, 128, 40, rows, cols);
    u_char **edge = c.getEdge();

    cv::Mat edge_img = array2Mat(edge, rows, cols);
    cv::imwrite(output_path, edge_img);

    return 0;


}
