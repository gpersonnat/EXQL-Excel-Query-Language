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


## Queries

This beta version of EXQL currently supports Read, Insert, and Update Queries. 

## Read 

Here are examples of Read/Select queries 

```

data> choose sheet SalesOrders select all
OrderDate|Region|Rep|Item|Units|Unit Cost�|Total
-----------------
2021-01-06|East|Jones|Pencil|95|1.99|189.05
2021-01-23|Central|Kivell|Binder|50|19.99|999.4999999999999
2021-02-09|Central|Jardine|Pencil|36|4.99|179.64000000000001
2021-02-26|Central|Gill|Pen|27|19.99|539.7299999999999
2021-03-15|West|Sorvino|Pencil|56|2.99|167.44
2021-04-01|East|Jones|Binder|60|4.99|299.40000000000003
2021-04-18|Central|Andrews|Pencil|75|1.99|149.25
2021-05-05|Central|Jardine|Pencil|90|4.99|449.1
2021-05-22|West|Thompson|Pencil|32|1.99|63.68
...

data> choose sheet SalesOrders select where Region = "Central"
OrderDate|Region|Rep|Item|Units|Unit Cost|Total
-----------------
2021-01-23|Central|Kivell|Binder|50|19.99|999.4999999999999
2021-02-09|Central|Jardine|Pencil|36|4.99|179.64000000000001
2021-02-26|Central|Gill|Pen|27|19.99|539.7299999999999
2021-04-18|Central|Andrews|Pencil|75|1.99|149.25
2021-05-05|Central|Jardine|Pencil|90|4.99|449.1
2021-06-25|Central|Morgan|Pencil|90|4.99|449.1
2021-09-01|Central|Smith|Desk|2|125.0|250.0
2021-10-05|Central|Morgan|Binder|28|8.99|251.72
2021-11-25|Central|Kivell|Pen Set|96|4.99|479.04
2021-12-12|Central|Smith|Pencil|67|1.29|86.43
2022-01-15|Central|Gill|Binder|46|8.99|413.54
2022-02-01|Central|Smith|Binder|87|15.0|1305.0
2022-03-24|Central|Jardine|Pen Set|50|4.99|249.5
2022-04-10|Central|Andrews|Pencil|66|1.99|131.34
2022-05-14|Central|Gill|Pencil|53|1.29|68.37
2022-05-31|Central|Gill|Binder|80|8.99|719.2
2022-06-17|Central|Kivell|Desk|5|125.0|625.0
2022-07-21|Central|Morgan|Pen Set|55|12.49|686.95
2022-08-07|Central|Kivell|Pen Set|42|23.95|1005.9
2022-09-10|Central|Gill|Pencil|7|1.29|9.030000000000001
2022-10-31|Central|Andrews|Pencil|14|1.29|18.060000000000002
2022-11-17|Central|Jardine|Binder|11|4.99|54.89
2022-12-04|Central|Jardine|Binder|94|19.99|1879.06
2022-12-21|Central|Andrews|Binder|28|4.99|139.72

data> choose sheet SalesOrders select where (Region = "East") and Rep = "Jones"
OrderDate|Region|Rep|Item|Units|Unit Cost|Total
-----------------
2021-01-06|East|Jones|Pencil|95|1.99|189.05
2021-04-01|East|Jones|Binder|60|4.99|299.40000000000003
2021-06-08|East|Jones|Binder|60|8.99|539.4
2021-08-15|East|Jones|Pencil|35|4.99|174.65
2021-09-18|East|Jones|Pen Set|16|15.99|255.84
2021-10-22|East|Jones|Pen|64|8.99|575.36
2022-02-18|East|Jones|Binder|4|4.99|19.96
2022-07-04|East|Jones|Pen Set|62|4.99|309.38

```

The Boolean Operators (and, or) are both supported. Equality and Comparison operators (=, >, <,  >=, <=) are also supported as well. More equality comparison operators will be added 
such as substring operators. 


## Update

You can perform update queries to update columns in rows satisfying a certain predicate. Currently, you are limited to updating one column per query.

```

data> update sheet SalesOrders where (Region = "East") and Rep = "Jones" set Units to 50
data> choose sheet SalesOrders select where (Region = "East") and Rep = "Jones"
OrderDate|Region|Rep|Item|Units|Unit Cost|Total
-----------------
2021-01-06|East|Jones|Pencil|50|1.99|189.05
2021-04-01|East|Jones|Binder|50|4.99|299.40000000000003
2021-06-08|East|Jones|Binder|50|8.99|539.4
2021-08-15|East|Jones|Pencil|50|4.99|174.65
2021-09-18|East|Jones|Pen Set|50|15.99|255.84
2021-10-22|East|Jones|Pen|50|8.99|575.36
2022-02-18|East|Jones|Binder|50|4.99|19.96
2022-07-04|East|Jones|Pen Set|50|4.99|309.38

```


## Insert 

You can perform insert queries by defining a row of key value pairs: {column: value}


```

data> insert {OrderDate: "2021-01-06", Region: "West", Rep: "James", Item: "Pen", Units: 30, UnitCost: "5.32", Total: "186.05"} into sheet SalesOrders

```

Limitation: CSV file needs to be cleaned by Python, causing columns to be inserted out of order, but this will be fixed in the next version.



## Next Steps

- Add better error messaging 
- Handle edge cases (null values from excel files and uncleaned data)
- Enable user to store select queries in variables 
- Add DELETE operation 
- Improve runtime performance of update queries 