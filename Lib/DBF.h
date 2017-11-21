#pragma once
#include <shapefil.h>
#include <string>
#include <kvs/Indent>
#include <kvs/Assert>


namespace ReadShapefile
{

class DBF
{
public:
    enum FieldType
    {
        String = FTString,
        Integer = FTInteger,
        Double = FTDouble,
        Logical = FTLogical,
        Invalid = FTInvalid
    };

    struct FieldInfo
    {
        FieldType type;
        std::string name;
        int width;
        int decimal;
    };

private:
    DBFHandle m_handler;

public:
    DBF( const std::string& filename, const std::string& access = "rb" );
    ~DBF();

    DBFHandle handler() { return m_handler; }
    bool isValid() const { return m_handler != NULL; }
    void open( const std::string& filename, const std::string& access = "rb" );
    void close();
    int numberOfFields() const;
    int numberOfRecords() const;
    int fieldIndexOf( const std::string& field_name ) const;
    FieldInfo fieldInfo( const int index ) const;
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

    template <typename T>
    T attribute( const int record_index, const int field_index ) const { return T(0); }
};

template <>
inline int DBF::attribute<int>( const int record_index, const int field_index ) const
{
    KVS_ASSERT( this->fieldInfo( field_index ).type == DBF::Integer );
    return DBFReadIntegerAttribute( m_handler, record_index, field_index );
}

template <>
inline double DBF::attribute<double>( const int record_index, const int field_index ) const
{
    KVS_ASSERT( this->fieldInfo( field_index ).type == DBF::Double );
    return DBFReadDoubleAttribute( m_handler, record_index, field_index );
}

template <>
inline std::string DBF::attribute<std::string>( const int record_index, const int field_index ) const
{
    KVS_ASSERT( this->fieldInfo( field_index ).type == DBF::String );
    return std::string( DBFReadStringAttribute( m_handler, record_index, field_index ) );
}

} // end of namespace ReadShapefile
