# Tetris in C
Pure C Tetris for Linux and Windows (hint: works better in Linux)

| OS      | Enviroment | Support             |
|---------|------------|---------------------|
| Linux   | (C)Make    | Full (used for dev) |
| Linux   | CLion      | Full (used for dev) |
| Windows | CLion      | Full (used for dev) |
| Windows | CodeBlocks | Partial (tested)    |
| Windows | (C)Make    | Should work         |

## Build instructions :hammer:

## Windows/Linux - CLion

* Clone or download the repository (`git clone https://github.com/MarosMacko/Tetris_in_C.git`)
* File -> New CMake project from sources -> Select the folder -> Open existing project
* File -> Reload CMake project (or Tools -> CMake -> Reload CMake project)

## Linux - CMake

* Clone or download the repository (`git clone https://github.com/MarosMacko/Tetris_in_C.git`)
* `cd` into the folder
```
mkdir build
cd build
cmake --target Release ..
make
./Tetris
```

## Codeblocks

* Clone or download the repository (.zip file and unextract)
* Open .cbp file
* Select `Release` target
* Cross your fingers and hit Build (and run)

#### Goals of this project

There are few things that could be done better. Faster. Maybe more interesting, effective, or make more sense. But decisions which are made were for my own academic purposes. Some things are overkill. I didn't want to use any external library for GUI, or similiar tasks. If you have any suggestions, PRs are welcome. 
