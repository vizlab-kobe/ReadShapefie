#include <iostream>
#include <string>
#include <ReadShapefile/Lib/SHP.h>
#include <ReadShapefile/Lib/DBF.h>


int main( int argc, char** argv )
{
    const std::string shp_filename( argv[1] );
    ReadShapefile::SHP shp( shp_filename );
    shp.print( std::cout << "SHP Information" << std::endl, kvs::Indent(4) );

    const std::string dbf_filename( argv[3] );
    ReadShapefile::DBF dbf( dbf_filename );
    dbf.print( std::cout << "DBF Information" << std::endl, kvs::Indent(4) );

    return 0;
}
