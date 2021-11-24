#include "closest_parallel.h"
#include <stdlib.h>
#include "math.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int curr_depth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
//   static int num_forks = 0;
    if (n <= 3 || pdmax == 0){
        return _closest_serial(P, n);
    }
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
    int fd_r[2], fd_l[2];
    pipe(fd_l);
    pid_t left_result = fork();
    if(left_result > 0){
        close(fd_l[1]);

    }
    else if(left_result == 0){
        close(fd_l[0]);
        double ans_l = _closest_parallel(left, left_size, pdmax - 1, pcount);
        *pcount += 1;
        if(write(fd_l[1], &ans_l, sizeof(double)) == -1){
            exit(1);
        }
        exit(*pcount);// check proper exit
    }
    pipe(fd_r);
    pid_t right_result = fork();
    if(right_result > 0){
        close(fd_r[1]);
    }
    else if(right_result == 0){
        close(fd_r[0]);
        double ans_r = _closest_parallel(right, right_size, pdmax - 1, pcount);
        *pcount += 1;
        if(write(fd_r[1], &ans_r, sizeof(double)) == -1){
            exit(1);
        }
        exit(*pcount);
    }

    int status1, status2;
    wait(&status1);
    *pcount += WEXITSTATUS(status1);
    wait(&status2);
    *pcount += WEXITSTATUS(status2);
    double dr, dl;
    if(read(fd_l[0], &dl, sizeof(double)) == -1){
        exit(1);
    }
    if(read(fd_r[0], &dr, sizeof(double)) == -1){
        exit(1);
    }
    double d = fmin(dl, dr);
    double clr = combine_lr(P, n, P[p_mid], d);
    free(left);
    free(right);
    return fmin(d, clr);
}
double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    qsort(P, n, sizeof(P[0]), compare_x);
    return _closest_parallel(P, n, pdmax, pcount);
}

