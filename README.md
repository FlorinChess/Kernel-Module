# Linux Kernel Modules

This repository contains a set of linux kernel modules of different levels of complexity containing various binary exploitation vulnerabilities. The main purpose of these modules is to test the taint tracking functionality of a static analysis compiler pass implemented in LLVM.

## Recommended environment for testing

Any x86-64 Linux distribution.

## How to build

The intermediate representation of the modules can be built by running the script

```shell
./compile_modules.sh <OUTPUT_DIRECTORY>
```

where the command line argument `OUTPUT_DIRECTORY` specifies where the compiled .ll files will be stored.
