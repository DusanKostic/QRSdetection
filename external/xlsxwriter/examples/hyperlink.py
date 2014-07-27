###############################################################################
#
# Example of how to use the XlsxWriter module to write hyperlinks
#
# Copyright 2013, John McNamara, jmcnamara@cpan.org
#
import xlsxwriter

# Create a new workbook and add a worksheet
workbook = xlsxwriter.Workbook('hyperlink.xlsx')
worksheet = workbook.add_worksheet('Hyperlinks')

# Format the first column
worksheet.set_column('A:A', 30)

# Add the standard url link format.
url_format = workbook.add_format({
    'color':     'blue',
    'underline': 1
})

# Add a sample alternative link format.
red_format = workbook.add_format({
    'color':     'red',
    'bold':      1,
    'underline': 1,
    'size':      12,
})

# Add an alternate description string to the URL.
string = 'Python home'

# Add a "tool tip" to the URL.
tip = 'Get the latest Python news here.'

# Write some hyperlinks
worksheet.write('A1', 'http://www.python.org/')  # Implicit format.
worksheet.write('A3', 'http://www.python.org/', url_format, string)
worksheet.write('A5', 'http://www.python.org/', url_format, string, tip)
worksheet.write('A7', 'http://www.python.org/', red_format)
worksheet.write('A9', 'mailto:jmcnamaracpan.org', url_format, 'Mail me')

# Write a URL that isn't a hyperlink
worksheet.write_string('A11', 'http://www.python.org/')

workbook.close()
