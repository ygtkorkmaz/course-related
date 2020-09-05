# Path Finding by Implementing APF
This code is written in order to achieve path planning by implementing Artificial Potential Field theory, which is a mathematical representation of effects of obstacles and target points on a robots trajectory.  

According to this theory, obstacles are represented as repulsive forces whose strength depends on their closeness to robot's position, where targets are represented as attractive forces, again whose strength depends on their positions with respect to robot's current position. So, if this function is created in such a way that, goal position(g) is the global minimum, the actuation will be ensured.

In rosThread.cpp file, you can find the coded version of mathematical expression below.

![APF](https://i.ibb.co/pr0nQRk/1.png)  
Where nominator and denominator of this function are defined as;

![APF2](https://i.ibb.co/sskR0rk/2.png)

![APF3](https://i.ibb.co/YkFF1zV/3.png) 

![APF4](https://i.ibb.co/W5nXPDq/4.png) 

o<sub>j</sub> is position of jth obstacle, and p<sub>r</sub>, p<sub>j</sub> are radii of the robot and jth obstacle respectively.


## Important Notes

- Note that this code is a part of bigger ROS code, so clonening this repo alone will not work. You need other parts such as odometry data etc.
- The locations of obstacles and goal points with respect to an origin must be defined in .h file in order for this code to run properly.
- The k parameter in the APF is kind of a smoothing parameter used in calculations moving curves. This can be changed from .h file
