#include "SymbolInfo.h"

static std::mutex gs_Mutex;
long long int SymbolInfo::gs_LasTimeStamp = 0;

bool SymbolInfo::Tokenize(const string &aLine, const char delim)
{
    /*
     * aLine: <TimeStamp>,<Symbol>,<Quantity>,<Price>
     */
    istringstream tokStr(aLine);
    string token;
    int nxt = SymColIdx::TIME_STAMP;

    while(std::getline(tokStr, token, delim))
    {
        switch(nxt)
        {
            case SymColIdx::TIME_STAMP:
                //m_aSymbolInfo[nxt++] = std::stoll(token);
                m_aSymbolInfo[nxt++] = std::stol(token);
                break;
            case SymColIdx::SYMBOL:
                m_aSymbolInfo[nxt++] = token;
                break;
            case SymColIdx::VOLUME:
                m_aSymbolInfo[nxt++] = std::stoi(token);
                break;
            case SymColIdx::PRICE:
                m_aSymbolInfo[nxt++] = std::stod(token);
                break;
            default:
                return false;
        }
    }

    return true;
}

SymbolInfo::SymbolInfo(const string & TimeSeriesData, const char delim)
{
    lock_guard<std::mutex> lck(gs_Mutex);
    
    m_aSymbolInfo.resize(SymColIdx::NUM_TIME_SERIES_COL);

    if (!Tokenize(TimeSeriesData, delim))
    {
        throw BadInputException("Invalid input line/time series data");
    }

    string out_msg;
    if (!IsValidInputStream(out_msg))
    {
        throw BadInputException(out_msg);
    }
    gs_LasTimeStamp = std::any_cast<long int>(m_aSymbolInfo[SymColIdx::TIME_STAMP]);
}

bool SymbolInfo::IsValidInputStream(string &out_msg) const
{
    try
    {
        /*
         * The following line throws bad_any_cast exception 
         * long long int ts = std::any_cast<long long int> (m_aSymbolInfo[SymColIdx::TIME_STAMP]);
         */
        out_msg = " of TIME_STAMP column, type=long int";
        
        long int ts = std::any_cast<long int> (m_aSymbolInfo[SymColIdx::TIME_STAMP]);
        
        if (ts <= 0)
        {
            out_msg = "Time stamp value <= 0!";
            return false;
        }
        if (ts < gs_LasTimeStamp)
        {
            out_msg = "Current time stamp is less that last time stamp!";
            return false;
        }

        out_msg = " of SYMBOL column, type=std::string";
        string sym = std::any_cast<std::string> (m_aSymbolInfo[SymColIdx::SYMBOL]);
        if (sym.length() == 0)
        {
            out_msg = "Invalid symbol!";
            return false;
        }

        out_msg = " of VOLUME column, type=int";
        int qty = std::any_cast<int>(m_aSymbolInfo[SymColIdx::VOLUME]);
        if (qty <= 0)
        {
            out_msg = "Share value <= 0!";
            return false;
        }
        out_msg = " of PRICE column, type=double";
        double px = std::any_cast<double>(m_aSymbolInfo[SymColIdx::PRICE]);
        if (px <= 0.0)
        {
            out_msg = "Price is less than or equal to 0!";
            return false;
        }
    }
    catch (std::bad_any_cast & e)
    {
        out_msg = e.what() + out_msg;
        return false;
    }

    out_msg = "";
    return true;
}

ostream & operator << (ostream & out, const SymbolInfo & si)
{
    try
    {
        out << std::any_cast<long int>(si.m_aSymbolInfo[SymbolInfo::SymColIdx::TIME_STAMP]) <<","
            << std::any_cast<std::string>(si.m_aSymbolInfo[SymbolInfo::SymColIdx::SYMBOL]) <<","
            << std::any_cast<int>(si.m_aSymbolInfo[SymbolInfo::SymColIdx::VOLUME]) << ","
            << std::any_cast<double>(si.m_aSymbolInfo[SymbolInfo::SymColIdx::PRICE]);
    }
    catch(std::bad_any_cast & e)
    {
        out << e.what();
    }
    return out;
}


