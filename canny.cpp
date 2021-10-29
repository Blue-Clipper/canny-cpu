#include"canny.hpp"

Canny::Canny(u_char **img, int upper_threshold, int lower_threshold, int rows, int cols) {
    _upper_threshold = upper_threshold;
    _lower_threshold = lower_threshold;
    _origin_img = img;
    _rows = rows;
    _cols = cols;

    _edge_img = new u_char*[_rows];
    for(int i = 0; i < _rows; i ++) {
        _edge_img[i] = new u_char[_cols];
        for(int j = 0; j < _cols; j ++) {
            _edge_img[i][j] = 0;
        }
    }
    gaussian_filter();
    gradient();
    trace_edge();
    suppress();
}
Canny::~Canny() {
    delete_img(_origin_img);
    delete_img(_filtered_img);
    delete_img(_gradient_strength);
    delete_img(_gradient_direction);
    delete_img(_edge_img);
}
u_char** Canny::getEdge() {
    return _edge_img;
}

u_char** Canny::getGaussian() {
    return _filtered_img;
}
u_char** Canny::getGradientStrength() {
    return _gradient_strength;
}
u_char** Canny::getGradientDirection() {
    return _gradient_direction;
}
void Canny::gaussian_filter() {
    int gaussian_mask[5][5] = {
                                {1, 4, 7, 4, 1},
                                {4, 16, 26, 16, 4},
                                {7, 26, 41, 26, 7},
                                {4, 16, 26, 16, 4},
                                {1, 4, 7, 4, 1}
                               };
    _filtered_img = new u_char*[_rows];
    for(int i = 0; i < _rows; i ++) {
        _filtered_img[i] = new u_char[_cols];
        for(int j = 0; j < _cols; j ++) {
            _filtered_img[i][j] = _origin_img[i][j];
        }
    }
    for(int i = 2; i < _rows - 2; i ++) {
        for(int j = 2; j < _cols - 2; j ++) {
            int new_pixel = 0;
            for(int row_offset = -2; row_offset <= 2; row_offset ++) {
                for(int col_offset = -2; col_offset <= 2; col_offset ++) {
                    int cur_row = i + row_offset;
                    int cur_col = j + col_offset;
                    new_pixel += (int)_origin_img[cur_row][cur_col] * gaussian_mask[2 + row_offset][2 + col_offset];
                }
            }
            _filtered_img[i][j] = (u_char)(new_pixel / 273);
        }
    }
}

void Canny::gradient() {
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
    _gradient_strength = new u_char*[_rows];
    for(int i = 0; i < _rows; i ++) {
        _gradient_strength[i] = new u_char[_cols];
        for(int j = 0; j < _cols; j ++) {
            _gradient_strength[i][j] = _filtered_img[i][j];
        }
    }
    _gradient_direction = new u_char*[_rows];
    for(int i = 0; i < _rows; i ++) {
        _gradient_direction[i] = new u_char[_cols];
        for(int j = 0; j < _cols; j ++) {
            _gradient_direction[i][j] = _filtered_img[i][j];
        }
    }

    for(int i = 1; i < _rows - 1; i ++) {
        for(int j = 1; j < _cols - 1; j ++) {
            int gx = 0;
            int gy = 0;
            for(int row_offset = -1; row_offset <= 1; row_offset ++) {
                for(int col_offset = -1; col_offset <= 1; col_offset ++) {
                    gx += _filtered_img[i + row_offset][j + col_offset] * gx_mask[row_offset + 1][col_offset + 1];
                    gy += _filtered_img[i + row_offset][j + col_offset] * gy_mask[row_offset + 1][col_offset + 1];
                }
            }
            _gradient_strength[i][j] = sqrt(gx * gx + gy * gy);
            double angle = (atan2(gx, gy) / M_PI) * 180.0;
            if ( ( (angle < 22.5) && (angle > -22.5) ) || (angle > 157.5) || (angle < -157.5) )
				_gradient_direction[i][j] = 0;
			else if ( ( (angle > 22.5) && (angle < 67.5) ) || ( (angle < -112.5) && (angle > -157.5) ) )
				_gradient_direction[i][j] = 45;
			else if ( ( (angle > 67.5) && (angle < 112.5) ) || ( (angle < -67.5) && (angle > -112.5) ) )
				_gradient_direction[i][j] = 90;
			else if ( ( (angle > 112.5) && (angle < 157.5) ) || ( (angle < -22.5) && (angle > -67.5) ) )
				_gradient_direction[i][j] = 135;
        }
    }
}

