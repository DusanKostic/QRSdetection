###############################################################################
#
# Worksheet - A class for writing Excel Worksheets.
#
# Copyright 2013, John McNamara, jmcnamara@cpan.org
#
import re
from warnings import warn

COL_NAMES = {}
range_parts = re.compile(r'(\$?)([A-Z]{1,3})(\$?)(\d+)')


def xl_rowcol_to_cell(row, col, row_abs=False, col_abs=False):
    """
    Convert a zero indexed row and column cell reference to a A1 style string.

    Args:
       row:     The cell row. Int.
       col:     The cell column. Int.
       row_abs: Optional flag to make the row absolute. Bool.
       col_abs: Optional flag to make the column absolute.  Bool.

    Returns:
        A1 style string.

    """
    row += 1  # Change to 1-index.
    row_abs = '$' if row_abs else ''
    col_abs = '$' if col_abs else ''

    col_str = xl_col_to_name(col, col_abs)

    return col_str + row_abs + str(row)


def xl_rowcol_to_cell_fast(row, col):
    """
    Optimised version of the xl_rowcol_to_cell function. Only used internally.

    Args:
       row: The cell row. Int.
       col: The cell column. Int.

    Returns:
        A1 style string.

    """
    if col in COL_NAMES:
        col_str = COL_NAMES[col]
    else:
        col_str = xl_col_to_name(col)
        COL_NAMES[col] = col_str

    return col_str + str(row + 1)


def xl_col_to_name(col_num, col_abs=False):
    """
    Convert a zero indexed column cell reference to a string.

    Args:
       col:     The cell column. Int.
       col_abs: Optional flag to make the column absolute.  Bool.

    Returns:
        Column style string.

    """
    col_num += 1  # Change to 1-index.
    col_str = ''
    col_abs = '$' if col_abs else ''

    while col_num:

        # Set remainder from 1 .. 26
        remainder = col_num % 26

        if remainder == 0:
            remainder = 26

        # Convert the remainder to a character.
        col_letter = chr(ord('A') + remainder - 1)

        # Accumulate the column letters, right to left.
        col_str = col_letter + col_str

        # Get the next order of magnitude.
        col_num = int((col_num - 1) / 26)

    return col_abs + col_str


def xl_cell_to_rowcol(cell_str):
    """
    TODO. Add Utility.py docs.

    """
    if not cell_str:
        return (0, 0)

    match = range_parts.match(cell_str)
    col_str = match.group(2)
    row_str = match.group(4)

    # Convert base26 column string to number.
    expn = 0
    col = 0
    for char in reversed(col_str):
        col += (ord(char) - ord('A') + 1) * (26 ** expn)
        expn += 1

    # Convert 1-index to zero-index
    row = int(row_str) - 1
    col -= 1

    return row, col


def xl_cell_to_rowcol_abs(cell_str):
    """
    TODO. Add Utility.py docs.

    """
    if not cell_str:
        return (0, 0, False, False)

    match = range_parts.match(cell_str)

    col_abs = match.group(1)
    col_str = match.group(2)
    row_abs = match.group(3)
    row_str = match.group(4)

    if col_abs:
        col_abs = True
    else:
        col_abs = False

    if row_abs:
        row_abs = True
    else:
        row_abs = False

    # Convert base26 column string to number.
    expn = 0
    col = 0
    for char in reversed(col_str):
        col += (ord(char) - ord('A') + 1) * (26 ** expn)
        expn += 1

    # Convert 1-index to zero-index
    row = int(row_str) - 1
    col -= 1

    return row, col, row_abs, col_abs


def xl_range(first_row, first_col, last_row, last_col):
    """
    TODO. Add Utility.py docs.

    """
    range1 = xl_rowcol_to_cell(first_row, first_col)
    range2 = xl_rowcol_to_cell(last_row, last_col)

    return range1 + ':' + range2


