#include "TradeMatrix.h"
#include "InputFileReader.h"

class Solution
{
    private:
        InputFileReader m_InFile;
        const string m_OutFile;
        TradeMatrix m_TradeMatrix;

    public:
        Solution(const string & input, const string &outfile) :
            m_InFile(input),
            m_OutFile(outfile)
        {}

        /*
         * This function will read just on line from input file
         * and process that line to simulate stream of trades.
         * Then it will read next line, process it and so on.
         */
        void ProcessTradeData()
        {
            while (!m_InFile.IsEOF())
            {
                /*
                 *  Read just one line from input file to simulate stream of
                 *  data
                 */
                auto si = m_InFile.ReadNextLine();

                if (si.has_value())
                {
                    /*
                     * Input line is valid.
                     * Add/Update stat for this line
                     */
                    m_TradeMatrix.UpdateSymbolMatrix(si.value(), m_InFile.GetDelimeter());
                }
#if 0
                else
                {
                    // Bad input error msg is already logged
                }
#endif
            }

            /*
             * Done with reading and processing input file. Now write stat to out file
             */
            m_TradeMatrix.WriteMatrixToOutFile(m_OutFile.c_str());
        }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr<<"ERROR: Invalid number of command line arguments"<<endl;
        cerr<<"Usage: "<< argv[0] << " <Input_file_name> <output_file_name>"<<endl;
        exit(1);
    }

    Solution sol(argv[1], argv[2]);

    sol.ProcessTradeData();

    return 0;
}
