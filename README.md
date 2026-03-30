# DA2026_PRJ1_T16_G01
This is the project completed by group 01 of of class 16 for DA in 2026. Here you will find detailed steps on how to build and run the project.

## Building the project
open the terminal and verify you are in the root of the project. If so type in the following commands: 
```bash
mkdir build
cd build
```
From here well use CMake as our build System simply type:
```bash
cmake ..
cmake --build .
```

Now the project should build. We have configured it so any executables get created in the bin/ folder from root.

## Running our project
Our project supports two modes; manual and batch. We'll go further into detail on how to run both

### Batch mode
This is the simplest and more immediate way of executing our project. Again make sure you are at the project root.
To run in batch mode when calling the executable you'll need to provide it with the arguments as follows:
```bash
./bin/DA2026_PRJ1_T16_G1.exe -b inputFilePath.csv outputFilePath.csv
```
NOTE: This command assumes you are located at the root of the project
- **-b** tells the program to run in batch mode without any human intervention and thats why the other two arguments are necessary.
- **inputFilePath.csv** is the full path from root to your input file, you can use for example data/input/dataset1.csv
- **outputFilePatch.csv** likewise this represents the file to wich the output is generated in.

### Manual mode
Running the executable without providin any additional arguments will initiate manual mode. Manual mode provides a simple CLI through wich you can interact with the algorithm directly. A built in *-help* command exists to clarify any doubts.
Let's walt through an example.
```bash
./bin/DA2026_PRJ1_T16_G1.exe
```
this will trigger manual mode after which you'll be greeted by the CLI. Let's try tout the following commands:
```bash
$ load data/input/dataset1.csv
```
This will cause the algorithm to load and parse the input form a file.
NOTE: this command is mandatory at least once before any computation takes place, failure to do so leads to unpredicatble results.
```bash
$ run
```
A simple *run* is enough to calculate and generate output as our program will base the parameters off the previously loaded file. Manual Mode also has some real time features to tweak on the go but ill let you explore those with the *-help* command on your own.