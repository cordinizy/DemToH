#include <iostream>
#include <Eigen/Eigen>
#include <vector>
#include <gdal.h>
#include <gdal_priv.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

Eigen::Vector2i getDEMXY(double &lat, double &lon, char* DEMfilepath) {
    GDALAllRegister();
    GDALDataset *demdataset;
    demdataset = (GDALDataset *) GDALOpen(DEMfilepath, GA_ReadOnly);

    if(demdataset == NULL){
        std::cout << "Can't open DEMdata." << std::endl;
    }

    GDALRasterBand *imageBand1;
    imageBand1 = demdataset->GetRasterBand(1);
    double trans[6];
    demdataset->GetGeoTransform(trans);
    int nX = (int)((lon - trans[0]) / trans[1]);
    int nY = (int)((lat - trans[3]) / trans[5]);
    Eigen::Vector2i theXY(nX, nY);
    delete demdataset;
    return theXY;
}

Eigen::Vector2d getDEMvalue(Eigen::Vector2i &startXY, Eigen::Vector2i &endXY, char* DEMfilepath) {
    GDALAllRegister();
    GDALDataset *demdataset;
    demdataset = (GDALDataset *) GDALOpen(DEMfilepath, GA_ReadOnly);

    if(demdataset == NULL){
        std::cout << "Can't open DEMdata." << std::endl;
    }
    GDALRasterBand *imageBand1;
    imageBand1 = demdataset->GetRasterBand(1);
    double h;
    std::vector<double> H;
    for (int i = startXY.x(); i <= endXY.x(); ++i) {
        for (int j = startXY.y(); j < endXY.y(); ++j) {
            imageBand1->RasterIO(GF_Read, i, j, 1, 1, &h, 1, 1, GDT_Float64, 0, 0);
            H.push_back(h);
        }
    }
    delete demdataset;
    auto result = std::minmax_element(H.begin(), H.end());
    double h_max = *result.first;
    double h_min = *result.second;
    Eigen::Vector2d minmax(h_min, h_max);
    return minmax;
}

void getTiffLatLon(const char *TifFilePath, double &leftTopLat, double &leftTopLon, double &rightTopLat, double &rightTopLon) {

    // 初始化GDAL库
    GDALAllRegister();
    // 打开TIFF文件
    GDALDataset *poDataset = (GDALDataset*) GDALOpen(TifFilePath, GA_ReadOnly);
    if (poDataset == NULL) {
        printf("Can't open TIFF File.\n");
        return;
    }
    // 获取地理转换信息
    double adfGeoTransform[6];
    if (poDataset->GetGeoTransform(adfGeoTransform) != CE_None) {
        printf("Can't get GeoTransform.\n");
        GDALClose(poDataset);
        return;
    }

    // 获取图像的宽度和高度
    int width = poDataset->GetRasterXSize();
    int height = poDataset->GetRasterYSize();

    leftTopLat = adfGeoTransform[3];
    leftTopLon = adfGeoTransform[0];
    rightTopLon = adfGeoTransform[0] + adfGeoTransform[1] * width + adfGeoTransform[2] * height;
    rightTopLat = adfGeoTransform[3] + adfGeoTransform[4] * width + adfGeoTransform[5] * height;

    GDALClose(poDataset);
}

std::vector<std::string> findTifFiles(const fs::path& dir) {
    std::vector<std::string> result;
    for (fs::recursive_directory_iterator it(dir), end; it != end; ++it) {
        if (fs::is_regular_file(*it) &&
            (it->path().extension() == ".tif" ||
             it->path().extension() == ".tiff")) {
            result.push_back(it->path().string());
        }
    }
    std::sort(result.begin(), result.end());
    return result;
}

int main(int argc, char* argv[]){

    if (argc <= 2){
        std::cout << "=========================== DEM_To_H Use Method ========================================" << std::endl;
        std::cout << "   : .exe -txt [leftTopLat] [leftTopLon] [rightBottomLat] [rightBottomLon] [DEMFilePath]" << std::endl;
        std::cout << "   : .exe -tif [tifFilePath] [DEMFilePath]" << std::endl;
        std::cout << "   : .exe -path [dirPath] [DEMFilePath]" << std::endl;
        std::cout << "   : ! Lat and Lon -> (North, East)" << std::endl;
        std::cout << "Image: " << std::endl;
        std::cout << "   startLatLon----------N-------------------- " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   W                                        E " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   -                                        - " << std::endl;
        std::cout << "   ---------------------S---------  endLatLon " << std::endl;
        return 0;
    }
    std::string option = argv[1];
    if(option == "-txt"){
        double start_lat = std::stod(argv[2]);//leftTopLat
        double start_lon = std::stod(argv[3]);//leftTopLon
        double end_lat = std::stod(argv[4]);//rightBottomLat
        double end_lon = std::stod(argv[5]);//rightBottomLon
        char* DemFilePath = argv[6];
        Eigen::Vector2i startXY;
        Eigen::Vector2i endXY;
        Eigen::Vector2d H_minmax;

        startXY = getDEMXY(start_lat, start_lon, DemFilePath);
        endXY = getDEMXY(end_lat, end_lon, DemFilePath);

        H_minmax = getDEMvalue(startXY, endXY, DemFilePath);
        std::cout << "(" << start_lat << ", " << start_lon << ")" << "---" << "(" << end_lat << ", " << end_lon << ")"
                  << ": Max H = " << H_minmax.x() << ",  Min H = " << H_minmax.y() << std::endl;
    } else if(option == "-tif"){
        double start_lat;//leftTopLat
        double start_lon;//leftTopLon
        double end_lat;//rightBottomLat
        double end_lon;//rightBottomLon
        char* DemFilePath = argv[3];
        getTiffLatLon(argv[2], start_lat, start_lon, end_lat, end_lon);
        Eigen::Vector2i startXY;
        Eigen::Vector2i endXY;
        Eigen::Vector2d H_minmax;

        startXY = getDEMXY(start_lat, start_lon, DemFilePath);
        endXY = getDEMXY(end_lat, end_lon, DemFilePath);

        H_minmax = getDEMvalue(startXY, endXY, DemFilePath);
        std::cout << "(" << start_lat << ", " << start_lon << ")" << "---" << "(" << end_lat << ", " << end_lon << ")"
                  << ": Max H = " << H_minmax.x() << ",  Min H = " << H_minmax.y() << std::endl;
    }else if(option == "-path"){
        std::vector<std::string> tifFiles;
        std::string dirpath = argv[2];
        tifFiles = findTifFiles(dirpath);
        for (auto tiffPath : tifFiles) {
            double start_lat;//leftTopLat
            double start_lon;//leftTopLon
            double end_lat;//rightBottomLat
            double end_lon;//rightBottomLon
            char *DemFilePath = argv[3];
            getTiffLatLon(tiffPath.data(), start_lat, start_lon, end_lat, end_lon);
            Eigen::Vector2i startXY;
            Eigen::Vector2i endXY;
            Eigen::Vector2d H_minmax;

            startXY = getDEMXY(start_lat, start_lon, DemFilePath);
            endXY = getDEMXY(end_lat, end_lon, DemFilePath);

            H_minmax = getDEMvalue(startXY, endXY, DemFilePath);
            std::cout << "====" << tiffPath << "======" << std::endl;
            std::cout << "(" << start_lat << ", " << start_lon << ")" << "---" << "(" << end_lat << ", " << end_lon << ")"
                      << ": Max H = " << H_minmax.x() << ",  Min H = " << H_minmax.y() << std::endl;
        }
    }
    return 1;
}