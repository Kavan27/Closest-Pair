#include "closest_helpers.h"
#include "math.h"

int verbose = 0;

int compare_x(const void* a, const void* b) 
{
    int A = ((struct Point *)a) -> x;
    int B = ((struct Point *)b) -> x;
    return ( A - B );
} 

int compare_y(const void* a, const void* b) 
{
    int A = ((struct Point *)a) -> y;
    int B = ((struct Point *)b) -> y;
    return ( A - B );
} 

double dist(struct Point p1, struct Point p2)
{
    double distance = sqrt(pow((p2.x - p1.x), 2) + pow((p1.y - p2.y), 2));
    return distance;
}
