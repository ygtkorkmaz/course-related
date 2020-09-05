#include "rosThread.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <sstream>


using namespace std;


RosThread::RosThread(){ //constructor of the class, variables can be initialized here if it's necessary

}

RosThread::~RosThread(){//destructor of the class

}

void RosThread::shutdownROS()
{
    ros::shutdown();
}

void RosThread::setVelocity(){//converts force to angular and linear velocities

  double velocity_angle = atan2(force_y,force_x);

  if(force_x==0 && force_y == 0){

	  linear_vel = 0;
      angular_vel = 0;
   }

  else{
    double delta_angle = velocity_angle-pose_theta;//difference between velocity angle and robot angle

    double vMax = 0.2;//maximum linear velocity
    double wMax = PI/3;//maximum angular velocity

  	linear_vel = cos(delta_angle)*0.2;
  	angular_vel = sin(delta_angle)*PI/3;


  }
   RosThread::setRobotSpeed();//calculates left and right speeds using linear and angular velocity
   RosThread::sendVelocityCommand();//publishes left and right speeds
}

void RosThread::setRobotSpeed(){//calculates left and right speeds using linear and angular velocity
    speedR = -(linear_vel + angular_vel*0.1);//minus comes from angular momentum
    speedL = linear_vel - angular_vel*0.1;

    //max and min values
    if(speedR > 1){
      speedR = 1.25;
    }
    else if(speedR < -1){
      speedR = -1.25;
    }
    if(speedL > 1){
      speedL = 1.25;
    }
    else if(speedL < -1){
      speedL = -1.25;
    }

}

void RosThread::sendVelocityCommand(){//publishes left and right speeds
    int leftTick = speedL * ticks_per_meter;
    int rightTick = speedR * ticks_per_meter;

    minik_ros_wrapper::minikSetVelocityMsg msg;//msg type is minik_ros_wrapper::minikSetVelocityMsg

    vector<int> motorID;
    motorID.push_back(0);
    motorID.push_back(1);
    msg.motorID = motorID;

    vector<int> velocity;
    velocity.push_back(leftTick);
    velocity.push_back(rightTick);
    msg.velocity = velocity;

    velPub.publish(msg);
}
////////////////////////////////////////////////////
void RosThread::apf(double pos[]){//finds force x and force y using apf
    double gamma =  pow((pose_x-pos[0]),2) + pow((pose_y-pos[1]),2);
    double gamma_grad[2];
    gamma_grad[0] = 2*(pose_x-pos[0]);
    gamma_grad[1] = 2*(pose_y-pos[1]);

    double beta = 1;
    double beta_j[nrObstacle];
    for(int i = 0; i<nrObstacle; i++) {
        beta_j[i] = (pow((pose_x - obstacle_pos[i][0]) , 2) + pow((pose_y - obstacle_pos[i][1]) , 2) - pow((ro_ij + obs_rads) , 2));
        beta = beta * beta_j[i];
    }
    
    double beta_grad[2] = {0, 0};
	for(int i = 0; i < nrObstacle; i++){
	    beta_grad[0] = beta_grad[0] + (pose_x - obstacle_pos[i][0])/beta_j[i];
        beta_grad[1] = beta_grad[1] + (pose_y - obstacle_pos[i][1])/beta_j[i];
    }

	beta_grad[0] = 2*beta*beta_grad[0];
    beta_grad[1] = 2*beta*beta_grad[1];

    force_x = -(gamma_grad[0]*pow((beta+pow(gamma,k)),(1/k)) - (pow((beta + pow(gamma,k)),((1/k)-1)))*((gamma/k)*beta_grad[0] + pow(gamma,k)*gamma_grad[0]))/pow((beta+pow(gamma,k)),(2/k));
	force_y = -(gamma_grad[1]*pow((beta+pow(gamma,k)),(1/k)) - (pow((beta + pow(gamma,k)),((1/k)-1)))*((gamma/k)*beta_grad[1] + pow(gamma,k)*gamma_grad[1]))/pow((beta+pow(gamma,k)),(2/k));

}

