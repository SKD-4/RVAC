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

Here are some channels I found helpful :

Tiziano Fiorenzani's ROS Tutorials - https://www.youtube.com/watch?v=N6K2LWG2kRI&list=PLuteWQUGtU9BU0sQIVqRQa24p-pSBCYNv

James Bruton's 2 Part series - https://www.youtube.com/watch?v=q1u_cC-5Sac


Intro to ROS - Now basically as told in the Video ROS is the framework or a platform with a lot of the code for major tasks of robots (Such as SLAM , Navigation , Path planning) pre written that are easy to implement .
Typically ROS Runs on Ubuntu platforms , in my case I just used the normal version of Ubuntu 20.04 , as 20.04 is the version ROS Noetic is made for.
All of your projects are stored as packages in your workspace called catkin workspace , this is where you have your scripts , launch files , parameters etc.
Most of the tools in ROS are run from terminal commands and some of them come with GUI like RVIZ Which is what you would use for visualizing pretty much anything for any project.

Now you can install ROS from this link - http://wiki.ros.org/noetic/Installation/Ubuntu

After that you can follow the ROS Tutorials linked at the end of the page (Also shown in James' first video) and setup the catkin_workspace.
Next up , I describe most of the working behind the project in the video but you can search up any of the nodes to completely understand what they're doing and how to use them.

Most of the code and files from the RVAC ROS Package is from the Nox Robot project , just like in James bruton's series with a major change being how we're dealing with Odometry and Transform.
Since that projects code is made to receive actual odometry input from the wheel encoders compared to the stepper motors I used in RVAC for cost and availability reasons I wrote the different simpler arduino code ,
along with the OdomTf.py script which takes cmd_vel information and calculates odom and tf assuming that the robot moved exactly as it was instructed to.

Which never happens in real life , so this would result in a higher odometric drift , but i went with it anyways to see if I could get away using the precise stepper motors and LiDAR.

Here's some information about some of the files in the package requiring attention-



Cfg

The config folder has files with all the requiered parameters for several things like the path planners. Out of the 3 local planners given I used only the DWA Local Planner which took a lot of experimenting 
with its parameters to get it the path planning and movement of to robot to work properly without any real time sensor input itself. 
It isnt completely Bug free now and still struggles with movements requiring
the robot to move forward in axis before moving backwards again (Like round a door requiring it to go front past the door and get back to the goal position) The workaround I use for now is to split the goal positions into 2 (One forward and one backward)
In the DWALocalParameters File some of the main parameters I experimented with were sim_time , vx & vtheta samples , angular and linear velocities and acceleration. 



Launch

The launch file folder has all the launch files used to run all the nodes and additional files required for a soecific task in one line.
The main ones are -
rvac_bringup.launch to Get it running with teleop which lets you control it from your keyboard (teleop_twist_keyboard node)
rvac_navigation.launch to run everything for navigation . This launch file runs the map server which is the node that makes a pre existing map available to all the nodes to use.
Since this method of odometry does not use actual sensor input for odometry I often comment out the rosserial arduino lines from the launch files if i need to test the software without
running the robot (This is what rvac_bringup_noserial.launch is).

Map Server
In the package folder there is an Empty map pgm and its yaml file , this is just to represent an open world with no obstacles that I used to get the planner working .
For the house map , since I still have to add the LiDAR Sensor to be able to use SLAM and map the house , I used a screenshot of the map created by a robot vacuum cleaner I already have from Xiaomi and traced it out in paint.
In the yaml file apart from the image location you have to specify resolution in the form of meters per pixel , for this use a tape a measure to measure the distance between any 2 far apart walls and after converting it into meters divide it by the number of pixels between the 2 walls in the image.

URDF - Not important until LiDAR is mounted , provides information regarding where different parts of the robot like the lidar is with respect to the center of the robot.

Cmakeslist text file - any python scripts that are made to be used as nodes first have to be made into executable files by opening the folder in terminal and running the following
    chmod +x myscript.py
along with adding the following lines at the end of the text file before running catkin_make

catkin_install_python(PROGRAMS scripts/OdomTf.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}






That should be it! Let me know if I missed anything or if you have any questions.
