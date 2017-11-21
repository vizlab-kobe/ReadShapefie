#include <kvs/PointObject>
#include <kvs/LineObject>
#include <kvs/PolygonObject>
#include "SHP.h"


namespace ReadShapefile
{

kvs::PointObject* ImportPoint( const ReadShapefile::SHP& shp );
kvs::LineObject* ImportArc( const ReadShapefile::SHP& shp );
kvs::LineObject* ImportPolygonBounds( const ReadShapefile::SHP& shp );

} // end of namespace ReadShapefile
