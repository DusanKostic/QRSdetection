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

        filename = 'hyperlink01.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """Test the creation of a simple XlsxWriter file with hyperlinks"""
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)

        # Turn off default URL format for testing.
        workbook.default_url_format = None

        worksheet = workbook.add_worksheet()

        worksheet.write_url('A1', 'http://www.perl.org/')

        workbook.close()

        ####################################################

        got, exp = _compare_xlsx_files(self.got_filename,
                                       self.exp_filename,
                                       self.ignore_files,
                                       self.ignore_elements)

        self.assertEqual(got, exp)

    def test_create_file_write(self):
        """Test the creation of a simple XlsxWriter file with hyperlinks with write()"""
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)

        # Turn off default URL format for testing.
        workbook.default_url_format = None

        worksheet = workbook.add_worksheet()

        worksheet.write('A1', 'http://www.perl.org/')

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
