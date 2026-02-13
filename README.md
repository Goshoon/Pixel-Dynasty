<h1><b>Pixel Dinasty - Sandbox </b></h1>
        Small sandbox game made using C++ and SDL2, based in games lie "Powder" and "The Sandbox".

<b><h2> Features </h2></b>
        <ul>
            <li> Place Static and Dynamic pixels </li>
            <li> Use materials such as Water with basic fluid mechanics or dirt </li>
            <li> Swap colors for dynamic and static pixels </li>
            <li> Chnage background color </li>
            <li> Save and load and share your levels! (Feature not implemented yet)</li>
        </ul>

<h2><b> Usage </b></h2>
        Before running the app place <b>Resources<b> folder in the Bin folder, or anywhere where the executable is creted, after that just run the application.


<b><h2>Compile</h2></b>
<h3>Linux</h3>
        On Linux, you can use `make` to build the project. This will place everything in the `Bin` folder.

<h3>Windows</h3>
        On Windows, you have two options to build the project::

1. **Manual Build**
           - Build the project manually using `g++`.
           - Place the required files (SDL2 .dll files and the **Resources** folder containing assets) in the appropriate directories.

2. **Automated Build (using `build.bat`)**
           - Use my script build.bat, which uses MinGW to build the project and automates most tasks.
           - **Requirements**: You need to have `SDL2/include` and `SDL2/lib` directories in a folder named **Dependencies** on your local `C:` drive. You can also modify the `DEFAULT_SDL2` to point to a different folder if your SDL2 files are located elsewhere.
           - This script also embeds descriptions, version information, and a game icon into the executable (`PixelDynasty.exe`), making it ready to be shipped.

In the end, you should have a Bin folder containing the Resources folder along with all the necessary .dll files.

<h2>Credits</h2>
        <ul>
                <li> SDL2: The Simple DirectMedia Layer (SDL) is used for handling graphics, events, and input. SDL is licensed under the [Zlib License](https://www.libsdl.org/license.php). </li>
                <li> Dear ImGui: Dear ImGui is used for creating the GUI elements in the project. It is a bloat-free graphical user interface library primarily used for debugging tools and applications. ImGui is licensed under the [MIT License](https://github.com/ocornut/imgui/blob/master/LICENSE.txt). </li>
                <li> C++ Standard Library: The C++ Standard Library is used for many common operations like memory management, data structures, and more. </li>
        </ul>

<h2> <b> License </b> </h2>
        This project is licensed under the AGPLv3 - see the [LICENSE](LICENSE) file for details.
