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
bool camAsaved = false;
bool camBsaved = false;


using namespace std;
using namespace pcl;


class SimpleOpenNIViewer
{
public:
    SimpleOpenNIViewer () : viewer ("PCL OpenNI Viewer") {}
    void cloud_cb_2_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
    {
        if (!viewer.wasStopped()){

            pcl::PassThrough<pcl::PointXYZRGBA> pass;

            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamBfiltered (new pcl::PointCloud<pcl::PointXYZRGBA> ());

            pass.setInputCloud (cloud);
            pass.setFilterFieldName ("z");
            pass.setFilterLimits (0.0, 3.5);
            //pass.setFilterLimitsNegative (true);
            pass.filter (*CloudCamBfiltered);


            viewer.showCloud (CloudCamBfiltered, cloudnameB);
            stringstream stream;
            stream << "referenceCloudB.pcd";
            string filename = stream.str();
            io::savePCDFileASCII(filename, *CloudCamBfiltered);

            stream.str(std::string());
            stream << "referenceCloudB.png";
            filename = stream.str();
            io::savePNGFile(filename, *CloudCamBfiltered, "rgb");
        }
    }

    void cloud_cb_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
    {
        if (!viewer.wasStopped()){
            pcl::PassThrough<pcl::PointXYZRGBA> pass;

            pcl::PointCloud<pcl::PointXYZRGBA>::Ptr CloudCamAfiltered (new pcl::PointCloud<pcl::PointXYZRGBA> ());

            pass.setInputCloud (cloud);
            pass.setFilterFieldName ("z");
            pass.setFilterLimits (0.0, 4.1);
            //pass.setFilterLimitsNegative (true);
            pass.filter (*CloudCamAfiltered);

            viewer.showCloud (CloudCamAfiltered, cloudnameA);
            stringstream stream;
            stream << "referenceCloudA.pcd";
            string filename = stream.str();
            io::savePCDFileASCII(filename, *CloudCamAfiltered);

            stream.str(std::string());
            stream << "referenceCloudA.png";
            filename = stream.str();
            io::savePNGFile(filename, *CloudCamAfiltered, "rgb");
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
        Grabber* interface2 = new io::OpenNI2Grabber("#2");
        boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f2 =
                boost::bind (&SimpleOpenNIViewer::cloud_cb_2_, this, _1);

        interface2->registerCallback (f2);
        interface2->start();
        //**********************************

        while (!viewer.wasStopped())
        {

        }
        interface->stop ();
        interface2->stop ();
    }

    pcl::visualization::CloudViewer viewer;
    const std::string cloudnameA = "cloudA";
    const std::string cloudnameB = "cloudB";
};


int main ()
{

    int tableLength = 2250;
    int tableWidth  = 1480;
    float tableHeight = 670;

    float tableStartingpointX = -1274.883;
    float tableStartingpointY = 1154.362;
    float tableStartingpointZ = -653.149;

    // Fill in the cloud data

    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr basic_cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGBA>);
    uint8_t r(255), g(15), b(15);
    uint8_t r3(15), g3(15), b3(255);
    for (float xAxis = tableStartingpointX; xAxis < tableStartingpointX+tableLength; xAxis += 10){
        for(float yAxis = tableStartingpointY; yAxis < tableStartingpointY+tableWidth; yAxis += 10){
            //for(float zAxis = tableStartingpointZ; zAxis < tableStartingpointZ+tableHeight; zAxis += 10){
            pcl::PointXYZRGBA basic_point;
            basic_point.x = xAxis/1000;
            basic_point.y = yAxis/1000;
            basic_point.z = tableStartingpointZ/1000;


            if(xAxis <= tableStartingpointX+1000){
                if(yAxis <= tableStartingpointY+1000){
                    uint32_t rgb = (static_cast<uint32_t>(r3) << 16 |
                                    static_cast<uint32_t>(g3) << 8 | static_cast<uint32_t>(b3));
                    basic_point.rgb = *reinterpret_cast<float*>(&rgb);
                }
                else{
                    uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
                                    static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
                    basic_point.rgb = *reinterpret_cast<float*>(&rgb);
                }
            }
            else if(xAxis > (tableStartingpointX+tableLength) -1000){
                if(yAxis > (tableStartingpointY+tableWidth)-1000){
                    uint32_t rgb = (static_cast<uint32_t>(r3) << 16 |
                                    static_cast<uint32_t>(g3) << 8 | static_cast<uint32_t>(b3));
                    basic_point.rgb = *reinterpret_cast<float*>(&rgb);
                }
                else {
                    uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
                                    static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
                    basic_point.rgb = *reinterpret_cast<float*>(&rgb);
                }

            }
            else{
                uint32_t rgb = (static_cast<uint32_t>(r) << 16 |
                                static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(b));
                basic_point.rgb = *reinterpret_cast<float*>(&rgb);
            }


            basic_cloud_ptr->points.push_back(basic_point);


            //}
        }
    }


    uint8_t r2(15), g2(255), b2(15);
    for (float xAxis = tableStartingpointX; xAxis < tableStartingpointX+500; xAxis += 10){
        for(float yAxis = tableStartingpointY; yAxis < tableStartingpointY+500; yAxis += 10){
            //for(float zAxis = tableStartingpointZ; zAxis < tableStartingpointZ+tableHeight; zAxis += 10){
            pcl::PointXYZRGBA basic_point;
            basic_point.x = xAxis/1000;
            basic_point.y = yAxis/1000;
            basic_point.z = (tableStartingpointZ - 100)/1000;
            uint32_t rgb = (static_cast<uint32_t>(r2) << 16 |
                            static_cast<uint32_t>(g2) << 8 | static_cast<uint32_t>(b2));
            basic_point.rgb = *reinterpret_cast<float*>(&rgb);
            basic_cloud_ptr->points.push_back(basic_point);

            //}
        }
    }

    basic_cloud_ptr->width = (int) basic_cloud_ptr->points.size ();
    basic_cloud_ptr->height = 1;

    pcl::io::savePCDFileASCII ("referenceCloudTable.pcd", *basic_cloud_ptr);
    std::cerr << "Saved data points to referenceCloudTable.pcd." << std::endl;

    SimpleOpenNIViewer v;
    v.run ();

    return (0);
}