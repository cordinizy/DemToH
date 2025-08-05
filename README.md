# DenToH
## Using DEM to obtain the max and min elevations within a certain latitude and longitude range
## The File named 'DemToH' is executable program, if your Linux computer has Eigen, GADL, and Boost, you can run directly.
## There must be supported by Eigen GDAL Boost Lib.

1.=========================== DEM_To_H Use Method ===================
2.           : .exe -txt [leftTopLat] [leftTopLon] [rightBottomLat] [rightBottomLon] [DEMFilePath]
3.           : .exe -tif [tifFilePath] [DEMFilePath]
4.           : .exe -path [dirPath] [DEMFilePath]
5.           : ! Lat and Lon -> (North, East)
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
