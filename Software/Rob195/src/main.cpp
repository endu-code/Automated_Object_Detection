﻿#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <chrono>

#include <pcl/io/openni2_grabber.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/png_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/console/parse.h>
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/compression/octree_pointcloud_compression.h>
#include <pcl/filters/voxel_grid.h>

#include <octomap/octomap.h>
#include <octomap/ColorOcTree.h>

#include "com_func.h"

#define SERV_TCP_PORT_READ 59004
#define SERV_TCP_PORT_WRITE 59003


using namespace std;
using namespace pcl;

pcl::PointCloud<pcl::PointXYZRGBA>::Ptr transformedCloudCamB (new pcl::PointCloud<pcl::PointXYZRGBA> ());
pcl::PointCloud<pcl::PointXYZRGBA>::Ptr transformedCloudCamA (new pcl::PointCloud<pcl::PointXYZRGBA> ());
pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA>* PointCloudEncoder;
pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA>* PointCloudDecoder;
octomap::ColorOcTree rayMap(0.04);

int nrows = 4;
int ncols = 4;

Eigen::Matrix4f transformCamA = Eigen::Matrix4f::Zero(nrows,ncols);
Eigen::Matrix4f transformCamB = Eigen::Matrix4f::Zero(nrows,ncols);

int positionIndex = 0;
bool debug = false;
bool rayMapinit = false;



class SimpleOpenNIViewer
{
public:
    SimpleOpenNIViewer () : viewer ("PCL OpenNI Viewer") {}
    void cloud_cb_2_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
    {
        if (!viewer.wasStopped()){
            //            auto startPCgenCamB = chrono::steady_clock::now();
            // Create the filtering object
            pcl::PassThrough<pcl::PointXYZRGBA> pass;

            // read cloud transformation data from file
            //            cout << "transformCamB = " << endl << transformCamB << endl;


            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamBfiltered (new pcl::PointCloud<pcl::PointXYZRGBA> ());
            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamBVoxelGrid (new pcl::PointCloud<pcl::PointXYZRGBA> ());

            pass.setInputCloud (cloud);
            pass.setFilterFieldName ("z");
            pass.setFilterLimits (0.0, 3.5);
            //pass.setFilterLimitsNegative (true);
            pass.filter (*CloudCamBfiltered);

            pcl::VoxelGrid<pcl::PointXYZRGBA> voxelFilter;
            voxelFilter.setInputCloud(CloudCamBfiltered);
            voxelFilter.setLeafSize (0.01f, 0.01f, 0.01f);
            voxelFilter.filter (*CloudCamBVoxelGrid);

            // Executing the transformation
            pcl::transformPointCloud (*CloudCamBVoxelGrid, *transformedCloudCamB, transformCamB);

            //    (row, column)
            viewer.showCloud (transformedCloudCamB, cloudnameB);
            //            if(debug == true){
            stringstream stream;
            stream << "inputCloudCamB.pcd";
            string filename = stream.str();
            io::savePCDFileASCII(filename, *CloudCamBfiltered);
//            PCL_ERROR("Problem saving %s.\n", filename.c_str());
            stream.str(std::string());
            stream << "transformedCloudB.pcd";
            filename = stream.str();
            io::savePCDFileASCII(filename, *transformedCloudCamB);
            stream.str(std::string());
            stream << "inputCLoudCamB.png";
            filename = stream.str();
            io::savePNGFile(filename, *cloud, "rgb");
            //            cout<< "Saved " << filename << "." << endl;
            //            cloud = transformedCloudCamB;
            //            }
            //            auto endPCgenCamB = chrono::steady_clock::now();
            //            cout << "Elapsed Time in milliseconds (camB PointCloud generation): " << chrono::duration_cast<chrono::milliseconds>(endPCgenCamB - startPCgenCamB).count() << "ms" << endl;
        }
    }

