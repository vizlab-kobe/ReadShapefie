#include <shapefil.h>
#include <iostream>
#include <string>
#include <vector>
#include <kvs/Vector4>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/PointObject>


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    const std::string filename( argv[1] );

    SHPHandle handler = SHPOpen( filename.c_str(), "rb" );

    const kvs::Vec4 min( handler->adBoundsMin[0], handler->adBoundsMin[1], handler->adBoundsMin[2], handler->adBoundsMin[3] );
    const kvs::Vec4 max( handler->adBoundsMax[0], handler->adBoundsMax[1], handler->adBoundsMax[2], handler->adBoundsMax[3] );
    std::cout << "Bounds min: " << min << std::endl;
    std::cout << "Bounds max: " << max << std::endl;

    switch ( handler->nShapeType )
    {
    case SHPT_POINT:
    {
        std::cout << "Shape type: Point" << std::endl;
        break;
    }
    case SHPT_ARC:
    {
        std::cout << "Shape type: Arcs" << std::endl;
        break;
    }
    case SHPT_POLYGON:
    {
        std::cout << "Shape type: Polygons" << std::endl;
        break;
    }
    case SHPT_MULTIPOINT:
    {
        std::cout << "Shape type: Multipoint" << std::endl;
        break;
    }
    default: break;
    }

    const int nrecords = handler->nRecords;
    std::cout << "Number of records: " << nrecords << std::endl;

    std::vector<kvs::Real32> coords;
    for ( int i = 0; i < nrecords; i++ )
    {
        SHPObject* shape = SHPReadObject( handler, i );
        const int nvertices = shape->nVertices;
        for ( int j = 0; j < nvertices; j++ )
        {
            const float x = shape->padfX[j];
            const float y = shape->padfY[j];
            const float z = shape->padfZ[j];
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        }

    }

    SHPClose( handler );

    kvs::PointObject* object = new kvs::PointObject();
    object->setColor( kvs::RGBColor::Black() );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    screen.registerObject( object );

    return app.run();
}
