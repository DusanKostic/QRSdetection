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

        filename = 'set_column08.xlsx'

        test_dir = 'xlsxwriter/test/comparison/'
        self.image_dir = test_dir + 'images/'
        self.got_filename = test_dir + '_test_' + filename
        self.exp_filename = test_dir + 'xlsx_files/' + filename

        self.ignore_files = []
        self.ignore_elements = {}

    def test_create_file(self):
        """Test the creation of a simple XlsxWriter file."""
        filename = self.got_filename

        ####################################################

        workbook = Workbook(filename)

        worksheet = workbook.add_worksheet()

        bold = workbook.add_format({'bold': 1})
        italic = workbook.add_format({'italic': 1})

        data = [
            [1, 2, 3, 4, 5],
            [2, 4, 6, 8, 10],
            [3, 6, 9, 12, 15],
        ]

        worksheet.write('A1', 'Foo', bold)
        worksheet.write('B1', 'Bar', italic)
        worksheet.write_column('A2', data[0])
        worksheet.write_column('B2', data[1])
        worksheet.write_column('C2', data[2])

        worksheet.set_row(12, None, None, {'hidden': True})
        worksheet.set_column('F:F', None, None, {'hidden': True})

        worksheet.insert_image('E12', self.image_dir + 'logo.png')

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
