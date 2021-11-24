#include "closest_serial.h"
#include <stdlib.h>
#include "math.h"


double combine_lr(struct Point P[], size_t n, struct Point mid_point, double d)
{
    int c = 0;
    for (int i = 0; i < n; i++) {
        if (abs(P[i].x - mid_point.x) < d){
            c++;
        }
    }
    struct Point *array = malloc(c * sizeof(struct Point));
    int counter = 0;
    for (int j = 0; j < n; j++) {
        if (abs(P[j].x - mid_point.x) < d){
            array[counter] = P[j];
            counter++;
        }
    }
    qsort(array, c, sizeof(array[0]), compare_y);
    double smallest = d;
    for (int x = 0; x < c; x++){
        for (int y = x + 1; y < c; y++) {
            if((array[y].y - array[x].y) >= d){
                break;
            }
            if((array[y].y - array[x].y < d) && dist(array[y], array[x]) < smallest){
                smallest = dist(array[y], array[x]);
            }
        }
    }
    free(array);
    return smallest;
}

double _closest_serial(struct Point P[], size_t n)
{
    int p_mid  = n / 2;
    struct Point *left = malloc(p_mid * sizeof(struct Point));
    struct Point *right = malloc((n - p_mid) * sizeof(struct Point));
    for (int i = 0; i < p_mid; i++) {
        left[i] = P[i];
    }
    for (int j = p_mid; j < n; j++) {
        right[j - p_mid] = P[j];
    }
    int left_size = p_mid;
    int right_size = n - p_mid;
    if(left_size <= 3 || right_size <= 3){
        free(left);
        free(right);
        return brute_force(P, n);
    }
    double dl = _closest_serial(left, left_size);    
    double dr = _closest_serial(right, right_size);
    double d = fmin(dl, dr);
    double clr = combine_lr(P, n, P[p_mid], d);
    free(left);
    free(right);
    return fmin(d, clr);
}

double closest_serial(struct Point P[], size_t n)
{
    qsort(P, n, sizeof(P[0]), compare_x);
    return _closest_serial(P, n);
}

