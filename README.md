# Mathy Compiler

This is a compiler to generate OpenMP programs for equations involving constructs like _Forall_, _Summation_ etc., frequently used in the scientific domain.

This project is written in C++ using Bison and Flex.

## How to use

Compile this project:
```
cd source
make install
```

Open a file and write an equation like this:

```
mean = sigma(a[i]/100) where 0<=i<=100
```

Or if you like Unicode characters, 
```
mean = Σ(a[i]/100) | 0<=i<=100
```

To generate OpenMP program for the equation:
```
./test filename [-o output_filename] [-t data type for variables]
```

The equations in [benchmark/source](./benchmark/source) are implementations of programs from [PolyBench/C](https://web.cse.ohio-state.edu/~pouchet.2/software/polybench/).

The OpenMP outputs for these equations can be found in [benchmark/output](benchmark/output).

To compile all the source files in [benchmark](./benchmark/), from the root directory, run
```
python3 benchmark.py
```

Please refer to the [wiki](https://github.com/adharshkamath/Mathy-Compiler/wiki) for the Grammar, Syntax and currently supported features of the project.

This project was done under the guidance of [Dr. Rupesh Nasre](https://www.cse.iitm.ac.in/~rupesh)
