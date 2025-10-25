# ISI phase diagrams of HR model
This code produces interspike interval (ISI) phase diagrams by taking steps in $(b, I_e)$ parameter and analyzing the ISI of the time series after discarding thetransient. The minimum and maximum values of ISI for a point in the parameter space is registered and used as a color map. 
The code is written in C, implementing a 4th order Runge-Kutta method to solve the HR equations:
$$
\begin{aligned}
& \frac{d x}{d t}=y+\phi(x)-z+I \\
& \frac{d y}{d t}=\psi(x)-y \\
& \frac{d z}{d t}=r\left[s\left(x-x_R\right)-z\right]
\end{aligned}
$$
