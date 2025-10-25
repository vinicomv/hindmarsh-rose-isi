#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define FILE_BUFFER_SIZE (100 * 1024 * 1024)

// Runge-Kutta 4th order integration for the Hindmarsh–Rose model
void runge(double h, int repete, double b, double I, FILE *pm, FILE *pd) {

    // time variable (used for output and ISI computation)
    double time = 0.0;

    // fixed model parameters
    double a = 1.0;
    double c = 1.0;
    double d = 5.0;
    double s = 4.0;
    double r = 0.05;
    double Xr = -1.6;

    // state variables and ISI tracking
    double x, y, z;
    double isi;
    double isi_max = 0;
    double isi_min = 200000;

    // RK4 integration setup
    int i;
    double tj[2];         // array for spike time tracking
    tj[0] = time;

    // IC
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;
    x = x0; y = y0; z = z0;

    // RK4 loop
    for (i = 0; i < repete; i++) {

        // step 1 
        double k1 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l1 = c - d * pow(x, 2.0) - y;
        double m1 = r * (s * (x - Xr) - z);

        x = x0 + h * k1 / 2.0;
        y = y0 + h * l1 / 2.0;
        z = z0 + h * m1 / 2.0;

        // step 2 
        double k2 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l2 = c - d * pow(x, 2.0) - y;
        double m2 = r * (s * (x - Xr) - z);

        x = x0 + h * k2 / 2.0;
        y = y0 + h * l2 / 2.0;
        z = z0 + h * m2 / 2.0;

        // step 3 
        double k3 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l3 = c - d * pow(x, 2.0) - y;
        double m3 = r * (s * (x - Xr) - z);

        x = x0 + (h * k3);
        y = y0 + (h * l3);
        z = z0 + (h * m3);

        // step 4 
        double k4 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l4 = c - d * pow(x, 2.0) - y;
        double m4 = r * (s * (x - Xr) - z);

        // RK4 update
        x = x0 + h / 6.0 * (k1 + 2.0*k2 + 2.0*k3 + k4);
        y = y0 + h / 6.0 * (l1 + 2.0*l2 + 2.0*l3 + l4);
        z = z0 + h / 6.0 * (m1 + 2.0*m2 + 2.0*m3 + m4);

        // time step
        time += h;

        // detect and compute ISI after transient
        if (time > 5000) {
            if ((x0 * x) < 0 && (x < x0)) { 
                tj[1] = time;
                isi = tj[1] - tj[0];
                tj[0] = tj[1];

                if (isi > isi_max)
                    isi_max = isi;

                if (isi < isi_min)
                    isi_min = isi;
            }
        }

        x0 = x;
        y0 = y;
        z0 = z;
    }

    // ISI values to output files
    fprintf(pd, "%.16f %.16f %.16f\n", b, I, isi_max);

    if (isi_min < 5000)
        fprintf(pm, "%.16f %.16f %.16f\n", b, I, isi_min);
    else
        fprintf(pm, "%.16f %.16f 0\n", b, I, isi_min);
}

int main(int argc, const char *argv[]) {

    int i, j;

    // parameters 
    double I = 5.22;
    double t = atof(argv[1]);     // total time (from command line)
    int repete = atof(argv[2]);   // number of iterations
    double h = t / repete;        // time step
    double b = 1.7;

    printf("t = %f --- rep = %d\n", t, repete);

    // data files 
    FILE *pd = fopen("phase_.maxdat", "a+");
    FILE *pm = fopen("phase_min.dat", "a+");

    // set large buffer to reduce I/O overhead
    char *file_buffer = malloc(FILE_BUFFER_SIZE);
    setvbuf(pm, file_buffer, _IOFBF, FILE_BUFFER_SIZE);

    // OpenMP
    int num_threads = 10;
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        // each thread scans part of the (b, I) parameter space
        for (j = 0; j < 1400; j++) {
            for (i = 0; i < 5000; i++) {
                runge(h, repete, b, I, pm, pd);
                b += 0.00005;
            }

            b = 1.7;
            I += 0.000025;
        }
    }

    // flush 'nd close 
    fflush(pm);
    fflush(pd);
    fclose(pd);
    fclose(pm);

    return 0;
}
