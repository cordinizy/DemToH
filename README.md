# DenToH

A tool for extracting maximum and minimum elevations from DEM (Digital Elevation Model) data within specified geographic coordinates.

## Dependencies
- Eigen
- GDAL
- Boost

## Installation
If your Linux system has Eigen, GDAL, and Boost libraries installed, you can run the executable directly.

## Usage

### Command Line Options:

1. **Using latitude/longitude coordinates:**
   ./DemToH -txt [leftTopLat] [leftTopLon] [rightBottomLat] [rightBottomLon] [DEMFilePath]
   - Coordinates format: (North, East)

2. **Using TIFF image bounds:**
   ./DemToH -tif [tifFilePath] [DEMFilePath]

3. **Processing all TIFF files in a directory:**
   ./DemToH -path [dirPath] [DEMFilePath]

## Coordinate System Explanation

For TIFF images:
- startLatLon--------------N---------------------------
-     |                                       |
-     |                                       |
-     |                                       |
-     W                                       E
-     |                                       |
-     |                                       |
-     |                                       |
- -------------------------S---------------endLatLon


- The image is oriented with North at the top, East at the right
- `startLatLon` represents the northwest (top-left) corner
- `endLatLon` represents the southeast (bottom-right) corner
