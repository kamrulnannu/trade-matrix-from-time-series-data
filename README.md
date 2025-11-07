PROBLEM DESCRIPTION:
  The task is to compute trade stat/matrix after market close from time series
  data which are comming from a exchange. The format of sample time series data:

  <TimeStamp>,<StockSymbol>,<Volume>,<Price>

  1. The program should be easily enhanced if more data fields are added at the
  end of the above sample time series data. 
  2. The prgramm will print following stat/matrix for each of stock symbol in
     ascending order by symbol:

     <StockSymbol>,<MaxTradeGap>,<TotalVolumeTraded>,<MaxTradePrice>,<VolWeightedAvgPrc>

     StockSymbol - Symbol of a stock eg. IBM
     MaxTradeGap - What is maimum trade gap in a given day for IBM
     TotalVolumeTraded - in a given day for stock IBM
     MaxTradePrice - What is max trade price in a gicen day for IBM.
     VolWeightedAvgPrc - TotalWeight/TotalVolume.
         TotalWeight= Volume1 * price1 + Volume2 * price2 + ...
         TotalVolume = Volume1 + Volume2 + ...

   The code should easily be enhanced if more stat are added or if we want
   fewer stat from above given stat.
 3. The code should not process invalid time series data
----------------------------------------------------------------------

1. I have built the project in cygwin (x86_64, windows 10 professional). Version
details:
$ g++ -v
Using built-in specs.
COLLECT_GCC=g++
...
Thread model: posix
gcc version 9.2.0 (GCC)
 
2. I have used some C++17 features. Command line to build:
  $ g++ -std=c++2a  InputFileReader.cpp TradeStat.cpp main.cpp -o trade_stat

3. Command used to run: command input_file output_file
   - 1st arument is the command
   - 2nd argument is the input file
   - 3rd argument is the output file

  (a) Using given input from Quantlab:
     $ trade_stat sample_input.csv sample_output.csv

  (b) Using my sample input file to test 
      (i) line validation logic in the input file, eg. blank symbol or 
          less than equal to 0 timestamp/volume/price
      (ii) whether number items in the line is 4 (A valid input line
           should have 4 items: timestamp, symbol, voulme, price)
      (iii) Whether current time stamp less than previous time stamp

    Example error sample while processing sample_input.csv:
        ERROR: Share price is <= 0!, Input Line =52900024703,aax,9,0, Line Num=4
        ERROR: Current time stamp is less than previous time stamp!, Input Line =52900027780,aac,21,638, Line Num=9
        ERROR: Invalid symbol!, Input Line =52900033455,,9,756, Line Num=11
        ERROR: Share value <= 0!, Input Line =52900033456,aap,0,756, Line Num=12
        ERROR: Share price is <= 0!, Input Line =52900033457,aaq,9,0, Line Num=13
        ERROR: Time stamp value <= 0!, Input Line =0,aar,9,759, Line Num=14

    $ trade_stat.exe sample_input.csv sample_output.csv
