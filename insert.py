from openpyxl import load_workbook

from sys import argv

sheet_name = argv[1]

workbook_name = argv[2]

row_data = argv[3].split(',')



workbook = load_workbook(filename=f"{workbook_name}.xlsx")


sheet = workbook[sheet_name]

sheet.append(row_data);

workbook.save(filename=f"{workbook_name}.xlsx")
