# Viscid Framework
This framework allows software written in C++ and using the Simple DirectMedia Layer library to be natively compiled into executables for Windows and Mac OSX. It is intended to be a simple game framework that can enable game developers to create computer games for additional platforms beyond Windows.

## Requirements
At minimum, CMake must be installed. Clang++ is the recommended compiler, and is what the project is currently configured to use.

Since support for 32-bit applications on Mac OSX was removed with MacOS Catalina, the Viscid Framework only supports 64-bit architectures. Due to the libraries and software used, the oldest supported versions of the Windows and macOS operating systems are Windows XP and OSX 10.10 (Yosemite).

While it should be possible to use the framework with other code editors or IDEs, Visual Studio Code and Visual Studio were used in development and therefore are the only ones confirmed to work. Project files can be found in this repository under the .vscode and .vs folders, although it is necessary to change some file paths for your own device. It should also be possible to compile executables directly from the command line using cmake and make.

If Visual Studio is used to build software using the framework on Windows, it is necessary to ship a copy of the Microsoft Visual C++ Redistributables or otherwise inform users that they may need to install it alongside your executable.n
