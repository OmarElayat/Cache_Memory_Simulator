#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include<algorithm>
#include <cstdlib>
#include<sstream>
#include<format>
#include<set>
#include <bitset>
#include<tuple>

using namespace std;

map<int, pair <int, bool> > cache_map;



class Memory_Simulator {
public:
	int Cache_line_Size_Ins;
	int Cache_length_Ins;
	int Cache_cycles_Ins;

	int Cache_line_Size_D;
	int Cache_length_D;
	int Cache_cycles_D;

public:
	Memory_Simulator(int Cache_Ins_S, int Cache_Ins_l_S, int Cache_Ins_c, int Cache_D_S, int Cache_D_l_S, int Cache_D_c) {
		// Instruction Cache
		Cache_line_Size_Ins = Cache_Ins_l_S; //in words
		Cache_length_Ins = Cache_Ins_S / (Cache_Ins_l_S * 4);
		cache_Instruction_map = new pair <int, bool>[Cache_length_Ins];
		Cache_cycles_Ins = Cache_Ins_c;

		// Data Cache
		Cache_line_Size_D = Cache_D_l_S; //in words
		Cache_length_D = Cache_D_S / (Cache_D_l_S * 4);
		cache_Data_map = new pair <int, bool>[Cache_length_D];
		Cache_cycles_D = Cache_D_c;
	}

	void read_seq(string filename) {

		string linemem;
		ifstream Sequence(filename);
		ofstream outfile;
		ofstream outfile_data;
		if (Sequence.is_open())
		{
			open_write();
			while (getline(Sequence, linemem)) {
				string address2 = linemem.substr(0, linemem.find(' '));
				char Ins = linemem[linemem.find(' ')+1];
				uint32_t address = stoi(address2);

				if (Ins == 'I' || Ins == 'i') 
				{
					Execute(address, true);
					write_filler(1);
				}
				else if (Ins == 'D' || Ins == 'd')
				{
					Execute(address, false);
					write_filler(0);
				}
				else {
					cout << "Wrong cache type: write D or I" << endl; 
				}
				             
				//	MemoryIn_map.insert({ address2,value2 });
				//	cout << address << endl<< value << endl;
			}
			write(true);
			write(false);
			cout << " sequence file" << endl;
			Sequence.close();
		}
		else { cout << "cannot open sequence file" << endl; }
	}


private:


	pair <int, bool>* cache_Instruction_map;
	pair <int, bool>* cache_Data_map;

	int  Number_Of_Access_Instruction = 0;
	float  Miss_Instruction = 0;
	float  hit_Instruction = 0;
	int  Number_Of_Access_Data = 0;
	float  Miss_Data = 0;
	float  hit_Data = 0;
	int Index = 0;
	int Tag = 0; 
	int word_offset = 0;
	int byte_offset = 0;
	ofstream outfile;
	ofstream outfile_data;

	int Execute(uint32_t Memory_address , bool Is_instruction ) {

		byte_offset = Memory_address & 0b0011;
		Memory_address = Memory_address >> 2;
		
		if (Is_instruction) {
			word_offset = Memory_address % (Cache_line_Size_Ins);
			Index = (Memory_address / Cache_line_Size_Ins) % (Cache_length_Ins);
			Tag = Memory_address / (Cache_line_Size_Ins * Cache_length_Ins);
			Number_Of_Access_Instruction++;
			pair <int, bool> mx = cache_Instruction_map[Index];
			if (mx.first == Tag && mx.second == true)
			{
				//same Tag & Valid Bit
				hit_Instruction++;
				cout << "hit" << endl;
			}
			else {
				//Different Tag or  unValid Bit
				Miss_Instruction ++;
				cache_Instruction_map[Index] = make_pair(Tag, true);
				cout << "miss" << endl;
			}
		}
		else{
			word_offset = Memory_address % (Cache_line_Size_D);
			Index = (Memory_address / Cache_line_Size_D) % (Cache_length_D);
			Tag = Memory_address / (Cache_line_Size_D * Cache_length_D);
			Number_Of_Access_Data++;
			pair <int, bool> mx = cache_Data_map[Index];
			if (mx.first == Tag && mx.second == true)
			{
				//same Tag & Valid Bit
				hit_Data++;
				cout << "hit" << endl;
			}
			else {
				//Different Tag or  unValid Bit
				Miss_Data++;
				cache_Data_map[Index] = make_pair(Tag, true);
				cout << "miss" << endl;
			}
		}
		
		cout << " index: " << Index << endl << "Tag:" << Tag << "byte off:"<<byte_offset<<"word off"<<word_offset<<endl;
		return Index;
	}


