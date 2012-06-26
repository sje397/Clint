Clint - shared multiple-entry clipboard
---------------------------------------

Usage:

  Clint [-p <portnum>] [-n <node address1> [-n <node address2> ...] ]

The Clint application creates a taskbar icon. Clicking the icon opens a window with a list of previous clipboard
entries, and selecting an item will set it as the current clipboard contents and move it to the top of the list.

The applications also listens on the given port, and sends messages to the node addresses provided whenever the
clipboard contents changes. This keeps the clipboard synchronised across the different nodes. Note that all
connected nodes must use the same port. The messages are sent over UDP.

Clint is written using the Qt C++ toolkit, and works with Windows, Linux, and OSX.

Note to Mac users:

In order to prevent the application from showing in the dock, add the following to the Info.plist file in the application
bundle:

<key>LSUIElement</key>
<string>1</string>


