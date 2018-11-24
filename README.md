# Minesweeper
The classic Minesweeper game written in C++ using the Win32 API. As a result of using this api it only works on Windows. 

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for you to enjoy
### Prerequisites
You need to have Visual Studio with Platform Toolkit v140 installed

### Installing

Download Minesweeper folder which includes source code, .vcxproj file, and .sln file.

Add msbuild.exe to your path.
You do that by finding where MSBuild.exe is and adding the folder to the system path. They tend to be in
```
C:\Program Files (x86)\MSBuild\14.0\Bin\amd64
```
to check that you did this part correctly, in a command prompt type 
```
msbuild /?
```
If it worked then it will display the help page. 


The next step is to find the rc.exe and the rcdll.dll files. They tend to be in
```
C:\Program Files (x86)\Windows Kits\10\bin\x86
```
copy them into your Minsweep folder where all the source code (.cpp and .h files) resides.

go back one level to the parent directory where you will find the file Minesweep.sln then type the following in a command prompt
```
msbuild Minesweep.sln
```

a subfolder will be created called x64 containing the debug folder which would have the Minesweep.exe ready to run

