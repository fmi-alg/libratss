## Introduction
This library snapps points that are ε-close to the sphere to rational points that are no more than 2ε away from the closest point on the sphere. It is based on the paper [
Rational Points on the Unit Sphere: Approximation Complexity and Practical Constructions](https://arxiv.org/abs/1707.08549)

## Dependecies
* gmpxx
* mpfr

## Quickstart guide
### Building the tools
1. git clone --recursive https://github.com/fmi-alg/libratss.git libratss
2. cd libratss && mkdir build && cd build
3. cmake -DCMAKE_BUILD_TYPE=lto ../
4. cd ratsstools && make

#### Snapping points
1. Build the tools
2. ./proj -h

#### Getting sample points
1. Build the tools
2. ./rndpoints -h

#### Snapping sample points
1. Build the tools
2. ./rndpoints -g nplane -f geo -d 3 -n 1000 | ./proj -b -r cf -s plane -if geo -of rational -p 128 -e 53

#### Visualizing points
1. Build the tools
2. ./rndpoints -g nplane -f geo -d 3 -n 1000 | ./proj -b -r cf -s plane -if geo -of rational -p 128 -e 53 -o 1ksnapped
3. gnuplot -e 'splot "1ksnapped" using ($1/$2):($3/$4):($5/$6); pause mouse keypres;'

### Use the library
1. add the repository to your project
2. make sure to add the appropriate compile definitions to your project. If you're using CMake and you're adding libratss as a subdirectory then these are exported. See the CMakeLists.txt for a list of exported variables.
3. add <libratss/ProjectSN.h> or <libratss/ProjectS2.h> as include

# License
libratss is licensed under LGPL v2.1.
Since libratss links with mpfr, mpfr C++, gmp, gmpxx and cgal the resulting binaries are usually governed by the GPL v3
