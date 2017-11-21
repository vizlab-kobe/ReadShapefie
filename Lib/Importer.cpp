#include "Importer.h"


namespace ReadShapefile
{

kvs::PointObject* ImportPoint( const ReadShapefile::SHP& shp )
{
    std::vector<kvs::Real32> coords;
    const int nrecords = shp.numberOfRecords();
    for ( int i = 0; i < nrecords; i++ )
    {
        ReadShapefile::SHP::Object object = shp.object(i);
        const float x = object.xCoord(0);
        const float y = object.yCoord(0);
        const float z = 0.0f;
        coords.push_back(x);
        coords.push_back(y);
        coords.push_back(z);
    }

    const kvs::Vec3 min( shp.boundsMin().xyz() );
    const kvs::Vec3 max( shp.boundsMax().xyz() );

    kvs::PointObject* object = new kvs::PointObject();
    object->setColor( kvs::RGBColor::Black() );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    object->setMinMaxObjectCoords( min, max );
    object->setMinMaxExternalCoords( min, max );
    return object;
}

kvs::LineObject* ImportArc( const ReadShapefile::SHP& shp )
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;
    const int nrecords = shp.numberOfRecords();
    for ( int i = 0, id = 0; i < nrecords; i++ )
    {
        ReadShapefile::SHP::Object object = shp.object(i);
        const int nvertices = object.numberOfVertices();
        for ( int j = 0; j < nvertices; j++ )
        {
            const float x = object.xCoord(j);
            const float y = object.yCoord(j);
            const float z = 0.0f;
            coords.push_back(x);
            coords.push_back(y);
            coords.push_back(z);
        }

        const int start = id;
        const int end = id + nvertices - 1;
        connects.push_back( start );
        connects.push_back( end );
        id += nvertices;
    }

    const kvs::Vec3 min( shp.boundsMin().xyz() );
    const kvs::Vec3 max( shp.boundsMax().xyz() );

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

kvs::LineObject* ImportPolygonBounds( const ReadShapefile::SHP& shp )
{
    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;
    const int nrecords = shp.numberOfRecords();
    for ( int i = 0, id = 0; i < nrecords; i++ )
    {
        ReadShapefile::SHP::Object object = shp.object(i);
        const int nvertices = object.numberOfVertices();
        for ( int j = 0; j < nvertices; j++ )
        {
            const float x = object.xCoord(j);
            const float y = object.yCoord(j);
            const float z = 0.0f;
            coords.push_back(x);
            coords.push_back(y);
            coords.push_back(z);
        }

        const int nparts = object.numberOfParts();
        for ( int j = 0; j < nparts; j++ )
        {
            const int start = id + object.startIndex(j);
            const int end = id + ( ( j + 1 < nparts ) ? object.startIndex( j + 1 ) : nvertices ) - 1;
            connects.push_back( start );
            connects.push_back( end );
        }
        id += nvertices;
    }

    const kvs::Vec3 min( shp.boundsMin().xyz() );
    const kvs::Vec3 max( shp.boundsMax().xyz() );

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

} // end of namespace ReadShapefile
