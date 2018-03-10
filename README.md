# TrayScriptLauncher
A C++ Win32 application that provides a tray icon to let you easily access and launch scripts on windows.

ONLY VALID FOR WINDOWS

Building should be very simple as everything the project relies on is included in the repo (header files for boost property tree, etc).
Open it in visual studio and build it.

The icon and the menu layout xml file gets copied into the executable directory when you build. 
You can edit these files with out needing to recompile as the asset is picked up and used in runtime.

Edit the ScriptMenu.xml to add scripts. The current one is a simple example showing how to nest menu and run scripts.
