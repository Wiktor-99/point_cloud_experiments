#include <iostream>
#include <string>
#include <filesystem>

#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

namespace fs = std::filesystem;

enum class FileFormat { PCD, PLY, UNKNOWN };

FileFormat getFileFormat(const std::string& filename) {
    fs::path path(filename);
    std::string ext = path.extension().string();

    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".pcd") return FileFormat::PCD;
    if (ext == ".ply") return FileFormat::PLY;
    return FileFormat::UNKNOWN;
}

template<typename PointT>
bool loadPointCloud(const std::string& filename, typename pcl::PointCloud<PointT>::Ptr& cloud) {
    FileFormat format = getFileFormat(filename);

    switch (format) {
        case FileFormat::PCD:
            if (pcl::io::loadPCDFile<PointT>(filename, *cloud) == -1) {
                std::cerr << "Error: Could not load PCD file: " << filename << std::endl;
                return false;
            }
            break;
        case FileFormat::PLY:
            if (pcl::io::loadPLYFile<PointT>(filename, *cloud) == -1) {
                std::cerr << "Error: Could not load PLY file: " << filename << std::endl;
                return false;
            }
            break;
        default:
            std::cerr << "Error: Unknown file format for: " << filename << std::endl;
            return false;
    }
    return true;
}

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <point_cloud_file>\n"
              << "\nSupported formats:\n"
              << "  .pcd - Point Cloud Data format\n"
              << "  .ply - Polygon File Format\n"
              << "\nViewer controls:\n"
              << "  Mouse left button   - Rotate view\n"
              << "  Mouse middle button - Pan view\n"
              << "  Mouse wheel         - Zoom in/out\n"
              << "  r                   - Reset camera\n"
              << "  g                   - Show/hide coordinate system\n"
              << "  j                   - Take screenshot\n"
              << "  q                   - Quit\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];

    if (!fs::exists(filename)) {
        std::cerr << "Error: File does not exist: " << filename << std::endl;
        return 1;
    }

    std::cout << "Loading point cloud from: " << filename << std::endl;

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_rgb(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz(new pcl::PointCloud<pcl::PointXYZ>);

    bool has_color = false;

    if (loadPointCloud<pcl::PointXYZRGB>(filename, cloud_rgb) && cloud_rgb->size() > 0) {
        bool all_black = true;
        for (const auto& pt : *cloud_rgb) {
            if (pt.r != 0 || pt.g != 0 || pt.b != 0) {
                all_black = false;
                break;
            }
        }
        has_color = !all_black;
    }

    if (!has_color) {
        if (!loadPointCloud<pcl::PointXYZ>(filename, cloud_xyz)) {
            return 1;
        }
    }

    size_t num_points = has_color ? cloud_rgb->size() : cloud_xyz->size();
    std::cout << "Loaded " << num_points << " points" << std::endl;

    if (num_points == 0) {
        std::cerr << "Error: Point cloud is empty" << std::endl;
        return 1;
    }

    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Point Cloud Viewer"));
    viewer->setBackgroundColor(0.1, 0.1, 0.1);

    if (has_color) {
        pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud_rgb);
        viewer->addPointCloud<pcl::PointXYZRGB>(cloud_rgb, rgb, "cloud");
    } else {
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud_xyz, 255, 255, 255);
        viewer->addPointCloud<pcl::PointXYZ>(cloud_xyz, single_color, "cloud");
    }

    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "cloud");
    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();
    viewer->resetCamera();

    std::cout << "Viewer started. Press 'q' to quit." << std::endl;

    while (!viewer->wasStopped()) {
        viewer->spinOnce(100);
    }

    return 0;
}