	void open_write() {
		outfile_data.open("Output_data.csv", ios::app);
		outfile.open("Output_inst.csv", ios::app);
		if (outfile.is_open() && outfile_data.is_open()) {
			
			outfile << "Index,Tag,Word offset,Byte offset,valid,Number of access,Hit ratio, Miss ratio, AMAT,\n";
			outfile_data << "Index,Tag,Word offset,Byte offset,valid,Number of access,Hit ratio, Miss ratio, AMAT,\n";
			outfile_data.close();
			outfile.close();
		}
		else {
			cout << "output file can't be oppened" << endl;
		}
	}

void write(bool ft) {

	float hit_ratio_Ins = hit_Instruction / Number_Of_Access_Instruction;
	float Miss_ratio_Ins = Miss_Instruction / Number_Of_Access_Instruction;
	float hit_ratio_D = hit_Data / Number_Of_Access_Data;
	float Miss_ratio_D = Miss_Data / Number_Of_Access_Data;

	if (ft) {
		outfile.open("Output_cache_overall_inst.csv", ios::app);
		outfile << "Index,Tag,valid,\n";
		if (outfile.is_open()) {

			for (int i = 0; i < Cache_length_Ins; i++) {
				if (cache_Instruction_map[i].second) {
					outfile << i << "," << cache_Instruction_map[i].first << "," << cache_Instruction_map[i].second << "," << endl;
				}
				else {
					outfile << i << "," << "-" << "," << "0" << endl;
				}
			}
			outfile.close();
			cout << "cloooooooooooosed" << endl;
		}
		else
			cout << "cann't open the overall output file";
	}
	else{
		outfile_data.open("Output_cache_overall_data.csv", ios::app);
		outfile_data << "Index,Tag,valid,\n";
		if (outfile_data.is_open()) {
			for (int i = 0; i < Cache_length_D; i++) {
					if (cache_Data_map[Index].second) {
						outfile_data << i << "," << cache_Data_map[i].first << "," << cache_Data_map[i].second << "," << endl;
					}
					else {
						outfile_data << i << "," << "-" << "," << "0" << endl;
					}
				}
			
			outfile_data.close();
			cout << "cloooooooooooosed" << endl;
		}
		else
			cout << "cann't open the overall output file";
	}
		}

void write_filler(bool filetype) {

	float hit_ratio_Ins = hit_Instruction / Number_Of_Access_Instruction;
	float Miss_ratio_Ins = Miss_Instruction / Number_Of_Access_Instruction;
	float hit_ratio_D = hit_Data / Number_Of_Access_Data;
	float Miss_ratio_D = Miss_Data / Number_Of_Access_Data;

	if (filetype) {
		outfile.open("Output_inst.csv", ios::app);
		if (outfile.is_open()) {
			outfile << Index << "," << Tag << "," << word_offset << "," << byte_offset << "," << cache_Instruction_map[Index].second << "," << Number_Of_Access_Instruction << ",";
			if (Number_Of_Access_Instruction) {
				outfile << hit_ratio_Ins << ",";
				outfile << Miss_ratio_Ins << ",";
				outfile << Cache_cycles_Ins + Miss_ratio_Ins * 100 << "," << endl;
			}
			else {
				outfile << "-" << "," << "-" << "," << "-" << "," << endl;
			}
			outfile.close();
			cout << "cloooooooooooosed" << endl;
		}
		else
			cout << "cann't open the output file";
	}
	else {
		outfile_data.open("Output_data.csv", ios::app);
		if (outfile_data.is_open()) {
			outfile_data << Index << "," << Tag << "," << word_offset << "," << byte_offset << "," << cache_Data_map[Index].second << "," << Number_Of_Access_Data << ",";
			if (Number_Of_Access_Data) {
				outfile_data << hit_ratio_D << ",";
				outfile_data << Miss_ratio_D << ",";
				outfile_data << Cache_cycles_Ins + Miss_ratio_D * 100 << "," << endl;
			}
			else {
				outfile_data << "-" << "," << "-" << "," << "-" << "," << endl;
			}
			outfile_data.close();
			cout << "cloooooooooooosed" << endl;
		}
		else
			cout << "cann't open the output file";
	}

}

};



int main()
{
	int Ins_cache_size;
	int Ins_Cache_line_Size;
	int Ins_Cache_cycles;

	int D_Cache_line_Size;
	int D_Cache_size;
	int D_Cache_cycles;

	cout << "Enter the Instruction Cache Size" << endl;
	cin >> Ins_cache_size;
	cout << "Enter the Instruction Cache line size" << endl;
	cin >> Ins_Cache_line_Size;
	cout << "Enter the Instruction Cache access cycles" << endl;
	cin >> Ins_Cache_cycles;

	cout << "Enter the Data Cache Size" << endl;
	cin >> D_Cache_size;
	cout << "Enter the Data Cache line size" << endl;
	cin >> D_Cache_line_Size;
	cout << "Enter the Data Cache access cycles" << endl;
	cin >> D_Cache_cycles;


	Memory_Simulator ass(Ins_cache_size, Ins_Cache_line_Size, Ins_Cache_cycles, D_Cache_size, D_Cache_line_Size, D_Cache_cycles);
	ass.read_seq("file1.txt");



	return 0;
}
