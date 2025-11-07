#ifndef INPUTFILE_READER_H_INCLUDED
#define INPUTFILE_READER_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

class InputFileReader
{
    private:
        string m_FileName = "";
        string m_Delimeter = ",";
        long long int m_LineNumber = 0; // Which line is just read, for debugging of input
        ifstream m_FileDescriptor;

        void OpenFileForRead();

    public:
        InputFileReader() = default;

        InputFileReader(const string & fn, const string & d = ","): 
            m_FileName(fn), 
            m_Delimeter(d),
            m_LineNumber(0)
        {
            OpenFileForRead();
        }

        ~InputFileReader()
        {
            if (m_FileDescriptor.is_open())
                m_FileDescriptor.close();
        }

        bool IsEOF() const
        {
            return m_FileDescriptor.eof();
        }

        void SetFileName(const string & fn)
        {
            if (m_FileDescriptor.is_open())
                m_FileDescriptor.close();

            m_FileName = fn;
            OpenFileForRead();
        }

        void SetDelimeter(const string & d)
        {
            m_Delimeter = d;
        }

        string GetDelimeter() const
        {
            return m_Delimeter;
        }

        InputFileReader(const InputFileReader & rhs) = delete;

        InputFileReader & operator = (const InputFileReader & rhs) = delete;

        std::optional<std::string> ReadNextLine();
};


#endif
