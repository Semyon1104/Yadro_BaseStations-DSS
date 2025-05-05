# Yadro_BaseStations-DSS

## How do I build a project on Windows?

1. Clone the repository
   ```
   https://github.com/Semyon1104/Yadro_BaseStations-DSS.git
   ```
2. Update the Google Test submodule
   ```
   git submodule update --init --recursive
   ```
3. Configure the project: Create a separate directory for configure the project and compile it:
   ```
   mkdir build
   cd build
   cmake ..
   ```
4. Build the project: Next, to build the project, we will need to enter the command
   ```
   cmake --build . --config Release
   ```
5. Run the project after building the project, you can find example of the system in the following path from the build folder
   ```
   cd build/Release
   ```
   and run the file
   ```
   computer_club_app.exe ../../../input/input.txt
   ```
   Where input.txt - a file with input data of the system located in the input folder
