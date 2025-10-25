#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


#define FILE_BUFFER_SIZE (100 * 1024 * 1024)

void runge (double h, int repete, double b, double I, FILE *pm, FILE *pd){

    //setup GNUPLOT
    double time = 0.0; // variable for writing on data txt and getting it to gnuplot


    //model fixed parameters
    double a = 1.0;
    double c = 1.0;
    double d = 5.0;
    double s = 4.0;
    double r = 0.05;
    double Xr = -1.6;

    //these variables are our x, y and z functions 
    double x, y, z;
    double isi;
    double isi_max = 0;
    double isi_min = 200000;

    //RK4 method
    int i;
    //array that stores time value for ISI calculation
    double tj[2];
    tj[0] = time;
    //initial conditions
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;
    x = x0;
    y = y0;
    z = z0;
    //ODEs 
    double Dx = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
    double Dy = c - d * pow(x, 2.0) - y;
    double Dz = r * (s * (x - Xr) - z);
    //starting loop
    for (i = 0; i < repete; i++){
        double k1 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l1 = c - d * pow(x, 2.0) - y;
        double m1 = r * (s * (x - Xr) - z);
        x = x0 + h * k1/2.0;
        y = y0 + h * l1/2.0;
        z = z0 + h * m1/2.0;
        double k2 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l2 = c - d * pow(x, 2.0) - y;
        double m2 = r * (s * (x - Xr) - z);
        x = x0 + h * k2/2.0; 
        y = y0 + h * l2/2.0;
        z = z0 + h * m2/2.0;
        double k3 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l3 = c - d * pow(x, 2.0) - y;
        double m3 = r * (s * (x - Xr) - z);
        x = x0 + (h * k3);
        y = y0 + (h * l3);
        z = z0 + (h * m3);
        double k4 = y - a * pow(x, 3.0) + b * pow(x, 2.0) - z + I;
        double l4 = c - d * pow(x, 2.0) - y;
        double m4 = r * (s * (x - Xr) - z);
        x = x0 + h/6.0 * (k1 + 2.0*k2 + 2.0*k3 + k4);
        y = y0 + h/6.0 * (l1 + 2.0*l2 + 2.0*l3 + l4);
        z = z0 + h/6.0 * (m1 + 2.0*m2 + 2.0*m3 + m4);
        time += h; //update time variable on gnuplot
	if (time > 5000){
        if((x0 * x) < 0 && (x < x0)){
            tj[1] = time;
            isi = tj[1] - tj[0];
	    tj[0] = tj[1];
		if (isi > isi_max){
		isi_max = isi;
} 
		if (isi < isi_min){
		isi_min = isi;
} 
			}
				}
        x0 = x;
        y0 = y;
        z0 = z;
    }
fprintf(pd, "%.16f %.16f %.16f\n", b, I, isi_max);
  if (isi_min < 5000){
  fprintf(pm, "%.16f %.16f %.16f\n", b, I, isi_min);
  }
  else{
  fprintf(pm, "%.16f %.16f 0\n", b, I, isi_min);
  }
}

int main(int argc, const char * argv[]){
    int i;
    int j;
    double I = 5.22;
    double t = atof(argv[1]);
    int repete = atof(argv[2]);
    double h = t/repete;
    double b = 1.7;
    printf("t = %f --- rep = %d\n", t, repete);
    FILE *pd = fopen("prl_max.dat", "a+");
    FILE *pm = fopen("prl_min.dat", "a+");
   char *file_buffer = malloc(FILE_BUFFER_SIZE);
   setvbuf(pm, file_buffer, _IOFBF, FILE_BUFFER_SIZE);	   
   int num_threads = 10;
   omp_set_num_threads(num_threads);
   #pragma omp parallel
   {
    for (j=0; j<1400; j++) {
    for (i = 0; i<5000; i++){
        runge(h, repete, b, I, pm, pd);
        b += 0.00005;
    }
b = 1.7;
I += 0.000025;
}
}
	fflush(pm);
	fflush(pd);
	fclose(pd);
	fclose(pm);

    return 0;
}
