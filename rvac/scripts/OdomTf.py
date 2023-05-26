#!/usr/bin/env python



import math
from math import sin, cos, pi

import rospy
import tf
from std_msgs.msg import String 
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3
l = 0.29
vlin = 0
vang = 0
vl = 0
vr = 0
#pub     = rospy.Publisher('odomtfpub',String,queue_size=50)

x = 0.0
y = 0.0
th = 0.0

vx = 0.1
vy = -0.1
vth = 0.1


odom_pub = rospy.Publisher("odom", Odometry, queue_size=50)
odom_broadcaster = tf.TransformBroadcaster()


    
def subscriber():
    
    global vlin , current_time , last_time , th , x , y , vang , vl , vr , vx , vth , vy , dt

    r = rospy.Rate(20)
    while not rospy.is_shutdown():
    
    
    
        current_time = rospy.Time.now()


        vl = vlin - (vang * l/2)
        vr = vlin + (vang * l/2)
        #dr = vr * dt
        #dl = vl * dt


        vx = vlin * cos(th)
        vy = vlin * sin(th)
        vth = vang


        dt = (current_time - last_time).to_sec()
        #delta_x = (vx * cos(th) - vy * sin(th)) * dt
        #delta_y = (vx * sin(th) + vy * cos(th)) * dt
        delta_x = vx * dt
        delta_y = vy * dt
        delta_th = vth * dt

        x += delta_x
        y += delta_y
        th += delta_th   
    	
        odom_quat = tf.transformations.quaternion_from_euler(0, 0, th)  
        odom_broadcaster.sendTransform(
            (x, y, 0.),
            odom_quat,
            current_time,
            "base_link",
            "odom"
    )
        odom = Odometry()
        odom.header.stamp = current_time
        odom.header.frame_id = "odom"
        
        # set the position
        odom.pose.pose = Pose(Point(x, y, 0.), Quaternion(*odom_quat))

        # set the velocity
        odom.child_frame_id = "base_link"
        odom.twist.twist = Twist(Vector3(vlin, 0, 0), Vector3(0, 0, vth))

        #odom.twist.twist.linear.x = vx
        #odom.twist.twist.linear.y = vy
        #odom.twist.twist.angular.z = vang

        # publish the message
        odom_pub.publish(odom)

        last_time = current_time 
        
               
        #pub.publish("Lolol%s"%vlin)
        r.sleep()
          
   
    
def callback_function(message):
    rospy.loginfo("Linear : %s"%message.linear.x)
    global vlin , vang
    vlin = message.linear.x
    rospy.loginfo("Angular: %s"%message.angular.z)
    vang = message.angular.z    
    
    #vl = vlin -(vang *l/2)
    #vr = vlin +(vang *l/2)
    #rospy.loginfo("bonk bruh")
    
    
    
if __name__ == "__main__":
    rospy.init_node("cmd_sub")
    sub = rospy.Subscriber('cmd_vel', Twist, callback_function)

    current_time = rospy.Time.now()
    last_time = rospy.Time.now()
    subscriber()
    
#use the method of replacing rspy.spin with a loop and publishing odom in it and also see the python guy with two publishers how he did it! Lol doge
