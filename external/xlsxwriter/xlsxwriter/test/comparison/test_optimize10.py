###############################################################################
#
# Tests for XlsxWriter.
#
# Copyright (c), 2013, John McNamara, jmcnamara@cpan.org
#

import unittest
import os
import codecs
from ...workbook import Workbook
from ..helperfunctions import _compare_xlsx_files


class TestCompareXLSXFiles(unittest.TestCase):
    """
    Test file created by XlsxWriter against a file created by Excel.

    """

    def setUp(self):
        self.maxDiff = None

        filename = 'optimize10.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename
        self.txt_filename = test_dir + 'xlsx_files/' + 'unicode_polish_utf8.txt'

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """Test example file converting Unicode text."""
        filename = self.got_filename

        ####################################################

        # Open the input file with the correct encoding.
        textfile = codecs.open(self.txt_filename, 'r', 'utf-8')

        # Create an new Excel file and convert the text data.
        workbook = Workbook(filename, {'constant_memory': True})
        worksheet = workbook.add_worksheet()

        # Widen the first column to make the text clearer.
        worksheet.set_column('A:A', 50)

        # Start from the first cell.
        row = 0
        col = 0

        # Read the text file and write it to the worksheet.
        for line in textfile:
            # Ignore the comments in the sample file.
            if line.startswith('#'):
                continue

            # Write any other lines to the worksheet.
            worksheet.write(row, col, line.rstrip("\n"))
            row += 1

        workbook.close()
        textfile.close()

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
