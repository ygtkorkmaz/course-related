#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <math.h>
#include <cfloat>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <minik_ros_wrapper/minikSetVelocityMsg.h>
#include <QObject>
#include <QDebug>

#define PI 3.14159265359

class RosThread:public QObject{
    Q_OBJECT

  public:

    RosThread();
    ~RosThread();

  private:

    double pose_x,pose_y,pose_theta; //position

	double force_x;//force x
	double force_y;//force y

    double linear_vel;//linear velocity
    double angular_vel;//angular velocity

    double speedR;//right speed
    double speedL;//left speed

    ///////////////////////parameters////////////////////////////////
    //parameters such as radius, k, obstacles can be defined here
    double ro_ij = 0.15;
    double k=4;
    int nrPackage = 3;
    int nrObstacle = 2;
    int nrPicked = 0;
    int nrDelivered = 0;
    double epsilon = 0.20;
    double sender_pos[3][2] = {{1.66,0.33},{2.33,1.66}, {0.66, 2.33}};
    double reciever_pos[3][2] = {{2.33,0},{0.33,0.66},{1, 1.33}};
    double obstacle_pos[2][2] = {{1.33, 1}, {1.33, 2}};
    double base_pos[2] = {0,0};
    double obs_rads = 0.05;
    double pickedPackages[3] = {0, 0, 0};
    double deliveredPackages[3] = {0, 0, 0};
    int flag = 0;

    int onLoc(double locPos[]);
    void check();
    void sort(double target[][2],double posx, double posy, int number);




	///////////////////////////////////////////////////////////
    //handles
	ros::NodeHandle n;
    ros::Subscriber odomSub;//to subscribe odometry data
    ros::Publisher velPub; //to publish speed

    void odomHandler(const nav_msgs::OdometryConstPtr &odomMsg);// subscriber function for odometry data
    void apf(double pos[]);//calculates force by using apf
    void setVelocity();//converts force to angular and linear velocities
    void setRobotSpeed();//calculates left and right speeds using linear and angular velocity
    void sendVelocityCommand();//publisher function for left and right speeds

    static const int ticks_per_meter = 10610;

  public slots:
    void work();
    void shutdownROS();

  signals:
    void rosFinished();
    void rosStarted();


};
