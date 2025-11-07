#ifndef TRADE_STAT_H_INCLUDED
#define TRADE_STAT_H_INCLUDED

#include <bits/stdc++.h>
#include "SymbolInfo.h"

using namespace std;

/*
 * Helper for GetOutputStringV4()
 */
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

class TradeMatrix
{
    private:
        
        class SymbolMatrix
        {
            private:
                enum MatrixColumn
                {
                    LAST_TRADE_TIME = 0, // type = long long int
                    TOTAL_VOLUME = 1,    // type = int
                    MAX_PRICE = 2,       // type = double
                    MAX_GAP = 3,         // type = long long int
                    TOTAL_WEIGHT = 4,    // type = double
                    // Add new column as needed
                    
                    TOTAL_TRADE_COLUMN
                };
                
                using ColumnType = std::variant<int, double, long long int>;

                /*
                 * Info of below vector for a gievn symbol:
                 * Index 0 is for LAST_TRADE_TIME
                 * Index 1 is for TOTAL_VOLUME
                 * Index 2 is for MAX_PRICE
                 * ...
                 * and so on
                 */
                vector<ColumnType> m_TradeColumns;

                struct Visitor
                {
                    private:
                        string m_Out;

                    public:
                        void operator()(const int i)
                        {
                            m_Out += "," + std::to_string(i);
                        }

                        void operator()(const double d)
                        {
                            m_Out += "," + std::to_string(d);
                        }

                        void operator()(const long long int i)
                        {
                            m_Out += "," + std::to_string(i);
                        }

                        string GetOutString() const
                        {
                            return m_Out;
                        }
                };

            public:
                SymbolMatrix() = default;

                SymbolMatrix(const SymbolInfo & symbol);
                //SymbolMatrix(const SymbolMatrix & rhs) = default; 
                //SymbolMatrix & operator = (const SymbolMatrix & rhs) = default;

                void UpdateMatrix(const SymbolInfo & si);

                long long int GetMostRecentTradeTime() const
                {
                    long long int tt = 0;
                    try
                    {
                        tt = std::get<2>(m_TradeColumns[LAST_TRADE_TIME]);
                        //tt = std::get<long long int>(m_TradeColumns[LAST_TRADE_TIME]);
                    }
                    catch (std::bad_variant_access & e)
                    {
                        cerr<<"ERROR: GetMostRecentTradeTime - "<<e.what()<<endl;
                        tt = -1;
                    }
                    return tt;
                }
                int GetVolume() const
                {
                    int v = 0;

                    try
                    {
                        v = std::get<int>(m_TradeColumns[TOTAL_VOLUME]);
                    }
                    catch(std::bad_variant_access& e)
                    {
                        cerr<<"ERROR::GetVolume - "<<e.what()<<endl;
                        v = -1;
                    }
                    return v;
                }

                double GetMaxPrice() const
                {
                    double mp = 0;

                    try
                    {
                        mp = std::get<double>(m_TradeColumns[MAX_PRICE]);
                    }
                    catch(std::bad_variant_access & e)
                    {
                        cerr<<"ERROR:GetMaxPrice - "<<e.what()<<endl;
                        mp = -1;
                    }

                    return mp;
                }

                long long int GetMaxGap() const
                {
                    long long int mg = 0;

                    try
                    {
                        mg = std::get<long long int>(m_TradeColumns[MAX_GAP]);
                    }
                    catch(std::bad_variant_access& e)
                    {
                        cerr<<"ERROR:GetMaxGap - "<<e.what()<<endl;
                        mg = -1;
                    }

                    return mg;
                }

                double GetTotalWeight() const
                {
                    double tw = 0.0;

                    try
                    {
                        tw = std::get<double>(m_TradeColumns[TOTAL_WEIGHT]);
                    }
                    catch(std::bad_variant_access& e)
                    {
                        cerr<<"ERROR:GetTotalWeight - "<<e.what()<<endl;
                        tw = -1;
                    }

                    return tw;
                }

                double GetWeightedAvgPrc() const
                {
                    return (GetTotalWeight() / GetVolume());
                }

