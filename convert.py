import pandas as pd
import sys


excel_file = sys.argv[1]

excel_name = sys.argv[2]

all_sheets = pd.read_excel(excel_file, sheet_name=None)

sheets = all_sheets.keys()

for sheet_name in sheets:
    sheet = pd.read_excel(excel_file, sheet_name=sheet_name)
    sheet.to_csv(f"{excel_name}/{sheet_name}.csv", index=False)
  
# Write the dataframe object
# into csv file

    
# read csv file and convert 
# into a dataframe object
