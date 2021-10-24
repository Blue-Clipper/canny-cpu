#include<iostream>
#include<math.h>
using namespace std;

u_char** canny(u_char **img, int rows, int cols);


u_char**  gaussian_filter(u_char **in, int rows, int cols);

u_char **gradient(u_char **in, int rows, int cols);