#!/usr/bin/env python

import time, os, sys
import argparse, subprocess
import collections
import scriptUtils
from math import sqrt

def main():
  args = parseCmdArguments()

  all_dict = parseLogFile(args.breakdown)

  results = []

  for name, times in all_dict.items():
    tmp_list = []
    min_time = min(times)
    max_time = max(times)
    avg_time = sum(times) / len(times)
    std_dev  = stdDeviation(times)
    tmp_list.append(name)
    tmp_list.append(avg_time)
    tmp_list.append(min_time)
    tmp_list.append(max_time)
    tmp_list.append(std_dev)
    results.append(tmp_list)

##################################################################################
#####                   Printing data to excel                           #####
##################################################################################
# Check if the XlsxWriter module exists
  path = os.path.abspath(os.path.dirname(__file__))
  scriptUtils.getXlsxwriter(os.path.join(os.getcwd(),
                            'external', 'xlsxwriter'))
  import xlsxwriter
  from xlsxwriter.utility import xl_rowcol_to_cell


# Create the workbook object and two worksheets
  xlsxFile = args.title + '.xlsx'
  print 'Making the Excel file ' + xlsxFile + ' ...'
  workbook = xlsxwriter.Workbook(xlsxFile)
  mainbook = workbook.add_worksheet('Summary')
  formats = scriptUtils.makeFormats(workbook)

  totalWidth = 5
  mainbook.set_column(0,0,25)
  mainbook.set_column(1,totalWidth,13)
  currentRow = 0

################################# Title ##########################################

  mainbook.set_row(currentRow, 40)
  title_text = "QRS detection performance report"
  mainbook.merge_range(currentRow, 0, currentRow, totalWidth,
    title_text, formats['title'])
  currentRow += 2

  mainbook.set_row(currentRow, 24)
  for i in range(totalWidth):
    mainbook.write_blank(currentRow, i, None, formats['header1'])

  mainbook.merge_range(currentRow, 0, currentRow, totalWidth,
    "Basic Summary".upper(), formats['header1'])
  currentRow += 2

  ### Basic Summary Table
  # Date and time of the test
  currentTime = time.localtime()
  mainbook.write(currentRow, 0, "Date and time:", formats['tables'][0])
  mainbook.merge_range(currentRow, 1, currentRow, totalWidth, time.strftime("%Y-%m-%d %H:%M", currentTime), formats['tables'][0])
  currentRow += 1

  mainbook.write(currentRow, 0, "Number of tests", formats['tables'][0])
  mainbook.merge_range(currentRow, 1, currentRow, totalWidth, args.testsnum, formats['tables'][0])
  currentRow += 1

  currentRow += 1

################################## Results ######################################

  mainbook.set_row(currentRow, 24)
  mainbook.merge_range(currentRow, 0, currentRow, totalWidth,
      "RESULTS", formats['header1'])
  currentRow += 2

  header_names = [{'header': 'Module'}, {'header': 'Average'}, {'header': 'Minimum'},
                  {'header': 'Maximum'}, {'header': 'StdDeviation'}]

  firstCol = 1
  mainbook.set_column(firstCol, firstCol + len(header_names) - 1, 20)
  mainbook.add_table(currentRow, firstCol, currentRow + len(results), firstCol + len(header_names) - 1,
                     {'data': results,
                      'first_column': True,
                      'columns': header_names
                     })

  workbook.close()
  print 'Excel report generated.'

def parseLogFile(filename):
  all_dict = collections.OrderedDict()

  try:
    print "Reading from " + filename
    with open(filename,'r') as logfile:
      for line in logfile:
        if not "consumed" in line:
          continue

        duration = int(line.split('consumed', 1)[1].split('ms', 1)[0].rstrip().lstrip())
        name = line.split(':', 1)[0].split(']', 1)[1].rstrip().lstrip()

        all_dict.setdefault(name, []).append(duration);

  except IOError:
    print "Could not open " + filename
  return all_dict

def parseCmdArguments():
  parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
  parser.add_argument("--breakdown","-b", help="Log file for the execution time")
  parser.add_argument("--title","-t", help="Filename and title of the report")
  parser.add_argument("--testsnum","-n", help="Number of snapshots taken")
  return parser.parse_args()

def stdDeviation(values):
  if len(values) == 1:
    return 0
  avg = sum(values) / float(len(values))
  std_dev = 0
  for a in values:
    std_dev += (a - avg) ** 2

  std_dev = sqrt(std_dev / float(len(values) - 1))
  return std_dev

if __name__ == "__main__":
  main()