                /*
                 * Return matrix for all columns of a symbol
                 */
                string GetOutputString(const string & sym) const
                {
                    /*
                     * <Symbol>,<MaxTimeGap>,<TotalVolume>,<MaxPrice>,<WeightedAvgPrice>
                     */
                    vector<int> column = {MAX_GAP, TOTAL_VOLUME, MAX_PRICE};

#if 0
                    // OK
                    string s = GetOutputStringV1(sym, column) + "," 
                               + std::to_string(GetWeightedAvgPrc());

                    // OK
                    string s = GetOutputStringV2(sym, column) + "," 
                               + std::to_string(GetWeightedAvgPrc());

                    // OK
                    string s = GetOutputStringV3(sym, column) + "," 
                               + std::to_string(GetWeightedAvgPrc());
#endif
                    // OK
                    string s = GetOutputStringV4(sym, column) + "," 
                               + std::to_string(GetWeightedAvgPrc());
                    return s; 
                }

                /*
                 * Return matrix for given columns of a symbol
                 */
                string GetOutputStringV1(const string &sym, const vector<int> & Columns) const
                {
                    struct Visitor vtor;

                    for( auto i : Columns)
                    {
                        // a void visitor
                        std::visit(vtor, m_TradeColumns[i]);
                    }

                    return (sym + vtor.GetOutString());
                }
                

                /*
                 * A template to check for missing type in following lambda code
                 */
                template<typename T> struct always_false : std::false_type {};

                string GetOutputStringV2(const string &sym, const vector<int> &columns) const
                {
                    string out = sym;

                    for (auto i : columns)
                    {
                        /*
                         * type matching visitor: a lambda that handles each 
                         * type differently and update a captured parameter
                         */
                        std::visit([&out](auto&& arg)
                        // std::visit([&](auto&& arg) - this also will work 
                        {
                            using T1 = std::decay_t<decltype(arg)>;

                            if constexpr (std::is_same_v<T1, int> ||
                                          std::is_same_v<T1, double> ||
                                          std::is_same_v<T1, long long int>)
                            {
                                out += "," + std::to_string(arg);
                            }
                            else
                            {
                                static_assert(always_false<T1>::value, "missing visitor for a data type!!");
                            }

                        }, m_TradeColumns[i]);
                    }

                    return out;
                }

                string GetOutputStringV3(const string & sym, const vector<int> & columns) const
                {
                    string out = sym;

                    for (auto i : columns)
                    {
                        /*
                         * type matching value returning visitor: a lambda 
                         * that handles each type differently and returns a 
                         * string
                         */
                        string s1 = std::visit([](auto &&arg) -> string 
                                {
                                    using T1 = std::decay_t<decltype(arg)>;

                                    if constexpr (std::is_same_v<T1, int> ||
                                                  std::is_same_v<T1, double> ||
                                                  std::is_same_v<T1, long long int>)
                                    {
                                        return std::to_string(arg);
                                    }
                                    else
                                    {
                                        static_assert(always_false<T1>::value, "missing visitor for a data type!!");
                                        return "";
                                    }

                                }, m_TradeColumns[i]);

                        out += "," + s1;
                    }
                    return out;
                }

                //template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
                //template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

                string GetOutputStringV4(const string &sym, const vector<int> &columns) const
                {
                    string out = sym;

                    for (auto i : columns)
                    {
                        /*
                         * Another type matching visitor, a class with
                         * 3 overloaded operartor()'s
                         */
                        std::visit(overloaded {
                                [&out](int arg) { out += "," + std::to_string(arg); },
                                [&out](double arg) {out += "," + std::to_string(arg); },
                                [&out](long long int arg) {out += "," + std::to_string(arg); },
                                [&out](auto arg) { out += "," + std::to_string(arg); },

                         }, m_TradeColumns[i]);

                    }
                    return out;
                }
        };

        /*
         * Here, We are using unordered_map instead of map for efficient
         * processing of time series data during market open. The assumption
         * is that writing the matrix to outfile will take place after market 
         * close when there is time series data.
         *
         * But if we ever need to write (sorted by symbol) matrix during 
         * market open, map may be used instead of unordered_map
         */
        unordered_map<string, SymbolMatrix> m_Sym2Matrix;

    public:
        void UpdateSymbolMatrix(const string & time_series_data, const string & delim);

        /*
         * Writing to out file (in sorted order by symbol) will happen
         * after market close when there is no time series data.
         * So we need to sort data from m_Sym2Matrix before writing to file.
         */
        void WriteMatrixToOutFile(const char * outfile);
};
#endif
