# Yadro_BaseStations-DSS

## How do I build a project on Windows?

1. Clone the repository
   ```
   https://github.com/Semyon1104/Yadro_BaseStations-DSS.git
   ```
   and
   ```
   cd Yadro_BaseStations-DSS
   ```
3. Update the Google Test submodule
   ```
   git submodule update --init --recursive
   ```
4. Configure the project: Create a separate directory for configure the project and compile it:
   ```
   mkdir build
   cd build
   cmake ..
   ```
5. Build the project: Next, to build the project, we will need to enter the command
   ```
   cmake --build . --config Release
   ```
6. Run the project after building the project, you can find example of the system in the following path from the build folder
   ```
   cd app/Release
   ```
   and run the file
   ```
   computer_club_app.exe ../../../input/input.txt
   ```
   Where input.txt - a file with input data of the system located in the input folder
## How an I test a project on Windows?

This project contains tests to verify functionality. To test the project, the Google Test Framework is used as a submodule of the project.

To start the testing process locally, you need to go to the directory
```
cd build/test/Release
```
and run the following file:
```
computer_club_tests.exe
```

## How do I build a project on Linux?

1. Clone the repository
   ```
   https://github.com/Semyon1104/Yadro_BaseStations-DSS.git
   ```
   and
   ```
   cd Yadro_BaseStations-DSS
   ```
3. Update the Google Test submodule
   ```
   git submodule update --init --recursive
   ```
4. Configure the project: Create a separate directory for configure the project and compile it:
   ```
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ ..
   ```
5. Build the project: Next, to build the project, we will need to enter the command
   ```
   make -j$(nproc)
   ```
6. Run the project after building the project, you can find example of the system in the following path from the build folder
   ```
   cd app/Release
   ```
   and run the file
   ```
   ./app/computer_club_app ../input/input.txt
   ```
   Where input.txt - a file with input data of the system located in the input folder
## How an I test a project on Linux?

This project contains tests to verify functionality. To test the project, the Google Test Framework is used as a submodule of the project.

To start the testing process locally, you need to go to the directory
```
cd test
```
and run the following file:
```
./computer_club_tests
```
