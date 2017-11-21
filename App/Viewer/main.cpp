#include <iostream>
#include <string>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/MersenneTwister>
#include <ReadShapefile/Lib/SHP.h>
#include <ReadShapefile/Lib/DBF.h>
#include <ReadShapefile/Lib/Importer.h>

#include <set>
#include <map>
#include <vector>

namespace
{

class RecordIndexMap
{
private:
    std::set<std::string> m_field_values; // set of field value
    std::multimap<std::string,int> m_bucket; // map of field value and record index

public:
    RecordIndexMap(
        const ReadShapefile::DBF& dbf,
        const int field_index )
    {
        for ( int i = 0; i < dbf.numberOfRecords(); i++ )
        {
            std::string key = dbf.attribute<std::string>( i, field_index );
            int value = i;
            m_bucket.insert( std::make_pair( key, value ) );
            m_field_values.insert( key );
        }
    }

    const std::set<std::string>& fieldValues() const { return m_field_values; }

    std::vector<int> find( const std::string& field_value ) const
    {
        typedef std::multimap<std::string,int> Map;
        typedef Map::const_iterator Itr;
        typedef std::pair<Itr,Itr> ItrPair;
        ItrPair range = m_bucket.equal_range( field_value );

        std::vector<int> record_indices;
        for ( Itr j = range.first; j != range.second; j++ )
        {
            const int record_index = j->second;
            record_indices.push_back( record_index );
        }
        return record_indices;
    }
};

kvs::LineObject* Import(
    const ReadShapefile::SHP& shp,
    const RecordIndexMap& record_index_map,
    const std::string& field_value,
    const kvs::RGBColor& color )
{
    std::vector<int> record_indices = record_index_map.find( field_value );

    std::vector<kvs::Real32> coords;
    std::vector<kvs::UInt32> connects;
    const int nrecords = record_indices.size();
    for ( int i = 0, id = 0; i < nrecords; i++ )
    {
        const int record_index = record_indices[i];
        ReadShapefile::SHP::Object object = shp.object( record_index );
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
    object->setColor( color );
    object->setSize( 1 );
    object->setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    object->setConnections( kvs::ValueArray<kvs::UInt32>( connects ) );
    object->setMinMaxObjectCoords( min, max );
    object->setMinMaxExternalCoords( min, max );
    return object;
}

} // end of namespace


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.show();

    const std::string shp_filename( argv[1] );
    ReadShapefile::SHP shp( shp_filename );
    shp.print( std::cout << "SHP Information" << std::endl, kvs::Indent(4) );

    const std::string dbf_filename( argv[3] );
    ReadShapefile::DBF dbf( dbf_filename );
    dbf.print( std::cout << "DBF Information" << std::endl, kvs::Indent(4) );

    if ( 0 )
    {
        kvs::LineObject* object = ReadShapefile::ImportPolygonBounds( shp );
        screen.registerObject( object );
    }

    const int field_index = 2; // 2: River code, 3: Drainage system code
    ::RecordIndexMap record_index_map( dbf, field_index );

    if ( 0 )
    {
        const std::string field_value( "2800290001" ); // Togagawa
        kvs::LineObject* object = ::Import( shp, record_index_map, field_value, kvs::RGBColor::Red() );
        screen.registerObject( object );
    }

    if ( 1 )
    {
        kvs::MersenneTwister R;
        for ( auto itr = record_index_map.fieldValues().begin();
              itr != record_index_map.fieldValues().end();
              itr++ )
        {
            const std::string field_value = *itr;
            const float r = R();
            const float g = R();
            const float b = R();
            const kvs::Vec3 c( r, g, b );
            kvs::LineObject* object = ::Import( shp, record_index_map, field_value, kvs::RGBColor( c ) );
            screen.registerObject( object );
        }
    }

    return app.run();
}
