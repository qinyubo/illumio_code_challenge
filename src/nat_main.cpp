#include "nattable.h" // function header file

using namespace std; 


int main()
{

	/* Construct and init NAT table */
	NatTable nat;
	nat.InitNatTable("./input/NAT.txt"); 

	/* Search FLOW and write to OUTPUT.txt */
	fstream flow_file, res_out;
	string flow_line, translated_ip;

	//Create OUTPUT file
	res_out.open("./OUTPUT.txt", ios::out);

	if(!res_out.is_open())
	{
		HandleError(5);
		exit(-1);
	}

	//Open and translate FLOW into OUTPUT
	flow_file.open("./input/FLOW.txt", ios::in);

	if(flow_file.is_open())
	{
		//Iterate FLOW 
		while(getline(flow_file, flow_line))
		{
			//Deal with blank line
			while (flow_line.length() == 0)
				getline(flow_file, flow_line);

			//Get translated <ip:port> pair
			translated_ip = nat.SearchTable(flow_line);

			//Write to OUPUT file
			if(translated_ip == "NOTFOUND")
			{
				res_out <<"No nat match for "<<flow_line<<endl;

			}
			else
			{
				res_out <<flow_line<<" -> "<<translated_ip<<endl;
			}

		}
	}
	else //Fail to open FLOW
	{	
		HandleError(6);
		exit(-1);
	}

	flow_file.close();
	res_out.close();

	return 0;
}