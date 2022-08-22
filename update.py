from sys import argv
import pandas as pd
from openpyxl import load_workbook
from helpers import excel_col_letter


column_name = argv[1]

new_value = argv[2]

value_type = argv[3]

row_number = int(argv[4])

sheet_name = argv[5]

column_number = int(argv[6])

workbook_name = argv[7]

df = pd.read_csv(f"{workbook_name}/{sheet_name}.csv")



df.loc[int(row_number), column_name] = new_value


df.to_csv(f"{workbook_name}/{sheet_name}.csv", index=False)

workbook = load_workbook(filename=f"{workbook_name}.xlsx")


sheet = workbook[sheet_name]

row = str(row_number + 2)

cell_name = f"{excel_col_letter(column_number)}{row}"


sheet[cell_name] = new_value

workbook.save(filename=f"{workbook_name}.xlsx")





