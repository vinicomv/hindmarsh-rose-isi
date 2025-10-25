#!/bin/bash
# -------------------------------------------------------------------
# Hindmarsh–Rose ISI Phase Diagram Builder
# -------------------------------------------------------------------
# --- Simulation parameters ---
DT=10000          # Time discretization (or integration step)
N_ITER=1000000    # Number of iterations
SRC=hr_code.c     # Source file
EXEC=HR           # Output binary name
PLOT_SCRIPT=plot_diagram.plt
OUTPUT_FIG_1=phase_diagram_min.eps
OUTPUT_FIG_2=phase_diagram_max.eps

# --- Compiler configuration ---
CC=gcc
CFLAGS="-march=native -O2 -fopenmp -lm"

# 1. Compile
# -------------------------------------------------------------------
echo "[1/4] Compiling $SRC with OpenMP..."
$CC $SRC $CFLAGS -o $EXEC

if [ $? -ne 0 ]; then
    echo "Compilation failed. Check your C code."
    exit 1
fi

# 2. Clean old data
# -------------------------------------------------------------------
echo "[2/4] Cleaning previous data files..."
rm -f *.dat "$OUTPUT_FIG1" "$OUTPUT_FIG2"

# 3. Run simulation
# -------------------------------------------------------------------
echo "[3/4] Running simulation with DT=$DT and N_ITER=$N_ITER..."
echo "     (Threads are set in hr_code.c or via OMP_NUM_THREADS)"
./"$EXEC" "$DT" "$N_ITER"

if [ $? -ne 0 ]; then
    echo "Execution failed. Exiting."
    exit 1
fi

# 4. Plot results
# -------------------------------------------------------------------
if [ -f "$PLOT_SCRIPT" ]; then
    echo "[4/4] Generating phase diagram with Gnuplot..."
    gnuplot "$PLOT_SCRIPT"
else
    echo "Plot script not found ($PLOT_SCRIPT). Skipping plot step."
fi

echo " Done! Output figure: $OUTPUT_FIG"

