# ISI phase diagrams of HR model
As shown in the article "Shrimp hubs in the Hindmarsh-Rose model" (https://doi.org/10.1063/5.0239268), co-autored by Rafael V. Stenzinger, Vinícius L. Oliveira and Marcelo H. R. Tragtenberg, the HR model presents shrimp-like periodic structures in its phase diagram. The code presented in this repository was used to produce the results in this article, and can be modified to generate plots such as the ones in Figures 4, 6, 7 and 8.

The code is written in C, implementing a 4th order Runge-Kutta method to solve the HR equations:

$$
\dot{x} = y - a x^3 + b x^2 - z + I
$$
$$
\dot{y} = c - d x^2 - y
$$
$$
\dot{z} = r [s (x - x_r) - z]. \\
$$

The interspike interval (ISI) phase diagrams are produced by taking steps in $(b, I_e)$ parameter and analyzing the ISI of the time series after discarding the transient. The minimum and maximum values of ISI for a point in the parameter space is registered and used as a color map. 

## Code implementation details
- The code is written in C and uses OpenMP to multithread, and run different lines of the phase diagram in parallel. Use -fopenmp flag when compiling and adapt thread number to your system.
- In order to reduce fprintf calls, and better save runtime, buffer sizes were set so that the data file is written "in batches". Change this to your preference.
- Bash script build_diagram.sh compiles, runs the C file with a time discretization and plots the phase diagram. In order to change time interval and number of iterations, change in this bash script.
- Plot details in GNUPLOT file.
- In order to compile, either run build_diagram.sh or use:
```
gcc hr_code.c -march=native -O2 -lm -o HR -fopenmp
```
