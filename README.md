# ISI phase diagrams of HR model
This code produces interspike interval (ISI) phase diagrams by taking steps in $(b, I_e)$ parameter and analyzing the ISI of the time series after discarding thetransient. The minimum and maximum values of ISI for a point in the parameter space is registered and used as a color map. 
The code is written in C, implementing a 4th order Runge-Kutta method to solve the HR equations:

$$
\dot{x} = y - a x^3 + b x^2 - z + I, \\
\dot{y} = c - d x^2 - y, \\
\dot{z} = r [s (x - x_r) - z]. \\
$$
