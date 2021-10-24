#include"canny.hpp"


u_char** canny(u_char **img, int rows, int cols) {
    img =  gaussian_filter(img, rows, cols);
    return gradient(img, rows, cols);
}

u_char**  gaussian_filter(u_char **in, int rows, int cols) {
    int gaussian_mask[5][5] = {
                                {1, 4, 7, 4, 1},
                                {4, 16, 26, 16, 4},
                                {7, 26, 41, 26, 7},
                                {4, 16, 26, 16, 4},
                                {1, 4, 7, 4, 1}
                               };
    u_char **out = new u_char*[rows];
    for(int i = 0; i < rows; i ++) {
        out[i] = new u_char[cols];
        for(int j = 0; j < cols; j ++) {
            out[i][j] = in[i][j];
        }
    }
    for(int i = 2; i < rows - 2; i ++) {
        for(int j = 2; j < cols - 2; j ++) {
            int new_pixel = 0;
            for(int row_offset = -2; row_offset <= 2; row_offset ++) {
                for(int col_offset = -2; col_offset <= 2; col_offset ++) {
                    int cur_row = i + row_offset;
                    int cur_col = j + col_offset;
                    new_pixel += (int)in[cur_row][cur_col] * gaussian_mask[2 + row_offset][2 + col_offset];
                }
            }
            out[i][j] = (u_char)(new_pixel / 273);
        }
    }
    return out;
}

u_char **gradient(u_char **in, int rows, int cols) {
    
    int gx_mask[3][3] = {
                         {-1, 0, 1}, 
                         {-2, 0, 2}, 
                         {-1, 0, 1}
                        };
    int gy_mask[3][3] = {
                         {-1, -2, -1}, 
                         {0, 0, 0}, 
                         {1, 2, 1}
                        };
    u_char **gradient = new u_char*[rows];
    for(int i = 0; i < rows; i ++) {
        gradient[i] = new u_char[cols];
        for(int j = 0; j < cols; j ++) {
            gradient[i][j] = in[i][j];
        }
    }
    for(int i = 1; i < rows - 1; i ++) {
        for(int j = 1; j < cols - 1; j ++) {
            int gx = 0;
            int gy = 0;
            for(int row_offset = -1; row_offset <= 1; row_offset ++) {
                for(int col_offset = -1; col_offset <= 1; col_offset ++) {
                    gx += in[i + row_offset][j + col_offset] * gx_mask[row_offset + 1][col_offset + 1];
                    gy += in[i + row_offset][j + col_offset] * gy_mask[row_offset + 1][col_offset + 1];
                }
            }
            gradient[i][j] = sqrt(gx * gx + gy * gy);
        }
    }

    return gradient;
}