.. _memory_perf:

Working with Memory and Performance
===================================

By default XlsxWriter holds all cell data in memory. This is to allow future
features when formatting is applied separately from the data.

The effect of this is that XlsxWriter can consume a lot of memory and it is
possible to run out of memory when creating large files.

Fortunately, this memory usage can be reduced almost completely by using the
:func:`Workbook` ``'constant_memory'`` property::

    workbook = xlsxwriter.Workbook(filename, {'constant_memory': True})

The optimisation works by flushing each row after a subsequent row is written.
In this way the largest amount of data held in memory for a worksheet is the
amount of data required to hold a single row of data.

Since each new row flushes the previous row, data must be written in sequential
row order when ``'constant_memory'`` mode is on::

    # With 'constant_memory' you must write data in row by column order.
    for row in range(0, row_max):
        for col in range(0, col_max):
            worksheet.write(row, col, some_data)

    # With 'constant_memory' this would only write the first column of data.
    for col in range(0, col_max):
        for row in range(0, row_max):
            worksheet.write(row, col, some_data)

Another optimisation that is used to reduce memory usage is that cell strings
aren't stored in an Excel structure call "shared strings" and instead are
written "in-line". This is a documented Excel feature that is supported by
most spreadsheet applications. One known exception is Apple Numbers for Mac
where the string data isn't displayed.

The trade-off when using ``'constant_memory'`` mode is that you won't be able
to take advantage of any new features that manipulate cell data after it is
written. Currently the only such feature is :ref:`Worksheet Tables <tables>`.

For larger files ``'constant_memory'`` mode also gives an increase in execution
speed, see below.


Performance Figures
-------------------

The performance figures below show execution time and memory usage for
worksheets of size ``N`` rows x 50 columns with a 50/50 mixture of strings and
numbers. The figures are taken from an arbitrary, mid-range, machine. Specific
figures will vary from machine to machine but the trends should be the same.

XlsxWriter in normal operation mode: the execution time and memory usage
increase more of less linearly with the number of rows:

+-------+---------+----------+----------------+
| Rows  | Columns | Time (s) | Memory (bytes) |
+=======+=========+==========+================+
| 200   | 50      | 0.65     | 2050552        |
+-------+---------+----------+----------------+
| 400   | 50      | 1.32     | 4478272        |
+-------+---------+----------+----------------+
| 800   | 50      | 2.64     | 8083072        |
+-------+---------+----------+----------------+
| 1600  | 50      | 5.31     | 17799424       |
+-------+---------+----------+----------------+
| 3200  | 50      | 10.74    | 32218624       |
+-------+---------+----------+----------------+
| 6400  | 50      | 21.63    | 64792576       |
+-------+---------+----------+----------------+
| 12800 | 50      | 43.49    | 128760832      |
+-------+---------+----------+----------------+

XlsxWriter in ``constant_memory`` mode: the execution time still increases
linearly with the number of rows but the memory usage remains small and
constant:

+-------+---------+----------+----------------+
| Rows  | Columns | Time (s) | Memory (bytes) |
+=======+=========+==========+================+
| 200   | 50      | 0.35     | 54248          |
+-------+---------+----------+----------------+
| 400   | 50      | 0.69     | 54248          |
+-------+---------+----------+----------------+
| 800   | 50      | 1.36     | 54248          |
+-------+---------+----------+----------------+
| 1600  | 50      | 2.74     | 54248          |
+-------+---------+----------+----------------+
| 3200  | 50      | 5.46     | 54248          |
+-------+---------+----------+----------------+
| 6400  | 50      | 10.99    | 54248          |
+-------+---------+----------+----------------+
| 12800 | 50      | 21.82    | 54248          |
+-------+---------+----------+----------------+

In the ``constant_memory`` mode the performance is also increased. There will
be further optimisation in both modes in later releases.

These figures were generated using programs in the ``dev/performance``
directory of the XlsxWriter source code.




