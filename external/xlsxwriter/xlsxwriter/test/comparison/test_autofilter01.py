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

        filename = 'autofilter01.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename
        self.txt_filename = test_dir + 'xlsx_files/' + 'autofilter_data.txt'

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """
        Test the creation of a simple XlsxWriter file with an autofilter.
        This test corresponds to the following examples/autofilter.py example:
        Example 1. Autofilter without conditions.
        """
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)
        worksheet = workbook.add_worksheet()

        # Set the autofilter.
        worksheet.autofilter('A1:D51')

        # Open a text file with autofilter example data.
        textfile = open(self.txt_filename)

        # Start writing data from the first worksheet row.
        row = 0

        # Read the text file and write it to the worksheet.
        for line in textfile:

            # Split the input data based on whitespace.
            data = line.strip("\n").split()

            # Convert the number data from the text file.
            for i, item in enumerate(data):
                try:
                    data[i] = float(item)
                except ValueError:
                    pass

            # Write out the row data.
            worksheet.write_row(row, 0, data)

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
