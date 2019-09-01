# C++ Raft

A Raft consensus library in C++.

## Getting Started

These instructions will get you a copy of the project up and running on your 
local machine for development and testing purposes. 

See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

 - gcc
 - [cmake](https://cmake.org/install/)

### Installing

Create the build folder.

```
$ mkdir ./build
```

Go into build folder.

```
$ cd ./build
```

Build the project

```
$ cmake .. && make -j2
```

## Running the tests

To run tests, stay in the `build` folder.

```
$ ctest 
```

To run tests in verbose

```
$ ctest -VV
```

## Deployment

**TODO**

## Built With

* c++11
* gcc
* cmake

## Authors

* **Christopher Coverdale** - *Initial work* - [ccdle12](https://github.com/ccdle12)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
