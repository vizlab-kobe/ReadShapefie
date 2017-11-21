#!/bin/sh
PROGRAM=${PWD##*/}
SHP_FILENAME=${HOME}/Work/Data/RiverBasin/5235/W07-09_5235-jgd_ValleyMesh.shp
SHX_FILENAME=${HOME}/Work/Data/RiverBasin/5235/W07-09_5235-jgd_ValleyMesh.shx
DBF_FILENAME=${HOME}/Work/Data/RiverBasin/5235/W07-09_5235-jgd_ValleyMesh.dbf
./$PROGRAM $SHP_FILENAME $SHX_FILENAME $DBF_FILENAME
