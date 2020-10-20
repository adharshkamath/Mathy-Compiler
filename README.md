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
make clean_all          # to delete all build files

make clean_scanner      # to delete scanner related files

make clean_parser       # to delete parser related files
```
