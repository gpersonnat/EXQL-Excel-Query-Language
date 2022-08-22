# EXQL-Excel-Query-Language

Excel Querying Language In C

This is an in-progress excel querying language built in C.


## Running Project 

The following command will start EXQL REPL.


```
./exql workbook.xlsx

workbook> 

```

### Example

```
% ./exql test.xlsx

test> 

```

When you start the REPL, all the sheets in the excel file will be converted to a CSV file, and stored in a folder called {workbook_name}.

In this example, all sheets (converted to CSV files) in test.xlsx are stored in a folder called test.