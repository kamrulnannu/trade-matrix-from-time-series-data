#include "InputFileReader.h"
//#include <boost/algorithm/string.hpp>

void InputFileReader::OpenFileForRead()
{
    m_FileDescriptor.open(m_FileName.c_str(), ios::in);

    if (!m_FileDescriptor)
    {
        cerr <<"ERROR: OpenFileForRead - Can not open input file for read, fn=" 
             << m_FileName << endl;
        exit(1);
    }
}

std::optional<std::string> InputFileReader::ReadNextLine()
{
    if (!m_FileDescriptor.is_open())
    {
        cerr<<"ERROR: ReadNextLine - Input is not open, fn = "<<m_FileName << endl;
        exit(2);
    }

    if (IsEOF())
    {
        cerr << "WARN: ReadNextLine - End of file"<<endl;
        return nullopt;  // Indicator of end of file
    }

    std::string aLine;

    m_LineNumber++;

    /*
     * Expected Input line:
     * <TimeStamp>,<Symbol>,<Share/Volume>,<Price>
     */
    if (std::getline(m_FileDescriptor, aLine))
    {
        return aLine;
        // or make_optional<std::string>(aLine);
        //boost::algorithm::split(words, aLine, boost::is_any_of(m_Delimeter));
    }

    if (!IsEOF())
    {
        cerr<<"WARN: ReadNextLine - getline failed."
            <<" Line Num="<< m_LineNumber << endl;
    }
    return { }; // Indicator of invalid input line or eof or getline error
}
