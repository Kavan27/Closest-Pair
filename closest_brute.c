#include "closest_brute.h"
#include <stdio.h>
double brute_force(struct Point P[], size_t n) 
{
    double smallest = dist(P[0], P[1]);
    for(int x = 0; x < n; x++){
        for(int y = x + 1; y < n; y++) {
            double value = dist(P[x], P[y]);
            if(value < smallest){
                smallest = value;
            }
        }
    }
    return smallest;
} 