int RosThread::onLoc(double locPos[]){
    double x = locPos[0] - pose_x;
    double y = locPos[1] - pose_y;
    double dist = sqrt(x*x + y*y);
    
    if(dist <= epsilon){
        return 1;
    }
    else{
        return 0;
    }
}

void RosThread::check(){
    for (int i = 0; i < nrPackage; i++) {
        if((onLoc(sender_pos[i]) == 1) && (pickedPackages[i]!= 1)){
            pickedPackages[i] = 1;
            nrPicked ++;
        }
        if((onLoc(reciever_pos[i]) == 1) && (deliveredPackages[i]!= 1)){
            deliveredPackages[i] = 1;
            nrDelivered ++;
        }
    }
}

void RosThread::sort(double target[][2],double posx, double posy, int number){
    double distance[number];
    double temp[2];
    double temp_2;
    for(int k = 0; k<number; k++){
        double x = target[k][0] - posx;
        double y = target[k][1] - posy;
        distance[k] = sqrt(x*x + y*y);
    }
    for(int i = 0; i<number-1; i++){
        for(int j = 0; j<number-i-1; j++){
            if(distance[j]>distance[j+1]){
				temp_2 = distance[j];
				distance[j] = distance[j+1];
				distance[j+1] = temp_2;
                temp[0] = target[j][0];
                temp[1] = target[j][1];
                target[j][0] = target[j+1][0];
                target[j][1] = target[j+1][1];
                target[j+1][0] = temp[0];
                target[j+1][1] = temp[1];
            }
        }
    }
}


////////////////////////////////////////////////////
void RosThread::odomHandler(const nav_msgs::OdometryConstPtr &odomMsg){// subscribes odometry data

    pose_x = odomMsg->pose.pose.position.x;//x position
    pose_y = odomMsg->pose.pose.position.y;//y position
    pose_theta = odomMsg->pose.pose.position.z;//theta (angle of robot, not z position)


}


void RosThread::work(){ //this function is run only once at main.cpp, this function is like main loop for a cpp file

if(!ros::ok()){//if there is any problem
        qDebug()<< "ROS Init failed!!!";
        ros::shutdown();
        emit rosFinished();
        return;

  }
  emit rosStarted();

  odomSub = n.subscribe("odom", 1, &RosThread::odomHandler, this);//subscribes odometry data
  velPub = n.advertise<minik_ros_wrapper::minikSetVelocityMsg>("minik_ros_wrapper/minikSetVelocityMsg", 1);//publishes left and right speeds

  ros::Rate loop_rate(10);


  while (ros::ok()){//
	if(flag == 0){
        RosThread::sort(sender_pos,base_pos[0],base_pos[1], nrPackage);
        RosThread::sort(reciever_pos,sender_pos[nrPackage-1][0],sender_pos[nrPackage-1][1], nrPackage);
        flag = 1;
    }
    RosThread::check();
   ////////////////////////////////////
   //if(onLoc(base_pos)!=1 && (force_y < 0.1 && force_y > -0.1) && (force_x < 0.1 && force_x > -0.1)){
     
     //RosThread::setVelocity();
   //}
  //else{
   if(nrPicked < nrPackage) {
       RosThread::apf(sender_pos[nrPicked]);
       cout<<"sender "<<sender_pos[nrPicked][0] << " "<< sender_pos[nrPicked][1] <<" number: "<< nrPicked<< "\n";
       RosThread::setVelocity();
   }
   else if(nrDelivered < nrPackage){
       RosThread::apf(reciever_pos[nrDelivered]);
       cout<<"reciever "<<reciever_pos[nrDelivered][0] << " "<< reciever_pos[nrDelivered][1] <<" number: "<< nrDelivered<< "\n";
       RosThread::setVelocity();
   }
   else{
       if(onLoc(base_pos) == 1){
           force_x = 0;
           force_y = 0;
           RosThread::setVelocity();
           RosThread::shutdownROS();
       }
       else{
           RosThread::apf(base_pos);
           RosThread::setVelocity();
       }
   }
   //}

	////////////////////////////////////
    ros::spinOnce();//for callback function(odometry callback function)

    loop_rate.sleep();
  }


  qDebug()<<"I am quitting test project";

    ros::shutdown();
    emit rosFinished();


}
