/*
 * xperia_node.cpp
 *
 *  Created on: 22/02/2015
 *      Author: Paulo Rezeck
 *      Email: prezeck@hotmail.com
 */

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_broadcaster.h>
#include <visualization_msgs/Marker.h>

ros::Publisher markerPub;

void imuCallback(const sensor_msgs::ImuConstPtr data)
{
    // Transformation from "base_link" to "imu" frame
    static tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    tf::Quaternion q;
    q.setX(data->orientation.x);
    q.setY(data->orientation.y);
    q.setZ(data->orientation.z);
    q.setW(data->orientation.w);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_link", data->header.frame_id));

    // Create Marker Visualization of Xperia model
    visualization_msgs::Marker xperia_model;
    xperia_model.header.frame_id = data->header.frame_id;
    xperia_model.header.stamp = ros::Time::now();
    xperia_model.ns = "xperia";
    xperia_model.id = 0;
    xperia_model.type = visualization_msgs::Marker::MESH_RESOURCE;
    xperia_model.action = visualization_msgs::Marker::ADD;
    xperia_model.pose.orientation = data->orientation;
    xperia_model.scale.x = 1.0;
    xperia_model.scale.y = 1.0;
    xperia_model.scale.z = 1.0;
    xperia_model.mesh_resource = "package://xperia_pkg/meshes/xperia_z1.dae";

    markerPub.publish (xperia_model);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "xperia_node");

    ros::NodeHandle nh_;

    ros::Subscriber imuSub = nh_.subscribe("/android/imu", 1, imuCallback);

    markerPub = nh_.advertise<visualization_msgs::Marker>("/xperia/model", 1);

    ros::Rate loop_rate(10);

    ROS_INFO("Executing xperia_node...");

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

	return 0;
}


