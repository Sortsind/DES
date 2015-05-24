#include <iostream>
#include <sstream>
#include <fstream>
#include "des.h"

using namespace std;

int value_file(ifstream &inputFile);
void entry_file(ifstream &inputFile, bool** data_in, int amount);
void enter_key(ifstream &inputFile, bool* initial_key);
void clear(bool** data_in, bool** encrypted_data_R, bool** encrypted_data_L, bool** key, int amount);

void first_iteration_key(bool* initial_key);
void second_iteration_key(bool* initial_key, bool** key);
void third_iteration_key(bool** key);

void pre_first_iteration(bool** data_in, int amount);
void formation_R0_L0(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);
void first_iteration(bool* encrypted_data_R, bool* encrypted_data_L1);
void second_iteration(bool* encrypted_data_R, bool* key);
void third_iteration(bool* encrypted_data_R);
void fourth_iteration(bool* encrypted_data_R);
void fifth_iteration(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_R1);

void output_file(bool* data_in, ifstream &inputFile);
void composition(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);
void final_iteration(bool* data_in);
void fifth_iteration_reverse(bool* encrypted_data_R, bool* encrypted_data_L, bool* encrypted_data_L1);
void formation_R15_L15(bool* encrypted_data_R, bool* encrypted_data_L, bool* data_in);

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		cout << "Incorrect string! Enter arguments as: 1/2 - code/decode input output key input! \n";
		system("pause");
		return 0;
	}
	bool initial_key[64];
	bool** key = new bool*[16];
	for (int i = 0; i < 16; i++)
		key[i] = new bool[56];
	int amount;
	ifstream f;
	f.open(argv[2], ios::in);
	if ((f.fail()))
	{
		cout << "Error: openning file.\n";
		return 0;
	}
	int amount_ = value_file(f);

	if ((amount_ % 8) == 0)
		amount = amount_ / 8;
	else
		amount = (amount_ / 8) + 1;

	bool** encrypted_data_L = new bool*[16];
	for (int i = 0; i < 16; i++)
		encrypted_data_L[i] = new bool[48];

	bool** encrypted_data_R = new bool*[16];
	for (int i = 0; i < 16; i++)
		encrypted_data_R[i] = new bool[48];

	bool** data_in = new bool*[amount];
	for (int i = 0; i < amount; i++)
		data_in[i] = new bool[64];

		switch (*argv[1])
		{
		case '1':
		{
				  ofstream o;
				  o.open(argv[3], ios::out);
				  ifstream k;
				  k.open(argv[4], ios::in);
				  if ((o.fail()) || (k.fail()))
				  {
					  cout << "Error: openning file.\n";
					  return 0;
				  }
				  cout << "Input file: ";
				  entry_file(f, data_in, amount);
				  enter_key(k, initial_key);

				  first_iteration_key(initial_key);
				  second_iteration_key(initial_key, key);
				  third_iteration_key(key);

				  pre_first_iteration(data_in, amount);
				  cout << "Output file: ";
				  for (int i = 0; i < amount; i++)
				  {
					  formation_R0_L0(encrypted_data_R[0], encrypted_data_L[0], data_in[i]);
					  for (int j = 0; j < 15; j++)
					  {
						  first_iteration(encrypted_data_R[j], encrypted_data_L[j + 1]);
						  second_iteration(encrypted_data_R[j], key[j + 1]);
						  third_iteration(encrypted_data_R[j]);
						  fourth_iteration(encrypted_data_R[j]);
						  fifth_iteration(encrypted_data_R[j], encrypted_data_L[j], encrypted_data_R[j + 1]);
					  }
					  composition(encrypted_data_R[15], encrypted_data_L[15], data_in[i]);
					  final_iteration(data_in[i]);
					  output_file(data_in[i], o);
				  }
				  cout << endl;
				  clear(data_in, encrypted_data_R, encrypted_data_L, key, amount);
				  return 0;
		}
		case '2':
		{
				  ifstream oo;
				  oo.open(argv[3], ios::in);
				  ifstream k;
				  k.open(argv[4], ios::in);
				  ofstream ff;
				  ff.open(argv[5], ios::out);
				  if ((oo.fail()) || (k.fail()) || (ff.fail()))
				  {
					  cout << "Error: openning file.\n";
					  return 0;
				  }
				  cout << "Output code file: ";
				  entry_file(oo, data_in, amount);
				  enter_key(k, initial_key);

				  first_iteration_key(initial_key);
				  second_iteration_key(initial_key, key);
				  third_iteration_key(key);

				  pre_first_iteration(data_in, amount);
				  cout << "Input decode file: ";
				  for (int i = 0; i < amount; i++)
				  {
					  formation_R15_L15(encrypted_data_R[15], encrypted_data_L[15], data_in[i]);
					  for (int j = 15; j > 0; j--)
					  {
						  first_iteration(encrypted_data_L[j], encrypted_data_R[j - 1]);
						  second_iteration(encrypted_data_L[j], key[j]);
						  third_iteration(encrypted_data_L[j]);
						  fourth_iteration(encrypted_data_L[j]);
						  fifth_iteration_reverse(encrypted_data_R[j], encrypted_data_L[j], encrypted_data_L[j - 1]);
					  }
					  composition(encrypted_data_R[0], encrypted_data_L[0], data_in[i]);
					  final_iteration(data_in[i]);
					  output_file(data_in[i], ff);
				  }
				  cout << endl;
				  clear(data_in, encrypted_data_R, encrypted_data_L, key, amount);
				  return 0;
		}
		default:
		{
				  cout << "Wrong menu option. Please again\n";
				  return 0;
		}
		}
}
