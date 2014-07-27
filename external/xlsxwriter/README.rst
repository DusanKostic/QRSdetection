
.. image:: https://raw.github.com/jmcnamara/XlsxWriter/master/dev/docs/source/_images/logo.png
   :target: https://github.com/jmcnamara/XlsxWriter

XlsxWriter
==========

.. image:: https://travis-ci.org/jmcnamara/XlsxWriter.png?branch=master
   :target: https://travis-ci.org/jmcnamara/XlsxWriter

.. image:: https://pypip.in/v/XlsxWriter/badge.png
   :target: https://crate.io/packages/XlsxWriter

.. image:: https://pypip.in/d/XlsxWriter/badge.png
   :target: https://crate.io/packages/XlsxWriter


**XlsxWriter** is a Python module for writing files in the Excel 2007+ XLSX
file format.

XlsxWriter can be used to write text, numbers, formulas and hyperlinks to
multiple worksheets and it supports features such as formatting and many more,
including:

* 100% compatible Excel XLSX files.
* Full formatting.
* Merged cells.
* Defined names.
* Charts.
* Autofilters.
* Data validation and drop down lists.
* Conditional formatting.
* Worksheet PNG/JPEG images.
* Rich multi-format strings.
* Cell comments.
* Memory optimisation mode for writing large files.

It supports Python 2.5, 2.6, 2.7, 3.1, 3.2, 3.3, Jython and PyPy and uses
standard libraries only.

Here is a simple example:

.. code-block:: python

   import xlsxwriter


   # Create an new Excel file and add a worksheet.
   workbook = xlsxwriter.Workbook('demo.xlsx')
   worksheet = workbook.add_worksheet()

   # Widen the first column to make the text clearer.
   worksheet.set_column('A:A', 20)

   # Add a bold format to use to highlight cells.
   bold = workbook.add_format({'bold': 1})

   # Write some simple text.
   worksheet.write('A1', 'Hello')

   # Text with formatting.
   worksheet.write('A2', 'World', bold)

   # Write some numbers, with row/column notation.
   worksheet.write(2, 0, 123)
   worksheet.write(3, 0, 123.456)

   # Insert an image.
   worksheet.insert_image('B5', 'logo.png')

   workbook.close()

.. image:: https://raw.github.com/jmcnamara/XlsxWriter/master/dev/docs/source/_images/demo.png

See the full documentation at: http://xlsxwriter.readthedocs.org

Release notes: https://xlsxwriter.readthedocs.org/en/latest/changes.html

