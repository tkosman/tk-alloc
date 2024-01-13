<h2 align="center">
  <br>
 <img src="img/tk-alloc.png" alt="Heap allocator" width="200">
  <br>
  Heap Memory Allocator by <a href="https://github.com/tkosman">tkosman</a>
  <br>
</h2>

<div align="center">

   <a href=""> ![example workflow](https://github.com/tkosman/tk-alloc/actions/workflows/ci.yml/badge.svg) </a>
   <a href=""> ![GitHub top language](https://img.shields.io/github/languages/top/tkosman/tk-alloc) </a>
   <a href=""> ![GitHub language count](https://img.shields.io/github/languages/count/tkosman/tk-alloc) </a>
   
</div>

## Installation

First, clone the repository:

```bash
  git clone https://github.com/tkosman/tk-alloc.git
  cd tk-alloc
```
Next, execute the installation script and specify your desired destination folder:
```bash
  ./install_lib.sh {DESTINATION_FOLDER}
```
During the installation process, you will be prompted to confirm the installation of required packages:
```
Do you want to install all necessary packages for the lib? (y/n)
``` 
Please note that you will need to enter your sudo password if agreed.

**Warning:**
If a `tk-alloc` folder already exists in the destination, the script will ask whether to rebuild it or exit. Respond as needed to proceed. Be aware as your data may be removed.
## Usage/Example

#### Code usage example:
*Warning: replace {CAPS} with values.*
```C
  #include <{PATH_TO_LIB}/include/heapAllocator.h>

  int main(void)
  {
    atexit(checkForUnfreedChunks); //to check if all mem is freed at program exit

    void *ptr = heapAlloc(10);     //to alloc
    heapFree(ptr);                 //to free

    printMemoryStats();            //to print all collected stats
  }
```

#### Compilation:
```bash
  gcc {SOURCE *.c FILES} -I{PATH_TO_LIB}/lib/include -L{PATH_TO_LIB}/lib -l-tk-alloc -o {DESIRED_OUTPUT}
```


## Running Tests and Analysis

To run both unit and e2e tests, run the following command:

```bash
  make test
```

For code analysis:
```bash
  make analyze
```

To clean `/target`:
```bash
  make clean
```

To do clean, build, test and analyze:
```bash
  make regression
```

## License

The code is licensed under the [MIT License](https://choosealicense.com/licenses/mit/). \
This license permits free use, modification, and distribution, with limitations on liability and warranty.