void Canny::trace_edge() {
    
    for(int i = 1; i < _rows - 1; i ++) {
        for(int j = 1; j < _cols - 1; j ++) {
            bool edgeEnd = false;
            if(_gradient_strength[i][j] > _upper_threshold) {
                switch(_gradient_direction[i][j]) {
                    case 0:
                        find_edge(0, 1, i, j, 0);
                        break;
                    case 45:
                        find_edge(1, 1, i, j, 45);
                        break;
                    case 90:
                        find_edge(1, 0, i, j, 90);
                        break;
                    case 135:
                        find_edge(1, -1, i, j, 135);
                        break;
                    default :
                        _edge_img[i][j] = 0;     
                }
            } else {
                _edge_img[i][j] = 0;     
            }
        }
    }
}
void Canny::find_edge(int rowShift, int colShift, int i, int j, int dir) {
	bool edgeEnd = false;
    int newRow = i + rowShift;
	int newCol = j + colShift;
    if(newRow < 0 || newRow >= _rows) {
        edgeEnd = true;
    }
    if(newCol < 0 || newCol >= _cols) {
        edgeEnd = true;
    }
    while((_gradient_direction[newRow][newCol] == dir) && !edgeEnd &&
        (_gradient_strength[newRow][newCol] > _lower_threshold)) {
            _edge_img[newRow][newCol] = 255;
            newRow = newRow + rowShift;
            newCol = newCol + colShift;
            if(newRow < 0 || newRow >= _rows) {
                break;
            }
            if(newCol < 0 || newRow >= _cols) {
                break;
            }
    }
    for(int m = 0; m < _rows; m ++) {
        for(int n = 0; n < _cols; n ++) {
            _edge_img[m][n] = _edge_img[m][n] == 255 ? 255 : 0;
        }
    }
}
void Canny::suppress() {
    for(int i = 1; i < _rows - 1; i ++) {
        for(int j = 1; j < _cols; j ++) {
            if(_edge_img[i][j] == 255) {
                switch (_gradient_direction[i][j]) {
                    case 0:
                        suppress_non_max(1, 0, i, j, 0);
                        break;
                    case 45:
                        suppress_non_max(1, -1, i, j, 45);
                        break;
                    case 90:
                        suppress_non_max(0, 1, i, j, 90);
                        break;
                    case 135:
                        suppress_non_max(1, 1, i, j, 135);
                }

            }
        }
    }
}
void Canny::suppress_non_max(int rowShift, int colShift, int i, int j, int dir) {
    bool edgeEnd = false;
    float nonMax[_cols][3];
    int pixelCnt = 0;
    int cnt;
    int newRow = i + rowShift;
    int newCol = j + colShift;
    if(newRow < 0 || newRow >= _rows) {
        edgeEnd = true;
    } 
    if(newCol < 0 || newCol >= _cols) {
        edgeEnd = true;
    }
    while(_gradient_direction[newRow][newCol] == dir && !edgeEnd && _edge_img[newRow][newCol] == 255) {
        int newRow = newRow + rowShift;
        int newCol = newCol + colShift;
        if(newRow < 0 || newRow >= _rows) {
            break;
        } 
        if(newCol < 0 || newCol >= _cols) {
            break;
        }
        nonMax[pixelCnt][0] = newRow;
        nonMax[pixelCnt][1] = newCol;
        nonMax[pixelCnt][2] = _gradient_strength[newRow][newCol];
        pixelCnt ++;
    }
    edgeEnd = false;
    colShift *= -1;
    rowShift *= -1;
    newRow = i + rowShift;
    newCol = j + colShift;
    if(newRow < 0 || newRow >= _rows) {
        edgeEnd = true;
    } 
    if(newCol < 0 || newCol >= _cols) {
        edgeEnd = true;
    }
    while(_gradient_direction[newRow][newCol] == dir && !edgeEnd && _edge_img[newRow][newCol] == 255) {
        int newRow = newRow + rowShift;
        int newCol = newCol + colShift;
        if(newRow < 0 || newRow >= _rows) {
            break;
        } 
        if(newCol < 0 || newCol >= _cols) {
            break;
        }
        nonMax[pixelCnt][0] = newRow;
        nonMax[pixelCnt][1] = newCol;
        nonMax[pixelCnt][2] = _gradient_strength[newRow][newCol];
        pixelCnt ++;
    }
    int max_pixel[3] = {0, 0, 0};
    for(int k = 0; k < pixelCnt; k ++) {
        if(nonMax[k][2] > max_pixel[2]) {
            max_pixel[0] = nonMax[k][0];
            max_pixel[1] = nonMax[k][1];
            max_pixel[2] = nonMax[k][2]; 
        }
    }
    for(int k = 0; k < pixelCnt; k ++) {
        _edge_img[max_pixel[0]][max_pixel[1]] = 0;
    }
}
void Canny::delete_img(u_char **ptr) {
    if(!ptr) {
        return;
    }
    for(int i = 0; i < _rows; i ++) {
        delete ptr[i];
    }
    delete ptr;
}