    void cloud_cb_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
    {
        if (!viewer.wasStopped()){
            //            auto startPCgenCamA = chrono::steady_clock::now();
            // Create the filtering object
            pcl::PassThrough<pcl::PointXYZRGBA> pass;

            //            cout << "transformCamA = " << endl << transformCamA << endl;

            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamAfiltered (new pcl::PointCloud<pcl::PointXYZRGBA> ());
            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamAVoxelGrid (new pcl::PointCloud<pcl::PointXYZRGBA> ());

            pass.setInputCloud (cloud);
            pass.setFilterFieldName ("z");
            pass.setFilterLimits (0.0, 4.1);
            //pass.setFilterLimitsNegative (true);
            pass.filter (*CloudCamAfiltered);


            pcl::VoxelGrid<pcl::PointXYZRGBA> voxelFilter;
            voxelFilter.setInputCloud(CloudCamAfiltered);
            voxelFilter.setLeafSize (0.01f, 0.01f, 0.01f);
            voxelFilter.filter (*CloudCamAVoxelGrid);


            // Executing the transformation
            pcl::transformPointCloud (*CloudCamAVoxelGrid, *transformedCloudCamA, transformCamA);

            //    (row, column)
            viewer.showCloud (transformedCloudCamA, cloudnameA);
            //            if(debug == true){
            stringstream stream;
            stream << "inputCloudCamA.pcd";
            string filename = stream.str();
            io::savePCDFileASCII(filename, *CloudCamAVoxelGrid);
//            PCL_ERROR("Problem saving %s.\n", filename.c_str());
            stream.str(std::string());
            stream << "transformedCloudA.pcd";
            filename = stream.str();
            io::savePCDFileASCII(filename, *transformedCloudCamA);
            stream.str(std::string());
            stream << "inputCLoudCamA.png";
            filename = stream.str();
            io::savePNGFile(filename, *cloud, "rgb");
            //            cout<< "Saved " << filename << "." << endl;
            //            cloud = transformedCloudCamA;
            //            }
            //            auto endPCgenCamA = chrono::steady_clock::now();
            //           cout << "Elapsed Time in milliseconds (camA PointCloud generation): " << chrono::duration_cast<chrono::milliseconds>(endPCgenCamA - startPCgenCamA).count() << "ms" << endl;
        }
    }

