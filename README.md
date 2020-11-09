# Mathy-Compiler

This is a compiler for a specific language that consists of consisting of mathematical constructs like _Forall_, _Summation_ etc., 
written in C++ using Bison and Flex.

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
./test filename [-o output_filename] [-t data tpe for variables]
```

Please refer to the [wiki](https://github.com/adharshkamath/Mathy-Compiler/wiki) for the Grammar, Syntax and currently supported features of the project.

This project was done under the guidance of [Dr.Rupesh Nasre](https://www.cse.iitm.ac.in/~rupesh)