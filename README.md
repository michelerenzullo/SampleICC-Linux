
# SampleICC-Linux
Linux port of SampleICC 1.6.11 (Win ver)

#### Why?
The last version for Linux is v1.6.8 as you can see on [sourceforge](https://sourceforge.net/projects/sampleicc/) , for Win is the 1.6.11. 
I m using it in a simple project where size and performance are crucial so other CMS weren't the first choice for me.
Therefore I've been made a few commits to make it compile correctly also on Linux, the main change is remove the carriage return with dos2unix and copy a couple of missing files
`find . -type f -print0 | xargs -0 dos2unix -ic0 | xargs -0 dos2unix -b`

After clone:
 ```
autoreconf -i
 
- For GCC, dynamic libs by default:

./configure CXXFLAGS="-O3"
make 

- Crosscompile WASM with static libs:

emconfigure ./configure CXXFLAGS="-static -O3"
emmake make
```
