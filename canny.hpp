#include<iostream>
#include<math.h>
#include <memory>
using namespace std;



class Canny {
    public:

        Canny(u_char **img, int upper_threshold, int lower_threshold, int rows, int cols);
        ~Canny();
        u_char** getEdge();
        u_char** getGaussian();
        u_char** getGradientStrength();
        u_char** getGradientDirection();
    private:
        unsigned _rows;
        unsigned _cols;
        int _upper_threshold;
        int _lower_threshold;
        u_char **_origin_img;
        u_char **_filtered_img;
        u_char **_gradient_strength;
        u_char **_gradient_direction;
        u_char **_edge_img;
        void gaussian_filter();
        void gradient();
        void trace_edge();
        void find_edge(int rowShift, int colShift, int row, int col, int dir);
        void delete_img(u_char **ptr);

};
