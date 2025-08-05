# DenToH
# Using DEM to obtain the max and min elevations within a certain latitude and longitude range
# The File named 'DemToH' is executable program, if your Linux computer has Eigen, GADL, and Boost, you can run directly.
There must be supported by Eigen GDAL Boost Lib.
=========================== DEM_To_H Use Method ===================
           : .exe -txt [leftTopLat] [leftTopLon] [rightBottomLat] [rightBottomLon] [DEMFilePath]
           : .exe -tif [tifFilePath] [DEMFilePath]
           : .exe -path [dirPath] [DEMFilePath]
           : ! Lat and Lon -> (North, East)
Tiff   Image: 
           startLatLon----------N--------------------------- 
                -                                      - 
                -                                      - 
                -                                      - 
                W                                      E
                -                                      - 
                -                                      - 
                -                                      - 
                -                                      - 
           ---------------------S---------------  endLatLon 
