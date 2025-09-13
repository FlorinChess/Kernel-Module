#!/bin/bash

# if argument number not equal to 1, er
if [ "$#" -ne 1]; then
    echo "Usage: ./compile_mods.sh <output_directory>"
    exit -1
fi

OUTPUT_DIRECTORY="./llvm-modules"
SOURCE_DIRECTORY="./src"

TARGET_ARCHITECTURE="x86_64-pc-linux-gnu"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIRECTORY"

# Loop through each .c file in the source directory
for SOURCE_FILE in "${SOURCE_DIRECTORY}"/*.c; do
    
    # Get the filename without the path and extension
    FILENAME=${SOURCE_FILE##*/}
    FILENAME_WITHOUT_EXTENSION=${FILENAME%.c}
    
    OUT_FILE="${FILENAME_WITHOUT_EXTENSION}.ll"

    # Compile the file with clang
    clang -S -emit-llvm "${SOURCE_FILE}" -o "${OUTPUT_DIRECTORY}"/"${OUT_FILE}" \
      -I/usr/src/linux-headers-$(uname -r)/arch/x86/include \
      -I/usr/src/linux-headers-$(uname -r)/arch/x86/include/generated \
      -I/usr/src/linux-headers-$(uname -r)/include \
      -I/usr/src/linux-headers-$(uname -r)/arch/x86/include/uapi \
      -I/usr/src/linux-headers-$(uname -r)/arch/x86/include/generated/uapi \
      -I/usr/src/linux-headers-$(uname -r)/include/uapi \
      -I/usr/src/linux-headers-$(uname -r)/include/generated/uapi \
      -I/usr/src/linux-headers-$(uname -r)/ubuntu/include \
      -D__KERNEL__ -DMODULE \
      -include /usr/src/linux-headers-$(uname -r)/include/linux/compiler-version.h \
      -include /usr/src/linux-headers-$(uname -r)/include/linux/kconfig.h \
      -include /usr/src/linux-headers-$(uname -r)/include/linux/compiler_types.h \
      -target "${TARGET_ARCHITECTURE}" \
      -fno-discard-value-names \
      -O0 -fno-rtti -fno-exceptions -fno-asynchronous-unwind-tables \
    
    # Check if compilation succeeded
    if [ $? -eq 0 ]; then
        echo "Compiled $SOURCE_FILE -> $OUTPUT_DIRECTORY/$OUT_FILE"
    else
        echo "Failed to compile $SOURCE_FILE"
    fi
done
