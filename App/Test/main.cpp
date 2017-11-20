#include <shapefil.h>
#include <iostream>
#include <string>
#include <vector>
#include <kvs/Vector4>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/PointObject>
#include <kvs/LineObject>
#include <kvs/PolygonObject>


namespace
{

kvs::PointObject* ImportPoint( SHPHandle handler )
{
    std::vector<kvs::Real32> coords;
    const int nrecords = handler->nRecords;
    for ( int i = 0; i < nrecords; i++ )
    {
        SHPObject* shape = SHPReadObject( handler, i );
        const float x = shape->padfX[0];
        const float y = shape->padfY[0];
        const float z = 0.0f;
        coords.push_back( x );
        coords.push_back( y );
        coords.push_back( z );
    }

    const kvs::Vec3 min( handler->adBoundsMin[0], handler->adBoundsMin[1], handler->adBoundsMin[2] );
    const kvs::Vec3 max( handler->adBoundsMax[0], handler->adBoundsMax[1], handler->adBoundsMax[2] );

    kvs::PointObject* object = new kvs::PointObject();
    object->setColor( kvs::RGBColor::Black() );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    object->setMinMaxObjectCoords( min, max );
    object->setMinMaxExternalCoords( min, max );
    return object;
}

kvs::LineObject* ImportArc( SHPHandle handler )
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;
    const int nrecords = handler->nRecords;
    for ( int i = 0, id = 0; i < nrecords; i++ )
    {
        SHPObject* shape = SHPReadObject( handler, i );
        const int nvertices = shape->nVertices;
        for ( int j = 0; j < nvertices; j++ )
        {
            const float x = shape->padfX[j];
            const float y = shape->padfY[j];
            const float z = 0.0f;
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        }

        const int start = id;
        const int end = id + nvertices - 1;
        connects.push_back( start );
        connects.push_back( end );
        id += nvertices;
    }

    const kvs::Vec3 min( handler->adBoundsMin[0], handler->adBoundsMin[1], handler->adBoundsMin[2] );
    const kvs::Vec3 max( handler->adBoundsMax[0], handler->adBoundsMax[1], handler->adBoundsMax[2] );

    kvs::LineObject* object = new kvs::LineObject();
    object->setLineTypeToPolyline();
    object->setColor( kvs::RGBColor::Black() );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    object->setMinMaxObjectCoords( min, max );
    object->setMinMaxExternalCoords( min, max );
    return object;
}

kvs::LineObject* ImportPolygonBounds( SHPHandle handler )
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;
    const int nrecords = handler->nRecords;
    for ( int i = 0, id = 0; i < nrecords; i++ )
    {
        SHPObject* shape = SHPReadObject( handler, i );
        const int nvertices = shape->nVertices;
        for ( int j = 0; j < nvertices; j++ )
        {
            const float x = shape->padfX[j];
            const float y = shape->padfY[j];
            const float z = 0.0f;
            coords.push_back( x );
            coords.push_back( y );
            coords.push_back( z );
        }

        const int nparts = shape->nParts;
        for ( int j = 0; j < nparts; j++ )
        {
            const int start = id + shape->panPartStart[j];
            const int end = id + ( ( j + 1 < nparts ) ? shape->panPartStart[ j + 1 ] : nvertices ) - 1;
            connects.push_back( start );
            connects.push_back( end );
        }
        id += nvertices;
    }

    const kvs::Vec3 min( handler->adBoundsMin[0], handler->adBoundsMin[1], handler->adBoundsMin[2] );
    const kvs::Vec3 max( handler->adBoundsMax[0], handler->adBoundsMax[1], handler->adBoundsMax[2] );

    kvs::LineObject* object = new kvs::LineObject();
    object->setLineTypeToPolyline();
    object->setColor( kvs::RGBColor::Black() );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    object->setMinMaxObjectCoords( min, max );
    object->setMinMaxExternalCoords( min, max );
    return object;
}

}


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    const std::string filename( argv[1] );
    SHPHandle handler = SHPOpen( filename.c_str(), "rb" );
    switch ( handler->nShapeType )
    {
    case SHPT_POINT:
    {
        std::cout << "Shape type: Point" << std::endl;
        kvs::PointObject* object = ::ImportPoint( handler );
        screen.registerObject( object );
        break;
    }
    case SHPT_ARC:
    {
        std::cout << "Shape type: Arcs" << std::endl;
        kvs::LineObject* object = ::ImportArc( handler );
        screen.registerObject( object );
        break;
    }
    case SHPT_POLYGON:
    {
        std::cout << "Shape type: Polygons" << std::endl;
        kvs::LineObject* object = ::ImportPolygonBounds( handler );
        screen.registerObject( object );
        break;
    }
    default: break;
    }
    SHPClose( handler );

    return app.run();
}
