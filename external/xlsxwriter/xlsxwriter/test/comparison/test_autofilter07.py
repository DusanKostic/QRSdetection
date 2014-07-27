###############################################################################
#
# Tests for XlsxWriter.
#
# Copyright (c), 2013, John McNamara, jmcnamara@cpan.org
#

import unittest
import os
from ...workbook import Workbook
from ..helperfunctions import _compare_xlsx_files


class TestCompareXLSXFiles(unittest.TestCase):
    """
    Test file created by XlsxWriter against a file created by Excel.

    """

    def setUp(self):
        self.maxDiff = None

        filename = 'autofilter07.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename
        self.txt_filename = test_dir + 'xlsx_files/' + 'autofilter_data.txt'

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """
        Test the creation of a simple XlsxWriter file with an autofilter.
        Test autofilters where column filter ids are relative to autofilter
        range.
        """
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)
        worksheet = workbook.add_worksheet()

        # Set the autofilter.
        worksheet.autofilter('D3:G53')

        # Add filter criteria.
        worksheet.filter_column('D', 'region == East')

        # Open a text file with autofilter example data.
        textfile = open(self.txt_filename)

        # Read the headers from the first line of the input file.
        headers = textfile.readline().strip("\n").split()

        # Write out the headers.
        worksheet.write_row('D3', headers)

        # Start writing data after the headers.
        row = 3

        # Read the rest of the text file and write it to the worksheet.
        for line in textfile:

            # Split the input data based on whitespace.
            data = line.strip("\n").split()

            # Convert the number data from the text file.
            for i, item in enumerate(data):
                try:
                    data[i] = float(item)
                except ValueError:
                    pass

            # Get some of the field data.
            region = data[0]

            # Check for rows that match the filter.
            if region == 'East':
                # Row matches the filter, no further action required.
                pass
            else:
                # We need to hide rows that don't match the filter.
                worksheet.set_row(row, options={'hidden': True})

            # Write out the row data.
            worksheet.write_row(row, 3, data)

            # Move on to the next worksheet row.
            row += 1

        textfile.close()
        workbook.close()

        ####################################################

        got, exp = _compare_xlsx_files(self.got_filename,
                                       self.exp_filename,
                                       self.ignore_files,
                                       self.ignore_elements)

        self.assertEqual(got, exp)

    def tearDown(self):
        # Cleanup.
        if os.path.exists(self.got_filename):
            os.remove(self.got_filename)


if __name__ == '__main__':
    unittest.main()
