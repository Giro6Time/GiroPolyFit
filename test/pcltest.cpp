
#pragma once
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>

#include "gtest/gtest.h"

class PCLTEST : public ::testing::Test
{
};

TEST_F(PCLTEST, ReadPointSet)
{
   pcl::PLYReader reader;
   pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
   reader.read<pcl::PointXYZ>("D:/Study/Polyfit/GiroPolyFit/test/happy_vrip.ply", *cloud);
   ASSERT_EQ(543652, cloud->size());
}

int main(int argc, char **argv)
{

   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
