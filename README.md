# Sysinfo

## Overview

This is a C++ application to get information about FW, AV, updates and hardware.

## Usage

By default, program shows all the information. But the following options are available:

1. Configure, what info you want to see with flags.
    
    For example to see only cpu and firewall info run
    ```
    sysinfo.exe --cpu --fw
    ```
    
    To see everything except cpu and firewall info run 
    ```
    sysinfo.exe --no-cpu --no-fw
    ```

    For more information about available flags run program with `-h` or `--help` option.


2. Get information in `json` format:
    ```
    sysinfo.exe --json=<output filename>
    ```

3. Get size of a directory or a file (via `-s` or `--size` flag):
    ```
    sysinfo.exe -s <filename> -s <directory name> ...
    ```

## Requirements

* `Windows` system
* `CMake 3.16` or higher
* `boost 1.65.1` or higher **with compiled binaries**
* `MSVC 19.29` or higher

## Build

Configure `CMakeLists.txt` in project directory by setting `BOOST_ROOT` variable to path, where the boost_"ver" directory is located. For example:
```cmake
set(BOOST_ROOT "C:/")
```

Then run following commands:

```
mkdir build
cd build
cmake ..
```

Finally, build `sysinfo.sln` file with `msbuild` or `Visual Studio`.