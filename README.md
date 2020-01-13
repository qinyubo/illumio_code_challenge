## File tree 
```
.
├── OUTPUT.txt
├── README.md
├── input
│   ├── FLOW.txt
│   └── NAT.txt
├── src
│   ├── nat_main.cpp
│   ├── nattable.cpp
│   └── nattable.h
└── test_script.sh
```

## Source file
``nattable.h`` Header file
``nattable.cpp`` Function implementation file
``nat_main.cpp`` NAT main file


## Compile and test
Just run ``test_script.sh`` from the root directory. I use relative paths in the code.

## Code design
### Structure
The application is splited into three files, header, implementation and main files. This design makes it easy to extand.


### NAT rule
In real scenario, the search speed is most important for NAT performance. Therefore, I use hash map to store the translation rules. It is time complexity is O(1) and space complexity is O(n).

Since we have "\*" in NAT rule, I store the hash key as the following rules.
if original <ip:port>, then key is <ip:port>.
if original <ip:"\*"> , then key is <ip>.
if original <"\*":port>, then key is <port>.

To deal with the duplicated rules case, such as <ip:"\*"> matches two translated <ip:pair>, the will output a warning message and replace hash map with the second (latest) rule.

### Functions
#### 1.Split
This is a helper function to split the input by delimiter. Since this code challenge only allow to use STL library

#### 2.HandleError
This function output the error or warning message. The code will check the input correctness.

#### 3.NatTable
This class encapture the NAT hash map data structure, and provide the hash map initiation and search functions

#### 4. NatTable.InitNatTable(NAT file path)
This function initiate the NAT table from the NAT file.

#### 5. NatTable.SearchTable(input)
This function search the FLOW input and result the translated <ip:port>. It first search the full original <ip:port>, if not found, it split the original <ip:port> input <ip> and <port> and search them again. 
If not found, return "NOTFOUND".

### Test case
My test example mainly checks the corner case and duplicated NAT rule cases. 
```
// Original input
10.0.1.1:8080,192.168.0.1:80
10.0.1.1:8084,192.168.0.2:8080
10.0.1.1:8086,192.168.0.4:80
10.0.1.1:8082,192.168.0.3:80
10.0.1.2:8080,1.1.1.1:1
*:21,1.2.3.4:12
10.11.12.13:*,40.100.20.7:3389

// Duplicated rule
// Same <ip:port>
10.0.1.3:8080,192.168.0.2:80
10.0.1.3:8080,192.168.0.3:80

// Same <*:port>
*:22,1.2.3.4:13
*:22,1.2.3.4:14
// Same <ip:*>
10.11.12.14:*,40.100.20.7:3390
10.11.12.14:*,40.100.20.7:3391

// Invalid input
*:*,2.2.2.2:2
```






