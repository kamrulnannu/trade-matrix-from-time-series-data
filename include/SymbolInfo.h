#ifndef SYMBOL_INFO_H_INCLUDED
#define SYMBOL_INFO_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

struct BadInputException : public exception
{
    private:
        const string m_ExceptionMsg;

    public:
        BadInputException(const string & msg) : m_ExceptionMsg(msg)
        {}

       const char * what () const noexcept
       {
            return m_ExceptionMsg.c_str();
       } 
};

class SymbolInfo
{
    /*
     * This class will have info of a line from input file (i.e. Time series
     * data)
     */
    public:
        enum SymColIdx
        {
            TIME_STAMP = 0, // type = long int
            SYMBOL = 1,     // type = std::string
            VOLUME = 2,     // type = int
            PRICE = 3,      // type = int
            // Add new column here as needed

            NUM_TIME_SERIES_COL
        };
    private:
        static long long int gs_LasTimeStamp; // Time stamp from previous input line

        /*
         * The below vector will info of a symbol:
         * Index 0 is for trade TIME_STAMP
         * Index 1 is for SYMBOL
         * Index 2 is for VOLUME or quantity trade
         * Index 3 is trade PRICE
         * ... and so on
         */
        vector<std::any> m_aSymbolInfo;

        bool IsValidInputStream(string &out_msg) const;
        bool Tokenize(const string & aLine, char delim);

    public:
        SymbolInfo() = default;

        SymbolInfo(const string & TimeSeriesData, const char delim = ',');

        SymbolInfo(const SymbolInfo &rhs) = default;

        SymbolInfo & operator = (const SymbolInfo & rhs) = default;

        long long int GetTimeStamp() const
        {
            return std::any_cast<long int>(m_aSymbolInfo[SymColIdx::TIME_STAMP]);
        }

        string GetSymbol() const
        {
            return std::any_cast<std::string> (m_aSymbolInfo[SymColIdx::SYMBOL]);
        }

        int GetShare() const
        {
            return std::any_cast<int>(m_aSymbolInfo[SymColIdx::VOLUME]);
        }

        double GetPrice() const
        {
            return std::any_cast<double>(m_aSymbolInfo[SymColIdx::PRICE]);
        }

        /*
         * For debugging
         */
        friend ostream & operator << (ostream & out, const SymbolInfo & si);
};

#endif
