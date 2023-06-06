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

The code Subscribes to the cmd_vel topic to get the velocity information converting it into Left and right motor velocities and figuring out the Required step delay by taking a ratio of a specific step delay which experimentally resulted in a specific speed (Refer to the video at 5:05)

StepDelayReq = (RandomStepDelay * SpeedAtThatRandomStepDelay)/SpeedRequired

It also turns off the motor drivers for both motors when none are supposed to move as of now as otherwise the motors would waste power and get hot if they kept maintaining their position when the robot dosent move (Idling).

Thats About it for the Arduino Code!




ROS:
I recommend watching some videos regarding ROS to better be able to understand this part But i'll try my best to explain everything i've done here.
Since this is my first time working with ROS too , there might be mistakes here so feel free to let me know if you find any!

Here are some channels I watched :

Tiziano Fiorenzani's ROS Tutorials - https://www.youtube.com/watch?v=N6K2LWG2kRI&list=PLuteWQUGtU9BU0sQIVqRQa24p-pSBCYNv

James Bruton's 2 Part series - https://www.youtube.com/watch?v=q1u_cC-5Sac


Intro to ROS - Now basically as told in the Video ROS is the framework or a platform with a lot of the code for major tasks of robots (Such as SLAM , Navigation , Path planning) pre written that are easy to implement .
Typically ROS Runs on Ubuntu platforms , in my case I just used the normal version of Ubuntu 20.04 , as 20.04 is the version ROS Noetic is made for.
All of your projects are stored as packages in your workspace called catkin workspace , this is where you have your scripts , launch files , parameters etc.
Most of the things in ROS are run from terminal commands and some tools come with GUI like RVIZ Which is what you would use for visualizing pretty much anything for any project.

Now you can install ROS from this link - http://wiki.ros.org/noetic/Installation/Ubuntu

(Still being written Check back later for updates!)
