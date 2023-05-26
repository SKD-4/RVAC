# RVAC
All the project files for RVAC (Currently at stage 1)

In this you will find the Arduino Code used , and the ROS Catkin package containing the scripts and other files in it.
I'll cover the important points about both along with helpful links and points for those recreating RVAC or something similar.

NOTE: All the files aren't refined and commented yet so feel free to contact me for any changes or questions.



ARDUINO CODE:
Just a simple code using the ROS Serial arduino library
You'll need to install the required files for both ROS and Arduino IDE
by following the process in this article : https://medium.com/robotics-weekends/ros-vl53l0x-range-sensor-based-on-arduino-mega-2560-57922804a419 (Change Melodic to Noetic)

ROS - sudo apt install ros-noetic-rosserial ros-noetic-rosserial-arduino


Arduino - "For installation of rosserial headers for Arduino IDE I’d recommend to generate the libraries in a separate directory and then copy result to Arduino libraries.
This will reduce probability of breaking existing libraries structure."

  mkdir ~/rosserial_libs
  cd ~/rosserial_libs
  rosrun rosserial_arduino make_libraries.py .
  
"The last command will generate ros_lib directory, which contains ROS message and ROS API headers. Now it’s time to copy generated ros_lib directory to the Arduino libraries folder.
By default it is sketchbook/libraries in your home directory."

The code Subscribes to the cmd_vel topic to get the velocity information converting it into Left and right velocities and figuring out the Required step delay by taking a ratio of a specific step delay for a specific speed

StepDelayReq = (RandomStepDelay * SpeedAtThatRandomStepDelay)/SpeedRequired

(Still being written Check back later for updates!)
