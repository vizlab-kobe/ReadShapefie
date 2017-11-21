#include "SHP.h"
#include <kvs/Assert>


namespace ReadShapefile
{

kvs::Vec4d SHP::Object::boundsMin() const
{
    return kvs::Vec4d( m_handler->dfXMin, m_handler->dfYMin, m_handler->dfZMin, m_handler->dfMMin );
}

kvs::Vec4d SHP::Object::boundsMax() const
{
    return kvs::Vec4d( m_handler->dfXMax, m_handler->dfYMax, m_handler->dfZMax, m_handler->dfMMax );
}

int SHP::Object::startIndex( const int part_index ) const
{
    KVS_ASSERT( part_index < this->numberOfParts() );
    return m_handler->panPartStart[ part_index ];
}

int SHP::Object::partType( const int part_index ) const
{
    KVS_ASSERT( part_index < this->numberOfParts() );
    return m_handler->panPartType[ part_index ];
}

kvs::ValueArray<int> SHP::Object::startIndices() const
{
    return kvs::ValueArray<int>( m_handler->panPartStart, m_handler->nParts );
}

kvs::ValueArray<int> SHP::Object::partTypes() const
{
    return kvs::ValueArray<int>( m_handler->panPartType, m_handler->nParts );
}

double SHP::Object::xCoord( const int index ) const
{
    KVS_ASSERT( index < this->numberOfVertices() );
    return m_handler->padfX[index];
}

double SHP::Object::yCoord( const int index ) const
{
    KVS_ASSERT( index < this->numberOfVertices() );
    return m_handler->padfY[index];
}

double SHP::Object::zCoord( const int index ) const
{
    KVS_ASSERT( index < this->numberOfVertices() );
    return m_handler->padfZ[index];
}

double SHP::Object::mCoord( const int index ) const
{
    KVS_ASSERT( index < this->numberOfVertices() );
    return m_handler->padfM[index];
}

kvs::ValueArray<double> SHP::Object::xCoords() const
{
    return kvs::ValueArray<double>( m_handler->padfX, m_handler->nVertices );
}

kvs::ValueArray<double> SHP::Object::yCoords() const
{
    return kvs::ValueArray<double>( m_handler->padfY, m_handler->nVertices );
}

kvs::ValueArray<double> SHP::Object::zCoords() const
{
    return kvs::ValueArray<double>( m_handler->padfZ, m_handler->nVertices );
}

kvs::ValueArray<double> SHP::Object::mCoords() const
{
    return kvs::ValueArray<double>( m_handler->padfM, m_handler->nVertices );
}

std::string SHP::Object::shapeTypeName() const
{
    return std::string( SHPTypeName( this->shapeType() ) );
}

std::string SHP::Object::partTypeName( const int part_index ) const
{
    return std::string( SHPPartTypeName( this->partType( part_index ) ) );
}

SHP::SHP( const std::string& filename, const std::string& access ):
    m_handler( NULL )
{
    this->open( filename, access );
}

SHP::~SHP()
{
    this->close();
}

void SHP::open( const std::string& filename, const std::string& access )
{
    m_handler = SHPOpen( filename.c_str(), access.c_str() );
}

void SHP::close()
{
    SHPClose( m_handler );
}

std::string SHP::shapeTypeName() const
{
    return SHPTypeName( this->shapeType() );
}

SHP::Object SHP::object( const int record_index ) const
{
    return Object( SHPReadObject( m_handler, record_index ) );
}

void SHP::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Shape type: " << this->shapeTypeName() << std::endl;
    os << indent << "Number of records: " << this->numberOfRecords() << std::endl;
    os << indent << "Bounds min.: " << this->boundsMin() << std::endl;
    os << indent << "Bounds max.: " << this->boundsMax() << std::endl;
}

} // end of namespace ReadShapefile
