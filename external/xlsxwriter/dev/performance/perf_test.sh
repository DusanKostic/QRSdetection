#!/bin/bash

##############################################################################
#
# Simple test runner for measuring speed and memory usage of XlsxWriter 
# and the Excel::Writer::XLSX modules.
#
# Copyright 2013, John McNamara, jmcnamara@cpan.org

echo ""
echo "Python and XlsxWriter"
echo "Rows, Columns, Time, Memory"
sleep 1; python perf_pyx.py 200
sleep 1; python perf_pyx.py 400
sleep 1; python perf_pyx.py 800
sleep 1; python perf_pyx.py 1600
sleep 1; python perf_pyx.py 3200
sleep 1; python perf_pyx.py 6400
sleep 1; python perf_pyx.py 12800

echo ""
echo "Python and XlsxWriter in optimisation mode"
echo "Rows, Columns, Time, Memory"
sleep 1; python perf_pyx.py 200   1
sleep 1; python perf_pyx.py 400   1
sleep 1; python perf_pyx.py 800   1
sleep 1; python perf_pyx.py 1600  1
sleep 1; python perf_pyx.py 3200  1
sleep 1; python perf_pyx.py 6400  1
sleep 1; python perf_pyx.py 12800 1

echo ""
echo "Perl and Excel::Writer::XSLX"
echo "Rows, Columns, Time, Memory"
sleep 1; perl perf_ewx.pl 200
sleep 1; perl perf_ewx.pl 400
sleep 1; perl perf_ewx.pl 800
sleep 1; perl perf_ewx.pl 1600
sleep 1; perl perf_ewx.pl 3200
sleep 1; perl perf_ewx.pl 6400
sleep 1; perl perf_ewx.pl 12800

echo ""
echo "Perl Excel::Writer::XSLX in optimisation mode"
echo "Rows, Columns, Time, Memory"
sleep 1; perl perf_ewx.pl 200   1
sleep 1; perl perf_ewx.pl 400   1
sleep 1; perl perf_ewx.pl 800   1
sleep 1; perl perf_ewx.pl 1600  1
sleep 1; perl perf_ewx.pl 3200  1
sleep 1; perl perf_ewx.pl 6400  1
sleep 1; perl perf_ewx.pl 12800 1

echo ""
echo ""
