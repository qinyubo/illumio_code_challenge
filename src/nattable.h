//nat.h
#ifndef NATTABLE_H
#define NATTABLE_H
#include <iostream>  
#include <fstream> // read/write file
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_map>

using namespace std; 

/* NAT table class*/
class NatTable{
private:
	unordered_map<string, string> nat_hmap;
public:
	void InitNatTable(string nat_path);
	string SearchTable(string pair);
}; 

/* Heler functions */
vector<string> Split(const string &str, const string &delimiter);
void HandleError(int msg_id);

#endif