#include "nattable.h" // function header file

using namespace std; 


void NatTable :: InitNatTable(string nat_path)
{
	fstream nat_file;
	nat_file.open(nat_path, ios::in);

	vector<string> tmp_pairs; //Cache the splited <ip:port> pairs
	string org_pair, trans_pair; //NAT original <ip:port> pair, translated <ip:port> pair
	string org_ip_part, org_port_part; //ip and port parts in original ip:port

	if (nat_file.is_open())
	{
		string nat_line;

		while(getline(nat_file, nat_line))
		{
			//Deal with blank line
			while (nat_line.length() == 0)
				getline(nat_file, nat_line);
			
			//Split the original pair and translated pair
			tmp_pairs = Split(nat_line, ",");		
			//Check error inputs
			if(tmp_pairs.size() != 2)
			{
				//Cannot split nat rule into original and translated ip, port pair
				HandleError(1);
			}
			org_pair = tmp_pairs[0];
			trans_pair = tmp_pairs[1];

			//Split ip and port from the original pair in order to deal with the "*" situation
			tmp_pairs = Split(org_pair, ":");
			if(tmp_pairs.size() != 2)
			{
				HandleError(1);
			}
			org_ip_part = tmp_pairs[0];
			org_port_part = tmp_pairs[1];


			//Build the NAT table under three conditions
			if (org_ip_part != "*" and org_port_part != "*")
			{
				if (nat_hmap.find(org_pair) != nat_hmap.end())
				{
					// The same <ip, port> pair has occured before
					// This results in multiple translation rules
					// Here we replace it with the lastest rule
					HandleError(2);
				}
				nat_hmap[org_pair] = trans_pair;
			
			}
			else if (org_ip_part != "*" and org_port_part == "*")
			{
				if (nat_hmap.find(org_ip_part) != nat_hmap.end())
				{
					HandleError(2);
				}
				nat_hmap[org_ip_part] = trans_pair;
			}
			else if (org_ip_part == "*" and org_port_part != "*")
			{
				if (nat_hmap.find(org_port_part) != nat_hmap.end())
				{
					HandleError(2);
				}

				nat_hmap[org_port_part] = trans_pair;
			}
			else
			{
				// Here is the case: org_ip_part == "*" and org_port_part == "*"
				// Invalide rule
				HandleError(1);
			}
		}
	}
	else
	{
		// Error: cannot open NAT file.
		// Exit program
		HandleError(3);
		exit(-1);
	}
	nat_file.close();
}


string NatTable :: SearchTable(string pair)
{
	vector<string> tmp_pairs2;
	string flow_ip, flow_port;
	string translated_ip;

	// First, directly search the nat_hmap
	if (nat_hmap.find(pair) != nat_hmap.end())
	{
		translated_ip = nat_hmap[pair];
		return translated_ip;
	}

	// If not found, search NAT rules with "*"

	// Split input
	tmp_pairs2 = Split(pair, ":");
	// Check input
	if(tmp_pairs2.size() != 2)
	{
		// Invalid FLOW input
		HandleError(4);
	}
	flow_ip = tmp_pairs2[0];
	flow_port = tmp_pairs2[1];

	if (nat_hmap.find(flow_ip) != nat_hmap.end())
	{
		translated_ip = nat_hmap[flow_ip];
	} 
	else if (nat_hmap.find(flow_port) != nat_hmap.end())
	{
		translated_ip = nat_hmap[flow_port];
	}
	else
	{
		translated_ip = "NOTFOUND";
	}

	return translated_ip;
}

/* To split input by delimiter */
vector<string> Split(const string &str, const string &delimiter)
{
	vector<string> res;
	if(str == "") return res;
	//To split the last part
	string strs = str + delimiter;
	size_t pos = strs.find(delimiter);

	while(pos != strs.npos)
	{
		string tmp = strs.substr(0, pos);
		res.push_back(tmp);
		//Remove the delimiter part and continue
		strs = strs.substr(pos+1, strs.size());
		pos = strs.find(delimiter);

	}

	return res;
}

/* Output the error and warning message */
void HandleError(int msg_id)
{
	switch(msg_id){

		case 1:
			cout <<"Warning: invalid NAT rule."<<endl;
			break;
		case 2:
			cout << "Warning: duplicated translation rules." <<endl;
			break;
		case 3:
			cout <<"Error: cannot open NAT file."<<endl;
			break;
		case 4:
			cout <<"Warning: invalid FLOW input."<<endl;
			break;
		case 5:
			cout <<"Error: cannot open OUTPUT file." <<endl;
			break;
		case 6:
			cout <<"Error: cannot open FLOW file." <<endl;
			break;
		default:
			break;
	}
}
