# vprocess

Little tool / toy project that uses psapi (windows) to enumerate
all processes accessible by the current user along
with their memory consumption.

## How to build and run
```bash
mkdir build
cd build
cmake ..
# linux
make
# windows
msbuild vprocess.sln
```