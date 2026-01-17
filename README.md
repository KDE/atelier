Atelier - KDE Printer Host for 3D Printers
---

Atelier has the goal to be the best open source printer host to control
3D printers from your computer on any platform: Linux, Windows and MacOS.
This project is under GPL V3, for more information read COPYING.GPL3.

Atelier depends on [AtCore](https://invent.kde.org/libraries/atcore), the API to manage the serial connection between the computer and 3D Printers/CNC.
AtCore is also under development by team Atelier.

---
### AtCore Supported Firmwares
 - Repetier
 - Marlin
 - Teacup
 - APrinter
 - SPrinter
 - Smoothie
 - Grbl
---
### Atelier Features
Atelier gives to you a user friendly interface to control and manage your printer.

- 3DView: So load and see your GCode File
- Temperatures Timeline: A real time graphic to monitor your bed and hotend temperatures
- Log: You can log everything that comes and go from your printer in a file to check if everything is ok.
- GCode Editor: A complete editor inside Atelier to make editions on your gcode file.

> Atelier is under development, beta version coming out soon.
---
#### Atelier is powered by [Qt](qt.io) and [KDE Frameworks](https://api.kde.org/frameworks/)
---
### Build Atelier for Development
##### Minimal Qt Version: 6.7

Dependencies for Atelier:
 - Build and install AtCore, see AtCore README to check the build information.
 Qt Libs
 - QtBase
 - QtCharts
 - QtSerialPort
 - Qt3D

 KDE Frameworks
 - KI18n
 - KXMLGui
 - KConfigWidgets
 - KTextEditor
 - Extra-Cmake-Modules
 ----
 #### Building on Linux
 ```bash
 $ git clone https://invent.kde.org/utilities/atelier.git
 $ cmake atelier
 $ make
 $ make install
 ```
---
### Getting in Touch
You can reach us via: <br/>
[IRC] - Libera Chat #kde-atelier <br/>
[Matrix] - KDE Atelier room <br/>
---
### Bugs
Please report any bugs to our [Bug Tracker]. Please remember to include as much information about your issue and provide a backtrace if possible.

[IRC]: https://web.libera.chat/
[Matrix]: https://matrix.to/#/#kde-atelier:kde.org
[Bug Tracker]: https://bugs.kde.org/enter_bug.cgi?product=Atelier
