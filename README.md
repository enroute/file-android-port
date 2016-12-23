# file-android-port
The linux 'file' command ported to android

# How to build
1. Download file-5.29 source code at
ftp://ftp.astron.com/pub/file/file-5.29.tar.gz

put it in the `jni/src' directory and build it under linux to generate
config.h and related files:
```
./configure
make
```

2. Use xxd to generate the magic data
```
cat magic/Magdir/* > all_magic
# or use awk with extra blank lines
awk 'FNR==1{print "";print ""}{print}' magic/Magdir/* > all_magic

xxd -i all_magic > all_magic.h
```

or use the generated `file' command to create compiled magic file:
src/file -C -m all_magic
which should create the compiled magic file all_magic.mgc

The following steps 3 and 4 are optional, which targets at capturing
the output of `file' command.

3. Change the `main' function to `file_main' in file.c

4. Create a wrapper to capture the output of original `file' command

5. Build it with Android ndk-build