# sbq-arduino-temperature-chamber
Code to control the temperature chamber with an Arduino

# Platformio Setup
### Installation
1. Open VsCode Extensions Manager and search for PlatformIO IDE then install
2. Once the installation is done you will be prompted to reload VsCode
3. You're all set, a new ant icon should be displayed in the vertical icon bar
4. If you don't want the Platformio home page to open at VsCode startup, you can uncheck the Show at startup option in the top right corner of the Platformio home page
### New project
1. Click on the Platformio ant icon in the vertical icon bar
2. Under PIO Home select open
3. A new window should open to the home page, select New Project in the Quick Access Menu
4. Enter the project name
5. Select the right Board along with the Framework you will be using (you can search for your Board and the Framework should auto select)
6. Select the Location where you want the project to be created
7. Click on finish to create the project
### Import an Arduino IDE project
1. Click on the Platformio ant icon in the vertical icon bar
2. Under PIO Home select open
3. A new window should open to the home page, select Import Arduino Project in the Quick Access Menu
4. Select the right Board
5. Choose the directory containing your .ino file and click import
6. By default, the imported project will be created under Documents\PlatformIO\Projects. Once the project is created, you can manualy move the files under the directory you want them to be by copying them
7. In the .ino file add at the beginning `#include  <Arduino.h>`
8. You will be prompted to rename the .ino file to a .cpp file so just do it because VsCode C/C++ IntelliSense service does not support .INO files
### Setting up the project and Using Platformio
1. Make sure you configure the .gitignore file to ignore folder such as .pio | .vscode and anything else you don't want git to follow. Here's an [article](https://www.freecodecamp.org/news/gitignore-what-is-it-and-how-to-add-to-repo/) on how to use .gitignore file
2. Take a second to look at the platformio.ini file for your project configuration. The only thing you might want to add at first is the monitor_speed option (ex: monitor_speed = 115200) to set your baud rate. For more information on this file, refer to this [page](https://docs.platformio.org/en/latest/projectconf/index.html)
3. All of the command you are used to with Arduino IDE are located in the blue bar at the bottom of the screen
4. The checkmark icon is used to build the project
5. The arrow icon is used to upload the project to the board
6. The plug icon is used to start a serial monitor
7. You can clean the project with the trash can icon
8. The folder icon let's you choose the environnement you want to run (many environments can be configured in the platformio.ini file to use different board for example)
### PlatformIO Unit Testing
Read the README file in the test folder to learn more about unit testing with Platformio
