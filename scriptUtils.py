#!/usr/bin/env python
import os, sys
import subprocess

def getXlsxwriter(lookInFolder):
  writerpath = ''

  if os.path.isdir(lookInFolder):
    writerpath = find('xlsxwriter', lookInFolder)
  else:
    os.mkdir(lookInFolder)

  sys.path.append(os.path.dirname(writerpath))

def find(name, path):
  for root, dirs, files in os.walk(path):
    if name in dirs:
      return os.path.join(root, name)

def makeFormats(workbook):
  """ Helper formats for the Excel sheet """
  formats = {}

  # Title format; 
  #global title
  formats['title'] = workbook.add_format({'bg_color': '#017DC7',
                             'font_color': '#FFFFFF',
                             'font_size' : 30,
                             'bold': True})

  #global header1
  formats['header1'] = workbook.add_format({'bg_color': '#002060',
                             'font_color': '#FFFFFF',
                             'font_size' : 18,
                             'bold': True})
  # Table formats
  table1 = workbook.add_format({'bg_color': '#daeef3',
                              'font_color': '#000000'})
  formats['tables'] = [table1]

  global passColor
  passColor = '#61D827'
  global failColor
  failColor = '#FF0000'

  return formats
