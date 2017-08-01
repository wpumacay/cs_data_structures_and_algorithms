
To compile this project just use the cmake file provided.

1) Go to "this" path ( rooted were the CMakeLists.txt file is located )
2) cmake . ( if cmake is not installed, run sudo apt-get install cmake )
3) make
4) ./cs_ds_project

The executable consists of a basic console interface. Press the options to test the solver.

If you want to test the Qt visualizer, just uncomment the USE_QT flag and comment the NO_GRAPHICS flag in the main.cpp file. Open the .pro file with qt-creator and run the application.