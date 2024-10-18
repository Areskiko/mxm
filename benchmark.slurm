#! /usr/bin/bash
#SBATCH --job-name="GEMM Benchmark"
#SBTACH --account=share-ie-idi
#SBATCH --partition=CPUQ
#SBATCH --time=24:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=28
#SBATCH --cpus-per-task=1
#SBATCH --mem=200G
#SBATCH --exclusive
#SBATCH --output=benchmark.log

# The lines above here are instructions for the batch scheduler

## Explanations of the SBATCH directives
# --job-name        : String that appears in the queue listing
# --account         : Send the bill to this institute (for us, it's IDI)
# --partition       : CPUQ, GPUQ, or short
# --time            : Job will be killed if it takes longer than HH:MM:SS
# --nodes           : How many separate machines to start on
# --ntasks-per-node : How many MPI ranks per machine
# --cpus-per-task   : How many threads per MPI rank

# The job commands start here:

# Go to the working directory which has this program in it
# (batch jobs launch directly under your home directory by default)
cd ${SLURM_SUBMIT_DIR}

# Let's use the Intel compilers today
module load GCC/13.3.0

mkdir -p /localscratch/mxm_workdir
mkdir -p /localscratch/mxm_datadir

# Build the program
export CC="gcc"
make clean benchmark

echo "" > benchmark_results.txt
for file in **/result.txt; do echo $file >> benchmark_results.txt; cat $file >> benchmark_results.txt; done