def xl_color(color):
    # Used in conjunction with the XlsxWriter *color() methods to convert
    # a colour name into an RGB formatted string. These colours are for
    # backward compatibility with older versions of Excel.
    named_colors = {
        'black': '#000000',
        'blue': '#0000FF',
        'brown': '#800000',
        'cyan': '#00FFFF',
        'gray': '#808080',
        'green': '#008000',
        'lime': '#00FF00',
        'magenta': '#FF00FF',
        'navy': '#000080',
        'orange': '#FF6600',
        'pink': '#FF00FF',
        'purple': '#800080',
        'red': '#FF0000',
        'silver': '#C0C0C0',
        'white': '#FFFFFF',
        'yellow': '#FFFF00',
    }

    if color in named_colors:
        color = named_colors[color]

    if not re.match('#[0-9a-fA-F]{6}', color):
        warn("Color '%s' isn't a valid Excel color" % color)

    # Convert the RGB color to the Excel ARGB format.
    return "FF" + color.lstrip('#').upper()


def get_sparkline_style(style_id):
    styles = [
        {'series':   {'theme': "4", 'tint': "-0.499984740745262"},
         'negative': {'theme': "5"},
         'markers':  {'theme': "4", 'tint': "-0.499984740745262"},
         'first':    {'theme': "4", 'tint': "0.39997558519241921"},
         'last':     {'theme': "4", 'tint': "0.39997558519241921"},
         'high':     {'theme': "4"},
         'low':      {'theme': "4"},
         },  # 0
        {'series':   {'theme': "4", 'tint': "-0.499984740745262"},
         'negative': {'theme': "5"},
         'markers':  {'theme': "4", 'tint': "-0.499984740745262"},
         'first':    {'theme': "4", 'tint': "0.39997558519241921"},
         'last':     {'theme': "4", 'tint': "0.39997558519241921"},
         'high':     {'theme': "4"},
         'low':      {'theme': "4"},
         },  # 1
        {'series':   {'theme': "5", 'tint': "-0.499984740745262"},
         'negative': {'theme': "6"},
         'markers':  {'theme': "5", 'tint': "-0.499984740745262"},
         'first':    {'theme': "5", 'tint': "0.39997558519241921"},
         'last':     {'theme': "5", 'tint': "0.39997558519241921"},
         'high':     {'theme': "5"},
         'low':      {'theme': "5"},
         },  # 2
        {'series':   {'theme': "6", 'tint': "-0.499984740745262"},
         'negative': {'theme': "7"},
         'markers':  {'theme': "6", 'tint': "-0.499984740745262"},
         'first':    {'theme': "6", 'tint': "0.39997558519241921"},
         'last':     {'theme': "6", 'tint': "0.39997558519241921"},
         'high':     {'theme': "6"},
         'low':      {'theme': "6"},
         },  # 3
        {'series':   {'theme': "7", 'tint': "-0.499984740745262"},
         'negative': {'theme': "8"},
         'markers':  {'theme': "7", 'tint': "-0.499984740745262"},
         'first':    {'theme': "7", 'tint': "0.39997558519241921"},
         'last':     {'theme': "7", 'tint': "0.39997558519241921"},
         'high':     {'theme': "7"},
         'low':      {'theme': "7"},
         },  # 4
        {'series':   {'theme': "8", 'tint': "-0.499984740745262"},
         'negative': {'theme': "9"},
         'markers':  {'theme': "8", 'tint': "-0.499984740745262"},
         'first':    {'theme': "8", 'tint': "0.39997558519241921"},
         'last':     {'theme': "8", 'tint': "0.39997558519241921"},
         'high':     {'theme': "8"},
         'low':      {'theme': "8"},
         },  # 5
        {'series':   {'theme': "9", 'tint': "-0.499984740745262"},
         'negative': {'theme': "4"},
         'markers':  {'theme': "9", 'tint': "-0.499984740745262"},
         'first':    {'theme': "9", 'tint': "0.39997558519241921"},
         'last':     {'theme': "9", 'tint': "0.39997558519241921"},
         'high':     {'theme': "9"},
         'low':      {'theme': "9"},
         },  # 6
        {'series':   {'theme': "4", 'tint': "-0.249977111117893"},
         'negative': {'theme': "5"},
         'markers':  {'theme': "5", 'tint': "-0.249977111117893"},
         'first':    {'theme': "5", 'tint': "-0.249977111117893"},
         'last':     {'theme': "5", 'tint': "-0.249977111117893"},
         'high':     {'theme': "5", 'tint': "-0.249977111117893"},
         'low':      {'theme': "5", 'tint': "-0.249977111117893"},
         },  # 7
        {'series':   {'theme': "5", 'tint': "-0.249977111117893"},
         'negative': {'theme': "6"},
         'markers':  {'theme': "6", 'tint': "-0.249977111117893"},
         'first':    {'theme': "6", 'tint': "-0.249977111117893"},
         'last':     {'theme': "6", 'tint': "-0.249977111117893"},
         'high':     {'theme': "6", 'tint': "-0.249977111117893"},
         'low':      {'theme': "6", 'tint': "-0.249977111117893"},
         },  # 8
        {'series':   {'theme': "6", 'tint': "-0.249977111117893"},
         'negative': {'theme': "7"},
         'markers':  {'theme': "7", 'tint': "-0.249977111117893"},
         'first':    {'theme': "7", 'tint': "-0.249977111117893"},
         'last':     {'theme': "7", 'tint': "-0.249977111117893"},
         'high':     {'theme': "7", 'tint': "-0.249977111117893"},
         'low':      {'theme': "7", 'tint': "-0.249977111117893"},
         },  # 9
        {'series':   {'theme': "7", 'tint': "-0.249977111117893"},
         'negative': {'theme': "8"},
         'markers':  {'theme': "8", 'tint': "-0.249977111117893"},
         'first':    {'theme': "8", 'tint': "-0.249977111117893"},
         'last':     {'theme': "8", 'tint': "-0.249977111117893"},
         'high':     {'theme': "8", 'tint': "-0.249977111117893"},
         'low':      {'theme': "8", 'tint': "-0.249977111117893"},
         },  # 10
        {'series':   {'theme': "8", 'tint': "-0.249977111117893"},
         'negative': {'theme': "9"},
         'markers':  {'theme': "9", 'tint': "-0.249977111117893"},
         'first':    {'theme': "9", 'tint': "-0.249977111117893"},
         'last':     {'theme': "9", 'tint': "-0.249977111117893"},
         'high':     {'theme': "9", 'tint': "-0.249977111117893"},
         'low':      {'theme': "9", 'tint': "-0.249977111117893"},
         },  # 11
        {'series':   {'theme': "9", 'tint': "-0.249977111117893"},
         'negative': {'theme': "4"},
         'markers':  {'theme': "4", 'tint': "-0.249977111117893"},
         'first':    {'theme': "4", 'tint': "-0.249977111117893"},
         'last':     {'theme': "4", 'tint': "-0.249977111117893"},
         'high':     {'theme': "4", 'tint': "-0.249977111117893"},
         'low':      {'theme': "4", 'tint': "-0.249977111117893"},
         },  # 12
        {'series':   {'theme': "4"},
         'negative': {'theme': "5"},
         'markers':  {'theme': "4", 'tint': "-0.249977111117893"},
         'first':    {'theme': "4", 'tint': "-0.249977111117893"},
         'last':     {'theme': "4", 'tint': "-0.249977111117893"},
         'high':     {'theme': "4", 'tint': "-0.249977111117893"},
         'low':      {'theme': "4", 'tint': "-0.249977111117893"},
         },  # 13
        {'series':   {'theme': "5"},
         'negative': {'theme': "6"},
         'markers':  {'theme': "5", 'tint': "-0.249977111117893"},
         'first':    {'theme': "5", 'tint': "-0.249977111117893"},
         'last':     {'theme': "5", 'tint': "-0.249977111117893"},
         'high':     {'theme': "5", 'tint': "-0.249977111117893"},
         'low':      {'theme': "5", 'tint': "-0.249977111117893"},
         },  # 14
        {'series':   {'theme': "6"},
         'negative': {'theme': "7"},
         'markers':  {'theme': "6", 'tint': "-0.249977111117893"},
         'first':    {'theme': "6", 'tint': "-0.249977111117893"},
         'last':     {'theme': "6", 'tint': "-0.249977111117893"},
         'high':     {'theme': "6", 'tint': "-0.249977111117893"},
         'low':      {'theme': "6", 'tint': "-0.249977111117893"},
         },  # 15
        {'series':   {'theme': "7"},
         'negative': {'theme': "8"},
         'markers':  {'theme': "7", 'tint': "-0.249977111117893"},
         'first':    {'theme': "7", 'tint': "-0.249977111117893"},
         'last':     {'theme': "7", 'tint': "-0.249977111117893"},
         'high':     {'theme': "7", 'tint': "-0.249977111117893"},
         'low':      {'theme': "7", 'tint': "-0.249977111117893"},
         },  # 16
        {'series':   {'theme': "8"},
         'negative': {'theme': "9"},
         'markers':  {'theme': "8", 'tint': "-0.249977111117893"},
         'first':    {'theme': "8", 'tint': "-0.249977111117893"},
         'last':     {'theme': "8", 'tint': "-0.249977111117893"},
         'high':     {'theme': "8", 'tint': "-0.249977111117893"},
         'low':      {'theme': "8", 'tint': "-0.249977111117893"},
         },  # 17
        {'series':   {'theme': "9"},
         'negative': {'theme': "4"},
         'markers':  {'theme': "9", 'tint': "-0.249977111117893"},
         'first':    {'theme': "9", 'tint': "-0.249977111117893"},
         'last':     {'theme': "9", 'tint': "-0.249977111117893"},
         'high':     {'theme': "9", 'tint': "-0.249977111117893"},
         'low':      {'theme': "9", 'tint': "-0.249977111117893"},
         },  # 18
        {'series':   {'theme': "4", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "4", 'tint': "0.79998168889431442"},
         'first':    {'theme': "4", 'tint': "-0.249977111117893"},
         'last':     {'theme': "4", 'tint': "-0.249977111117893"},
         'high':     {'theme': "4", 'tint': "-0.499984740745262"},
         'low':      {'theme': "4", 'tint': "-0.499984740745262"},
         },  # 19
        {'series':   {'theme': "5", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "5", 'tint': "0.79998168889431442"},
         'first':    {'theme': "5", 'tint': "-0.249977111117893"},
         'last':     {'theme': "5", 'tint': "-0.249977111117893"},
         'high':     {'theme': "5", 'tint': "-0.499984740745262"},
         'low':      {'theme': "5", 'tint': "-0.499984740745262"},
         },  # 20
        {'series':   {'theme': "6", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "6", 'tint': "0.79998168889431442"},
         'first':    {'theme': "6", 'tint': "-0.249977111117893"},
         'last':     {'theme': "6", 'tint': "-0.249977111117893"},
         'high':     {'theme': "6", 'tint': "-0.499984740745262"},
         'low':      {'theme': "6", 'tint': "-0.499984740745262"},
         },  # 21
        {'series':   {'theme': "7", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "7", 'tint': "0.79998168889431442"},
         'first':    {'theme': "7", 'tint': "-0.249977111117893"},
         'last':     {'theme': "7", 'tint': "-0.249977111117893"},
         'high':     {'theme': "7", 'tint': "-0.499984740745262"},
         'low':      {'theme': "7", 'tint': "-0.499984740745262"},
         },  # 22
        {'series':   {'theme': "8", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "8", 'tint': "0.79998168889431442"},
         'first':    {'theme': "8", 'tint': "-0.249977111117893"},
         'last':     {'theme': "8", 'tint': "-0.249977111117893"},
         'high':     {'theme': "8", 'tint': "-0.499984740745262"},
         'low':      {'theme': "8", 'tint': "-0.499984740745262"},
         },  # 23
        {'series':   {'theme': "9", 'tint': "0.39997558519241921"},
         'negative': {'theme': "0", 'tint': "-0.499984740745262"},
         'markers':  {'theme': "9", 'tint': "0.79998168889431442"},
         'first':    {'theme': "9", 'tint': "-0.249977111117893"},
         'last':     {'theme': "9", 'tint': "-0.249977111117893"},
         'high':     {'theme': "9", 'tint': "-0.499984740745262"},
         'low':      {'theme': "9", 'tint': "-0.499984740745262"},
         },  # 24
        {'series':   {'theme': "1", 'tint': "0.499984740745262"},
         'negative': {'theme': "1", 'tint': "0.249977111117893"},
         'markers':  {'theme': "1", 'tint': "0.249977111117893"},
         'first':    {'theme': "1", 'tint': "0.249977111117893"},
         'last':     {'theme': "1", 'tint': "0.249977111117893"},
         'high':     {'theme': "1", 'tint': "0.249977111117893"},
         'low':      {'theme': "1", 'tint': "0.249977111117893"},
         },  # 25
        {'series':   {'theme': "1", 'tint': "0.34998626667073579"},
         'negative': {'theme': "0", 'tint': "-0.249977111117893"},
         'markers':  {'theme': "0", 'tint': "-0.249977111117893"},
         'first':    {'theme': "0", 'tint': "-0.249977111117893"},
         'last':     {'theme': "0", 'tint': "-0.249977111117893"},
         'high':     {'theme': "0", 'tint': "-0.249977111117893"},
         'low':      {'theme': "0", 'tint': "-0.249977111117893"},
         },  # 26
        {'series':   {'rgb': "FF323232"},
         'negative': {'rgb': "FFD00000"},
         'markers':  {'rgb': "FFD00000"},
         'first':    {'rgb': "FFD00000"},
         'last':     {'rgb': "FFD00000"},
         'high':     {'rgb': "FFD00000"},
         'low':      {'rgb': "FFD00000"},
         },  # 27
        {'series':   {'rgb': "FF000000"},
         'negative': {'rgb': "FF0070C0"},
         'markers':  {'rgb': "FF0070C0"},
         'first':    {'rgb': "FF0070C0"},
         'last':     {'rgb': "FF0070C0"},
         'high':     {'rgb': "FF0070C0"},
         'low':      {'rgb': "FF0070C0"},
         },  # 28
        {'series':   {'rgb': "FF376092"},
         'negative': {'rgb': "FFD00000"},
         'markers':  {'rgb': "FFD00000"},
         'first':    {'rgb': "FFD00000"},
         'last':     {'rgb': "FFD00000"},
         'high':     {'rgb': "FFD00000"},
         'low':      {'rgb': "FFD00000"},
         },  # 29
        {'series':   {'rgb': "FF0070C0"},
         'negative': {'rgb': "FF000000"},
         'markers':  {'rgb': "FF000000"},
         'first':    {'rgb': "FF000000"},
         'last':     {'rgb': "FF000000"},
         'high':     {'rgb': "FF000000"},
         'low':      {'rgb': "FF000000"},
         },  # 30
        {'series':   {'rgb': "FF5F5F5F"},
         'negative': {'rgb': "FFFFB620"},
         'markers':  {'rgb': "FFD70077"},
         'first':    {'rgb': "FF5687C2"},
         'last':     {'rgb': "FF359CEB"},
         'high':     {'rgb': "FF56BE79"},
         'low':      {'rgb': "FFFF5055"},
         },  # 31
        {'series':   {'rgb': "FF5687C2"},
         'negative': {'rgb': "FFFFB620"},
         'markers':  {'rgb': "FFD70077"},
         'first':    {'rgb': "FF777777"},
         'last':     {'rgb': "FF359CEB"},
         'high':     {'rgb': "FF56BE79"},
         'low':      {'rgb': "FFFF5055"},
         },  # 32
        {'series':   {'rgb': "FFC6EFCE"},
         'negative': {'rgb': "FFFFC7CE"},
         'markers':  {'rgb': "FF8CADD6"},
         'first':    {'rgb': "FFFFDC47"},
         'last':     {'rgb': "FFFFEB9C"},
         'high':     {'rgb': "FF60D276"},
         'low':      {'rgb': "FFFF5367"},
         },  # 33
        {'series':   {'rgb': "FF00B050"},
         'negative': {'rgb': "FFFF0000"},
         'markers':  {'rgb': "FF0070C0"},
         'first':    {'rgb': "FFFFC000"},
         'last':     {'rgb': "FFFFC000"},
         'high':     {'rgb': "FF00B050"},
         'low':      {'rgb': "FFFF0000"},
         },  # 34
        {'series':   {'theme': "3"},
         'negative': {'theme': "9"},
         'markers':  {'theme': "8"},
         'first':    {'theme': "4"},
         'last':     {'theme': "5"},
         'high':     {'theme': "6"},
         'low':      {'theme': "7"},
         },  # 35
        {'series':   {'theme': "1"},
         'negative': {'theme': "9"},
         'markers':  {'theme': "8"},
         'first':    {'theme': "4"},
         'last':     {'theme': "5"},
         'high':     {'theme': "6"},
         'low':      {'theme': "7"},
         },  # 36
    ]

    return styles[style_id]
