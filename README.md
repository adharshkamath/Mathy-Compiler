# Mathy-Compiler
A compiler to generate OpenMP code for mathematical notations like forall, sigma, etc

Build:

```
cd src

make
```
Then
```
./test [-t type] file
```
Where type is one of *float*, *long* or *long_double*

To build the scanner or parser individually:

```
make scanner    # for scanner

make parser     # for parser
```
To remove files:

```
make clean         # to delete all build files
```
