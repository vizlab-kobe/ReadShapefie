#include "DBF.h"


namespace
{

inline std::string FieldTypeName( const int field_type )
{
    switch ( field_type )
    {
    case ReadShapefile::DBF::String: return "String";
    case ReadShapefile::DBF::Integer: return "Integer";
    case ReadShapefile::DBF::Double: return "Double";
    case ReadShapefile::DBF::Logical: return "Logical";
    case ReadShapefile::DBF::Invalid: return "Invalid";
    default: break;
    }
    return "Unknown";
}

}


namespace ReadShapefile
{

DBF::DBF( const std::string& filename, const std::string& access ):
    m_handler( NULL )
{
    this->open( filename, access );
}

DBF::~DBF()
{
    this->close();
}

void DBF::open( const std::string& filename, const std::string& access )
{
    m_handler = DBFOpen( filename.c_str(), access.c_str() );
}

void DBF::close()
{
    DBFClose( m_handler );
}

int DBF::numberOfFields() const
{
    return DBFGetFieldCount( m_handler );
}

int DBF::numberOfRecords() const
{
    return DBFGetRecordCount( m_handler );
}

int DBF::fieldIndexOf( const std::string& field_name ) const
{
    return DBFGetFieldIndex( m_handler, field_name.c_str() );
}

DBF::FieldInfo DBF::fieldInfo( const int index ) const
{
    char name[12];
    int width;
    int decimal;
    DBFFieldType type = DBFGetFieldInfo( m_handler, index, name, &width, &decimal );

    FieldInfo info;
    info.type = FieldType( type );
    info.name = std::string( name );
    info.width = width;
    info.decimal = decimal;
    return info;
}

void DBF::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Number of records: " << this->numberOfRecords() << std::endl;
    os << indent << "Number of fields: " << this->numberOfFields() << std::endl;
    for ( int i = 0; i < this->numberOfFields(); i++ )
    {
        FieldInfo info = this->fieldInfo( i );
        os << indent << "Field " << i << ": " << info.name << std::endl;
        os << indent.nextIndent() << "Type: " << ::FieldTypeName( info.type ) << std::endl;
        os << indent.nextIndent() << "Width: " << info.width << std::endl;
        os << indent.nextIndent() << "Decimal: " << info.decimal << std::endl;
    }
}

} // end of namespace ReadShapefile