    void run ()
    {
        //        //**********************************
        //        start first Xtion
        Grabber* interface = new io::OpenNI2Grabber("#1");

        boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f =
                boost::bind (&SimpleOpenNIViewer::cloud_cb_, this, _1);

        interface->registerCallback (f);
        interface->start();
        //**********************************

        //**********************************
        //        start second Xtion
        pcl::Grabber* interface2 = new io::OpenNI2Grabber("#2");
        boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f2 =
                boost::bind (&SimpleOpenNIViewer::cloud_cb_2_, this, _1);

        interface2->registerCallback (f2);

        interface2->start();
        //**********************************

        while (!viewer.wasStopped())
        {
            bool showStatistics = true;
            bool reset = false;

            pcl::io::compression_Profiles_e compressionProfile = pcl::io::MED_RES_ONLINE_COMPRESSION_WITH_COLOR;

            PointCloudEncoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA> (compressionProfile, showStatistics);
            PointCloudDecoder = new pcl::io::OctreePointCloudCompression<pcl::PointXYZRGBA> ();


            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr concatenatedCloud (new pcl::PointCloud<pcl::PointXYZRGBA> ());
            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZRGBA> ());

            *concatenatedCloud = *transformedCloudCamA;
            *concatenatedCloud += *transformedCloudCamB;

            io::savePCDFileASCII("concatenated.pcd", *concatenatedCloud);

            pcl::VoxelGrid<pcl::PointXYZRGBA> voxelFilter;
            voxelFilter.setInputCloud(concatenatedCloud);
            voxelFilter.setLeafSize (0.04f, 0.04f, 0.04f);
            voxelFilter.filter (*cloud_filtered);


            io::savePCDFileASCII("cloud_filtered.pcd", *cloud_filtered);

            // stringstream to store compressed point cloud
            std::stringstream compressedData;

            // compress point cloud
            PointCloudEncoder->encodePointCloud (cloud_filtered, compressedData);

            //octomap
            //            cout<<"copy data into octomap..."<<endl;
            //
            //            auto startOctreeGen = chrono::steady_clock::now();
            octomap::ColorOcTree tree( 0.04 );
            for (float x = -2.5; x <= 2.5; x += 0.04f) {
                for (float y = 0; y <= 4; y += 0.04f) {
                    for (float z = -1.5; z <= 2; z += 0.04f) {
                        octomap::point3d endpoint(x, y, z);
                        tree.updateNode(endpoint, false); // integrate 'free' measurement
                        //                        rayMap.updateNode(endpoint, false);
                    }
                }
            }

            for (auto p:cloud_filtered->points)
            {
                tree.updateNode(octomap::point3d(p.x, p.y, p.z), true);
                if(rayMapinit == false){
                    rayMap.updateNode(octomap::point3d(p.x, p.y, p.z), true);
                }
            }


            for (auto p:cloud_filtered->points)
            {
                tree.integrateNodeColor( p.x, p.y, p.z, p.r, p.g, p.b );
                if(rayMapinit == false){
                    rayMap.integrateNodeColor( p.x, p.y, p.z, p.r, p.g, p.b );
                }
            }
            rayMapinit = true;
            //            auto endOctreeGen = chrono::steady_clock::now();
            //            cout << "Elapsed Time in milliseconds (Octomap generation): " << chrono::duration_cast<chrono::milliseconds>(endOctreeGen - startOctreeGen).count() << "ms" << endl;

            float fixedPos[24][6];
            //position 1 left side top 1
            fixedPos[0][0] = -860;
            fixedPos[0][1] = 1330;
            fixedPos[0][2] = -560;
            fixedPos[0][3] = 180;
            fixedPos[0][4] = 0.0;
            fixedPos[0][5] = 90.0;

            //position 2 left side bottom
            fixedPos[1][0] = -860;
            fixedPos[1][1] = 1330;
            fixedPos[1][2] = -650;
            fixedPos[1][3] = 180;
            fixedPos[1][4] = 0.0;
            fixedPos[1][5] = 90.0;

            //position 3 left side top 2
            fixedPos[2][0] = -860;
            fixedPos[2][1] = 1330;
            fixedPos[2][2] = -560;
            fixedPos[2][3] = 180;
            fixedPos[2][4] = 0.0;
            fixedPos[2][5] = 90.0;

            //position 4 right side top 1
            fixedPos[3][0] = 600;
            fixedPos[3][1] = 1550;
            fixedPos[3][2] = -560;
            fixedPos[3][3] = 180;
            fixedPos[3][4] = 0.0;
            fixedPos[3][5] = 90.0;

            //position 5 right side bottom
            fixedPos[4][0] = 600;
            fixedPos[4][1] = 1550;
            fixedPos[4][2] = -655;
            fixedPos[4][3] = 180;
            fixedPos[4][4] = 0.0;
            fixedPos[4][5] = 90.0;

            //position 6 right side top 2
            fixedPos[5][0] = 600;
            fixedPos[5][1] = 1550;
            fixedPos[5][2] = -560;
            fixedPos[5][3] = 180;
            fixedPos[5][4] = 0.0;
            fixedPos[5][5] = 90.0;

            //position 7 left side top 1
            fixedPos[6][0] = -860;
            fixedPos[6][1] = 1550;
            fixedPos[6][2] = -560;
            fixedPos[6][3] = 180;
            fixedPos[6][4] = 0.0;
            fixedPos[6][5] = 90.0;

            //position 8 left side bottom
            fixedPos[7][0] = -860;
            fixedPos[7][1] = 1550;
            fixedPos[7][2] = -650;
            fixedPos[7][3] = 180;
            fixedPos[7][4] = 0.0;
            fixedPos[7][5] = 90.0;

            //position 9 left side top 2
            fixedPos[8][0] = -860;
            fixedPos[8][1] = 1550;
            fixedPos[8][2] = -560;
            fixedPos[8][3] = 180;
            fixedPos[8][4] = 0.0;
            fixedPos[8][5] = 90.0;

            //position 10 right side top 1
            fixedPos[9][0] = 600;
            fixedPos[9][1] = 1330;
            fixedPos[9][2] = -560;
            fixedPos[9][3] = 180;
            fixedPos[9][4] = 0.0;
            fixedPos[9][5] = 90.0;

            //position 11 right side bottom
            fixedPos[10][0] = 600;
            fixedPos[10][1] = 1330;
            fixedPos[10][2] = -655;
            fixedPos[10][3] = 180;
            fixedPos[10][4] = 0.0;
            fixedPos[10][5] = 90.0;

            //position 12 right side top 2
            fixedPos[11][0] = 600;
            fixedPos[11][1] = 1330;
            fixedPos[11][2] = -560;
            fixedPos[11][3] = 180;
            fixedPos[11][4] = 0.0;
            fixedPos[11][5] = 90.0;

            //position 13 right side top 1
            fixedPos[12][0] = 600;
            fixedPos[12][1] = 1550;
            fixedPos[12][2] = -560;
            fixedPos[12][3] = 180;
            fixedPos[12][4] = 0.0;
            fixedPos[12][5] = 90.0;

            //position 14 right side bottom
            fixedPos[13][0] = 600;
            fixedPos[13][1] = 1550;
            fixedPos[13][2] = -655;
            fixedPos[13][3] = 180;
            fixedPos[13][4] = 0.0;
            fixedPos[13][5] = 90.0;

            //position 15 right side top 2
            fixedPos[14][0] = 600;
            fixedPos[14][1] = 1550;
            fixedPos[14][2] = -560;
            fixedPos[14][3] = 180;
            fixedPos[14][4] = 0.0;
            fixedPos[14][5] = 90.0;

            //position 16 left side top 1
            fixedPos[15][0] = -860;
            fixedPos[15][1] = 1330;
            fixedPos[15][2] = -560;
            fixedPos[15][3] = 180;
            fixedPos[15][4] = 0.0;
            fixedPos[15][5] = 90.0;

            //position 17 left side bottom
            fixedPos[16][0] = -860;
            fixedPos[16][1] = 1330;
            fixedPos[16][2] = -650;
            fixedPos[16][3] = 180;
            fixedPos[16][4] = 0.0;
            fixedPos[16][5] = 90.0;

            //position 18 left side top 2
            fixedPos[17][0] = -860;
            fixedPos[17][1] = 1330;
            fixedPos[17][2] = -560;
            fixedPos[17][3] = 180;
            fixedPos[17][4] = 0.0;
            fixedPos[17][5] = 90.0;

            //position 19 right side top 1
            fixedPos[18][0] = 600;
            fixedPos[18][1] = 1330;
            fixedPos[18][2] = -560;
            fixedPos[18][3] = 180;
            fixedPos[18][4] = 0.0;
            fixedPos[18][5] = 90.0;

            // position 20 right side bottom
            fixedPos[19][0] = 600;
            fixedPos[19][1] = 1330;
            fixedPos[19][2] = -655;
            fixedPos[19][3] = 180;
            fixedPos[19][4] = 0.0;
            fixedPos[19][5] = 90.0;

            //position 21 right side top 2
            fixedPos[20][0] = 600;
            fixedPos[20][1] = 1330;
            fixedPos[20][2] = -560;
            fixedPos[20][3] = 180;
            fixedPos[20][4] = 0.0;
            fixedPos[20][5] = 90.0;

            //position 22 left side top 1
            fixedPos[21][0] = -860;
            fixedPos[21][1] = 1550;
            fixedPos[21][2] = -560;
            fixedPos[21][3] = 180;
            fixedPos[21][4] = 0.0;
            fixedPos[21][5] = 90.0;

            //position 23 left side bottom
            fixedPos[22][0] = -860;
            fixedPos[22][1] = 1550;
            fixedPos[22][2] = -650;
            fixedPos[22][3] = 180;
            fixedPos[22][4] = 0.0;
            fixedPos[22][5] = 90.0;

            //position 24 left side top 2
            fixedPos[23][0] = -860;
            fixedPos[23][1] = 1550;
            fixedPos[23][2] = -560;
            fixedPos[23][3] = 180;
            fixedPos[23][4] = 0.0;
            fixedPos[23][5] = 90.0;

            float *currentPosition = 0;
            float originPosition[6];
            float goalPosition [6];
            float avoidPositionStart [6];
            float avoidPositionGoal [6];

            currentPosition = connectReadCartesian(currentPosition);
            // remove tool from map to avoid collision with itself
            cout << "***********************************************************************" << endl;
            cout << "Delete camera data of robot tool" << endl;
            cout << "***********************************************************************" << endl;
            for (float x = (currentPosition[0]/1000)-0.10; x <= (currentPosition[0]/1000)+0.10; x += 0.04f) {
                for (float y = (currentPosition[1]/1000)-0.04; y <= (currentPosition[1]/1000)+0.1; y += 0.04f) {
                    for (float z = (currentPosition[2]/1000)-0.04; z <= (currentPosition[2]/1000)+0.1; z += 0.04f) {
                        octomap::point3d endpoint(x, y, z);
                        tree.updateNode(endpoint, false); // integrate 'free' measurement
                        //                        rayMap.updateNode(endpoint, false);
                    }
                }
            }
            cout << "***********************************************************************" << endl;
            cout << "data deleted" << endl;
            cout << "***********************************************************************" << endl;

            for(int s = 0; s < 6; s++){
                goalPosition[s] = fixedPos[positionIndex][s];
                avoidPositionGoal[s] = goalPosition[s];
                avoidPositionStart[s] =  currentPosition[s];
                originPosition[s] = currentPosition[s];
            }
            if(debug == true){
                cout << "***********************************************************************" << endl;
                cout << "Initialisation" << endl;
                cout << "***********************************************************************" << endl;
                printf("avoidPositionStart X pos: %f \n", avoidPositionStart[0]);
                printf("avoidPositionStart Y pos: %f \n", avoidPositionStart[1]);
                printf("avoidPositionStart Z pos: %f \n", avoidPositionStart[2]);
                printf("avoidPositionStart W pos: %f \n", avoidPositionStart[3]);
                printf("avoidPositionStart P pos: %f \n", avoidPositionStart[4]);
                printf("avoidPositionStart R pos: %f \n", avoidPositionStart[5]);
                cout << "***********************************************************************" << endl;
                cout << "***********************************************************************" << endl;
                printf("avoidPositionGoal X pos: %f \n", avoidPositionGoal[0]);
                printf("avoidPositionGoal Y pos: %f \n", avoidPositionGoal[1]);
                printf("avoidPositionGoal Z pos: %f \n", avoidPositionGoal[2]);
                printf("avoidPositionGoal W pos: %f \n", avoidPositionGoal[3]);
                printf("avoidPositionGoal P pos: %f \n", avoidPositionGoal[4]);
                printf("avoidPositionGoal R pos: %f \n", avoidPositionGoal[5]);
                cout << "***********************************************************************" << endl;
                cout << "Initialisation" << endl;
                cout << "***********************************************************************" << endl;
            }
            bool pathFound = false;
            int avoidAttemptCnt = 1;

            tree.updateInnerOccupancy();
            tree.write( "output_file.ot" );

            while(pathFound == false && reset == false){
                cout << "***********************************************************************" << endl;
                cout << "PositionIndex #" << positionIndex << endl;
                cout << "***********************************************************************" << endl;
                cout << "Attempt #" << avoidAttemptCnt << " to move" << endl;

                for (auto p:cloud_filtered->points)
                {
                    tree.updateNode(octomap::point3d(p.x, p.y, p.z), true);
                }


                for (auto p:cloud_filtered->points)
                {
                    tree.integrateNodeColor( p.x, p.y, p.z, p.r, p.g, p.b );
                }

                tree.updateInnerOccupancy();
                tree.write( "output_file.ot" );

                octomap::point3d rayStart(avoidPositionStart[0]/1000, avoidPositionStart[1]/1000, avoidPositionStart[2]/1000);
                octomap::point3d rayDirection((avoidPositionGoal[0]-avoidPositionStart[0])/1000, (avoidPositionGoal[1]-avoidPositionStart[1])/1000, (avoidPositionGoal[2]-avoidPositionStart[2])/1000);
                octomap::point3d end;
                octomap::point3d rayEnd(avoidPositionGoal[0]/1000, avoidPositionGoal[1]/1000, avoidPositionGoal[2]/1000);
                double rayDistance = sqrt(pow((avoidPositionGoal[0]-avoidPositionStart[0])/1000, 2) + pow((avoidPositionGoal[1]-avoidPositionStart[1]), 2)+ pow((avoidPositionGoal[2]-avoidPositionStart[2])/1000, 2));

                bool obstacleFound = tree.castRay(rayStart, rayDirection, end, true, rayDistance);

                cout << "rayDirection x:" << rayDirection.x() << "   y:" << rayDirection.y() << "   z:" << rayDirection.z() << endl;
                std::vector<octomap::point3d> ray;
                //                if(debug == true){
                rayMap.computeRay(rayStart, rayEnd, ray);
                cout << "after computeRay" << endl;
                //                cout << "ray.size() ==" << ray.size()<< endl;

                // compute Ray for rayMap

                for(unsigned int x = 0; x < ray.size(); x++){

                    rayMap.updateNode( octomap::point3d(ray[x].x(), ray[x].y(), ray[x].z()), true );
                    rayMap.integrateNodeColor(ray[x].x(), ray[x].y(), ray[x].z(), 255, 0, 0);
                    //                        cout << "rayCoordinates #" << x <<" :   x:" << ray[x].x() <<"   y:" << ray[x].y()<< "   z:" << ray[x].z() << endl;
                }


                rayMap.updateInnerOccupancy();
                rayMap.write( "rayMap.ot" );
                cout << "after rayMap" << endl;
                //                }

                // Vertical Movement not regarded for collision avoidance
                if(positionIndex == 2 || positionIndex == 5 || positionIndex == 8 || positionIndex == 11 || positionIndex == 14 || positionIndex == 17 || positionIndex == 20 || positionIndex == 23){       // maybe directional condition based on rayDirection to check if robot is moving up or down
                    if(debug == true){do {
                            cout << "***************************************************" << endl;
                            cout << "moving to upper position, press any key to continue" << endl;
                            cout << "***************************************************" << endl;

                        } while (cin.get() != '\n');
                    }
                    connectWriteCartesian(avoidPositionGoal, currentPosition);
                    sleep(0.5);
                    positionIndex ++;
                    pathFound = true;
                }
                // Vertical Movement not regarded for collision avoidance
                else if(positionIndex == 1 || positionIndex == 7 || positionIndex == 13 || positionIndex == 19){  // maybe directional condition based on rayDirection to check if robot is moving up or down
                    if(debug == true){
                        do {
                            cout << "******************************************************" << endl;
                            cout << "moving to gripping position, press any key to continue" << endl;
                            cout << "******************************************************" << endl;

                        } while (cin.get() != '\n');
                    }
                    connectWriteCartesian(avoidPositionGoal, currentPosition);
                    sleep(2);
                    gripperOn();
                    positionIndex ++;
                    pathFound = true;
                }
                // Vertical Movement not regarded for collision avoidance
                else if(positionIndex == 4  || positionIndex == 10 || positionIndex == 16 || positionIndex == 22){  // maybe directional condition based on rayDirection to check if robot is moving up or down
                    if(debug == true){
                        do {
                            cout << "******************************************************" << endl;
                            cout << "moving to gripping position, press any key to continue" << endl;
                            cout << "******************************************************" << endl;

                        } while (cin.get() != '\n');
                    }
                    connectWriteCartesian(avoidPositionGoal, currentPosition);
                    sleep(2);
                    gripperOff();
                    positionIndex ++;
                    pathFound = true;
                }

                //Raycasting not finding any object in the path
                else if( obstacleFound == false){
                    if(debug == true){
                        do {
                            cout << "**************************************************" << endl;
                            cout << "no obstacle, press any key to move to goalPosition" << endl;
                            cout << "**************************************************" << endl;

                        } while (cin.get() != '\n');
                    }
                    connectWriteCartesian(avoidPositionGoal, currentPosition);
                    sleep(2);
                    if(avoidPositionGoal != goalPosition){
                        cout << "avoidPositionGoal != goalPosition" << endl;
                        currentPosition = connectReadCartesian(currentPosition);
                        connectWriteCartesian(goalPosition, currentPosition);
                        sleep(1);
                    }
                    avoidAttemptCnt = 1;
                    positionIndex ++;
                    pathFound = true;
                }

                //Raycasting finding any object in the path
                else if(obstacleFound == true){
                    //                    auto startAvoid = chrono::steady_clock::now();
                    // Attempts to avoid collisions too high!
                    if(avoidAttemptCnt >= 6){

                        do {
                            cout << "*************************************************************" << endl;
                            cout << "can't find a path, remove object and press any key to move to" << endl;
                            cout << "       origin position and restart collision detection" << endl;
                            cout << "*************************************************************" << endl;

                        } while (cin.get() != '\n');

                        connectWriteCartesian(originPosition, currentPosition);
                        sleep(2);
                        //                        currentPosition = connectReadCartesian(currentPosition);
                        for(int s = 0; s < 6; s++){
                            avoidPositionGoal[s] = goalPosition[s];
                            //                            avoidPositionStart[s] =  currentPosition[s];
                        }
                        avoidAttemptCnt = 1;
                        reset = true;
                    }
                    // Attempting to avoid collision by moving in z-Direction
                    else{
                        if(debug == true){
                            do {
                                cout << "***********************************************************************" << endl;
                                cout << "obstacle found, moving up to avoid collision. Press any key to continue" << endl;
                                cout << "***********************************************************************" << endl;

                            } while (cin.get() != '\n');
                        }
                        avoidPositionStart[2] += 100;
                        avoidPositionGoal[2] +=100;
                        if(debug == true){
                            cout << "***********************************************************************" << endl;
                            cout << "After z incrementation" << endl;
                            cout << "***********************************************************************" << endl;
                            printf("avoidPositionStart X pos: %f \n", avoidPositionStart[0]);
                            printf("avoidPositionStart Y pos: %f \n", avoidPositionStart[1]);
                            printf("avoidPositionStart Z pos: %f \n", avoidPositionStart[2]);
                            printf("avoidPositionStart W pos: %f \n", avoidPositionStart[3]);
                            printf("avoidPositionStart P pos: %f \n", avoidPositionStart[4]);
                            printf("avoidPositionStart R pos: %f \n", avoidPositionStart[5]);
                            cout << "***********************************************************************" << endl;
                            cout << "After z incrementation" << endl;
                            cout << "***********************************************************************" << endl;
                        }
                        connectWriteCartesian(avoidPositionStart, currentPosition);
                        sleep(0.5);
                        currentPosition = connectReadCartesian(currentPosition);
                        avoidAttemptCnt ++;
                    }
                    //                    auto endAvoid = chrono::steady_clock::now();
                    //                    cout << "Elapsed Time in milliseconds (avoid obstacle): " << chrono::duration_cast<chrono::milliseconds>(endAvoid - startAvoid).count() << "ms" << endl;
                }

                // counter for fixed positions reset to start from the beginning.
                if(positionIndex >= 24){
                    positionIndex = 0;
                }
                if(debug == true){
                    do {
                        cout << "*********************************************" << endl;
                        cout << "checkpoint reached, press any key to continue" << endl;
                        cout << "*********************************************" << endl;
                    } while (cin.get() != '\n');
                }
            }
        }
        interface->stop ();
        interface2->stop ();
    }

    pcl::visualization::CloudViewer viewer;
    const std::string cloudnameA = "cloudA";
    const std::string cloudnameB = "cloudB";
};

int main (int argc, char * const argv[])
{
    if(argc > 1){
        if (argv[1] == std::string("-d")){
            cout << "*********************************************" << endl;
            cout << "Entering Debug Mode, have fun pushing buttons" << endl;
            cout << "*********************************************" << endl;
            debug = true;
        }
    }

    // read cloud transformation data from file
    ifstream fin ("./../Rob195/cloudA.txt");

    if (fin.is_open())
    {
        for (int row = 0; row < nrows; row++)
            for (int col = 0; col < ncols; col++)
            {
                float item = 0.0;
                fin >> item;
                transformCamA(row, col) = item;
            }
        fin.close();
    }
    ifstream fin2 ("./../Rob195/cloudB.txt");
    {
        for (int row = 0; row < nrows; row++)
            for (int col = 0; col < ncols; col++)
            {
                float item = 0.0;
                fin2 >> item;
                transformCamB(row, col) = item;
            }
        fin2.close();
    }

    SimpleOpenNIViewer v;
    v.run ();

    return 0;
}