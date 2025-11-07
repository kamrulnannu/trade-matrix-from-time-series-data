#include "TradeMatrix.h"

TradeMatrix::SymbolMatrix::SymbolMatrix(const SymbolInfo &si)
{
    m_TradeColumns.resize (TradeMatrix::SymbolMatrix::TOTAL_TRADE_COLUMN);
    
    ColumnType ts {std::in_place_type<long long int>, si.GetTimeStamp()};
    m_TradeColumns[SymbolMatrix::LAST_TRADE_TIME] = ts;

    ColumnType sh {std::in_place_index<0>, si.GetShare()};  // int
    m_TradeColumns[SymbolMatrix::TOTAL_VOLUME] = sh;

    ColumnType px {std::in_place_index<1>, si.GetPrice()}; // double
    m_TradeColumns[SymbolMatrix::MAX_PRICE] = px;
    
    ColumnType gp {std::in_place_type<long long int>, 0};
    m_TradeColumns[SymbolMatrix::MAX_GAP] = gp;

    ColumnType ct {std::in_place_type<double>, si.GetShare() * si.GetPrice()};
    m_TradeColumns[SymbolMatrix::TOTAL_WEIGHT] = ct;
}

void TradeMatrix::SymbolMatrix::UpdateMatrix(const SymbolInfo & si)
{

    auto pInt = std::get_if<int>(&m_TradeColumns[TOTAL_VOLUME]);
    *pInt += si.GetShare();

    auto pDbl = std::get_if<double>(&m_TradeColumns[MAX_PRICE]);
    *pDbl = std::max(*pDbl, si.GetPrice());

    auto pLtt  = std::get_if<long long int>(&m_TradeColumns[LAST_TRADE_TIME]);
    auto pLong = std::get_if<long long int>(&m_TradeColumns[MAX_GAP]);
    *pLong = std::max(*pLong,
                      si.GetTimeStamp() - *pLtt);

    *pLtt = si.GetTimeStamp();

    pDbl = std::get_if<double>(&m_TradeColumns[TOTAL_WEIGHT]);

    *pDbl += si.GetShare() * si.GetPrice();
}

void TradeMatrix::UpdateSymbolMatrix(const string &time_series_data, const string & delim)
{
    try
    {
        SymbolInfo si(time_series_data, delim[0]);

        auto itr = m_Sym2Matrix.find(si.GetSymbol());

        if (itr == m_Sym2Matrix.end())
        {
            // This symbol is seen 1st time
            SymbolMatrix ss (si);

            m_Sym2Matrix[si.GetSymbol()] = ss;
        }
        else
        {
            itr->second.UpdateMatrix(si);
        }
    }
    catch (BadInputException & e)
    {
        cerr<<"TradeMatrix::UpdateSymbolMatrix: "<<e.what()<<endl;
    }
}

void TradeMatrix::WriteMatrixToOutFile(const char *outfile)
{
    if (!outfile)
    {
        cerr<<"ERROR: WriteMatrixToOutFile - NULL out file"<<endl;
        return;
    }

    fstream outFd;

    outFd.open(outfile, ios::out);

    if (!outFd)
    {
        cerr <<"ERROR: WriteMatrixToOutFile - could not open file to write, fn="
             << outfile << endl;
        exit(3);
    }

    map<string, SymbolMatrix> Sym2Mat;

    auto itr = m_Sym2Matrix.begin();
    while (itr != m_Sym2Matrix.end())
    {
        Sym2Mat.emplace(itr->first, itr->second);
        itr = m_Sym2Matrix.erase(itr);
    }

    auto mitr = Sym2Mat.begin();

    while (mitr != Sym2Mat.end())
    {
        string out_line = mitr->second.GetOutputString(mitr->first);

        outFd << out_line << endl;

        mitr++;
    }

    outFd.close();
}
