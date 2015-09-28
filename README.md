# Graphs

Remake of my final project for the Programming 3 class at Unitec.

## Changes

  - Created header files.
  - Mapped Win32 API's messages into custom defined classes for easier use.
  - Reorganized code for dialog procedures and event handlers into separate files.
  - Windows Procedures decide which messages to process when recieved **BUT** all logic is encapsulated in the Event Handlers.
  - Removed a bunch of unused and repeated code in the Windows Procedures.
  - Since the code was a lot cleaner and more organized, null checks and other parameter validity checks were added.
  - Upgraded Visual Studio project to VS2013.
  
## TODOs

 - Implement better graph drawings.
 - Add better icons to the top toolbar.
 - Refactor the graph's algorithms. (The code was just copy/pasted from the old project.)
