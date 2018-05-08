# Planes
### Disclaimer - most likely you are not interested in this project. It's just an educational code snippet written as a homework
This project is a 2D graphics and database demo that was developed as a university homework (and because of that it will not be developed further). Demo consists of a different 2D planes flying on the static map. Planes may be created and destroyed using a mouse and the basic interface. Generic information about planes and plane sessions is stored in the database and can be viewed via a simple dialog. Project uses wxWidgets library to draw graphics and UI as well as SQLite3 to manage the database. (wxWidgets is used because it provides both graphical toolkit and possibility to draw on canvas, SQLite is just a convinient database engine).
## Motivation
Project itself has no practical purpose (however it has successfully completed its task and I've got a highest mark), so motivation may seem unclear. Firstly, I just do not want to lose this code. Secondly, it seems useful as a "snippet" of the minimal wxWidgets application (e.g. to refresh my memory in the future) and SQLite3 C API.
## Building
Firstly you should find plane sprites and a map (PNG recommended). Copy them into the 'data' directory and edit 'src/PlanesApp.cpp' - file with plane model definitions.

Then add the path to SQLite library to the PATH variable (on Windows) and execute this in the project directory:
```bash
mkdir bin
cd bin
cmake ..
cmake --build .
```
Then you'll have an executable placed in the 'bin/src/planes'. Run it:
```bash
./src/planes ../data planes.db # executable [data directory] [database file]
```
Both arguments may be omitted - 'open file/directory' dialogs will appear.
## License
Author: Jevgēnijs Protopopovs
The project (source, build files) is distributed under the terms of MIT License.
This product includes software developed as part of:
* https://github.com/LuaDist/libsqlite3/blob/master/cmake/FindSQLite3.cmake