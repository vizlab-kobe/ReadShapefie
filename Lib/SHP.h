#pragma once
#include <shapefil.h>
#include <string>
#include <iostream>
#include <kvs/Vector4>
#include <kvs/ValueArray>
#include <kvs/Indent>


namespace ReadShapefile
{

class SHP
{
public:
    enum ShapeType
    {
        Null = SHPT_NULL,
        Point = SHPT_POINT,
        Arc = SHPT_ARC,
        Polygon = SHPT_POLYGON,
        MultiPoint = SHPT_MULTIPOINT,
        PointZ = SHPT_POINTZ,
        ArcZ = SHPT_ARCZ,
        PolygonZ = SHPT_POLYGONZ,
        MultiPointZ = SHPT_MULTIPOINTZ,
        PointM = SHPT_POINTM,
        ArcM = SHPT_ARCM,
        PolygonM = SHPT_POLYGONM,
        MultiPointM = SHPT_MULTIPOINTM,
        MultiPatch = SHPT_MULTIPATCH
    };

    enum PartType
    {
        TriStrip = SHPP_TRISTRIP,
        TriFan = SHPP_TRIFAN,
        OuterRing = SHPP_OUTERRING,
        InnerRing = SHPP_INNERRING,
        FirstRing = SHPP_FIRSTRING,
        Ring = SHPP_RING
    };

    class Object
    {
    private:
        SHPObject* m_handler;

    public:
        Object( SHPObject* handler ): m_handler( handler ) {}

        SHPObject* handler() { return m_handler; }
        int shapeType() const { return m_handler->nSHPType; }
        int shapeID() const { return m_handler->nShapeId; }
        int numberOfParts() const { return m_handler->nParts; }
        int numberOfVertices() const { return m_handler->nVertices; }
        const int* pointerToPartStartIndices() const { return m_handler->panPartStart; }
        const int* pointerToPartTypes() const { return m_handler->panPartType; }
        const double* pointerToXCoords() const { return m_handler->padfX; }
        const double* pointerToYCoords() const { return m_handler->padfY; }
        const double* pointerToZCoords() const { return m_handler->padfZ; }
        const double* pointerToMCoords() const { return m_handler->padfM; }
        kvs::Vec4d boundsMin() const;
        kvs::Vec4d boundsMax() const;
        int startIndex( const int part_index ) const;
        int partType( const int part_index ) const;
        kvs::ValueArray<int> startIndices() const;
        kvs::ValueArray<int> partTypes() const;
        double xCoord( const int index ) const;
        double yCoord( const int index ) const;
        double zCoord( const int index ) const;
        double mCoord( const int index ) const;
        kvs::ValueArray<double> xCoords() const;
        kvs::ValueArray<double> yCoords() const;
        kvs::ValueArray<double> zCoords() const;
        kvs::ValueArray<double> mCoords() const;
        std::string shapeTypeName() const;
        std::string partTypeName( const int part_index ) const;
    };

private:
    SHPHandle m_handler;

public:
    SHP( const std::string& filename, const std::string& access = "rb" );
    ~SHP();

    SHPHandle handler() { return m_handler; }
    bool isValid() const { return m_handler != NULL; }
    int shapeType() const { return m_handler->nShapeType; }
    int numberOfRecords() const { return m_handler->nRecords; }
    int maxRecords() const { return m_handler->nMaxRecords; }
    kvs::Vec4d boundsMin() const { return kvs::Vec4d( m_handler->adBoundsMin ); }
    kvs::Vec4d boundsMax() const { return kvs::Vec4d( m_handler->adBoundsMax ); }
    void open( const std::string& filename, const std::string& access = "rb" );
    void close();
    std::string shapeTypeName() const;
    Object object( const int record_index ) const;
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
};

} // end of namespace ReadShapefile
