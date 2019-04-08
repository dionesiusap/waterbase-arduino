# WaterBase Smart Home IoT

## Project Description

Pada sistem arduino menggunakan raspberry pi sebagai gateway. Gateway ini berguna untuk menghubungkan antar arduino dan antara arduino dam android. Arduino menerima input dari sensor IR dan DHT11. Hasil dr IR digunakan untuk menyalakan lampu sedangkan hasil dr DHT11 untuk menyalakan A/C. Arduino juga memiliki lcd dan 7 segment untuk menampilakan status dari keadaan IOT pada rumah.


## Installation

### Dependencies

- [Robot Operating System (ROS), Kinetic Version](http://wiki.ros.org/kinetic) (middleware for robotics, see installation manual),
- [rosserial] (ROS serialized messages wrapper)
- [rosserial_client] (generalized client side source for rosserial)
- [rosserial_python] (Python-based implementation of rosserial)

		sudo apt-get install ros-kinetic-rosserial ros-kinetic-rosserial-client ros-kinetic-rosserial-python
- [arduino_msgs](https://gitlab.informatika.org/if3111-2019-waterbase/ros-packages/arduino_msgs) (package containing custom messages used by Arduino)

### Building

#### ROS Workspace

The system runs in a ROS environment. Create a workspace for the system using

    mkdir -p ~/<your_workspace>/src
    cd ~/<your_workspace>
    source /opt/ros/kinetic/setup.bash #or setup.zsh if you're using zsh
    catkin_make
    source devel/setup.bash #or setup.zsh if you're using zsh

> note: every new terminal must be sourced by using `source /opt/ros/kinetic/setup.bash` and `source devel/setup.bash`

Then in `src` directory, clone every package from [here](https://gitlab.informatika.org/if3111-2019-waterbase/ros-packages). You should now have 3 folders in `src` directory, one for each package with corresponding names. Then, on your workspace directory (`~/<your_workspace>`), build the workspace by using

    cd ~/<your_workspace>
    catkin_make
    source devel/setup.bash #or setup.zsh if you're using zsh

#### Arduino

Simply clone this directory and open to Arduino sketchbook directory (Linux default is `~/Arduino`) using

    git clone https://gitlab.informatika.org/if3111-2019-waterbase/arduino.git
    cd arduino
    mkdir ~/Arduino/actuators && mkdir ~/Arduino/sensors
    cp actuators.ino ~/Arduino/actuators/
    cp sensors.ino ~/Arduino/sensors/

Now generate ROS library for Arduino. Go to your ROS workspace directory, generate the library, and copy them to Arduino libraries folder (Linux default is `~/Arduino/libraries`) using

    cd ~/<your_workspace>
    rosrun rosserial_client make_libraries . arduino_msgs
	cp -R ros_lib ~/Arduino/libraries/

Next, launch Arduino IDE and open `actuators.ino` and `sensors.ino`. Now, connect the Arduino for sensors to `/dev/ttyACM0` and the Arduino for actuators to `/dev/ttyACM1`. Upload each `.ino` files to corresponding Arduinos.

#### Android

Simply go to the [project repository](https://gitlab.informatika.org/if3111-2019-waterbase/mobile) and follow the instructions.


## System Setup

1. Connect the Arduino for sensors to `/dev/ttyACM0` and the Arduino for actuators to `/dev/ttyACM1`.
2. Open 3 terminals (or, if you're fancy, you can use terminal multiplexer like `tmux`, whatever), then on each terminal type in the following commands.

        cd ~/<your_workspace>
        source /opt/ros/kinetic/setup.bash #or setup.zsh if you're using zsh
        source devel/setup.bash #or setup.zsh if you're using zsh

3. On the first terminal run roscore by using

        roscore

4. On the second one, run the API server node using

        rosrun network_api network.py

5. On the third terminal, launch the hardware interface nodes by typing in

        roslaunch hardware_controller hardware_interface.launch

And you're all set.


## Usage

1. Switching devices on and off
    > You can open the Android app, go to Home IoT tab and tap the lights or A/C that you want to switch on or off.

2. Checking the status of the system
    > The system control panel (connected to the Arduino for sensors) has one push button, an LCD screen, and a 4-digits seven-segments display. The default message on the LCD screen is a greetings message. Push the button once and it will switch to show the temperature measurement. Push once again, it will show the humidity measurement. Push it once more and it will be showing back the greetings message. The seven-segments display always shows the current temperature reading.


[rosserial]: http://wiki.ros.org/rosserial
[rosserial_python]: http://wiki.ros.org/rosserial_python?distro=kinetic
[rosserial_client]: http://wiki.ros.org/rosserial_client?distro=kinetic