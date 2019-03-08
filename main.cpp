#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool Read(vector<char> &Vec, const char* File)
{
	ifstream RawText(File, ifstream::in);

	if (RawText.is_open())
	{
		char temp = 0;
		int i = 0;
		RawText.seekg(0, RawText.end);
		i = RawText.tellg();
		Vec.reserve(i);
		RawText.seekg(0, RawText.beg);
		while (i)
		{
			i--;
			temp = RawText.get();
			if (temp > 64 && temp < 91) temp = (temp + 32 - 19) % 26 + 97;
			if (temp < 97 || temp > 126) continue;
			Vec.emplace_back(temp);
		}
		RawText.close();
		return 0;
	}
	else
	{
		cout << "Error, cannot find \"tekst.txt\"" << endl;
		return 1;
	}
}

void Write(vector<char> &Vec)
{
	ofstream CryptedText("CryptedText.txt", ofstream::out);
	for (auto i : Vec)
	{
		CryptedText << i;
	}
	CryptedText.close();
}

void Crypting(vector<char> &Vec, string K)
{
	for (unsigned int j = 0, i = 0; i < Vec.size(); i++)
	{
		Vec[i] = (Vec[i] + K[j] - 12) % 26 + 97;  //12 bo mod_26(97 * 2) == 12
		j++;
		if (j == K.length()) { j = 0; }
	}
}

void Set_N_and_ni(vector<char> &Text, vector<int> &alphabet, int &Col, double &size, int C)
{
	size = 0;

	for (int letter = 0; letter < 26; letter++)
	{
		alphabet[letter] = 0;
		for (unsigned int j = C; j < Text.size(); j += Col)
		{
			if (Text[j] == 97 + letter)
			{
				alphabet[letter]++;
				size++;
			}
		}
	}
}

void Ic(vector<char> &CryptedText, int Columns)
{
	double N = 0, sum = 0;
	vector<int> alphabet;
	alphabet.reserve(26);

	for (unsigned int i = 0; i < 26; i++) { alphabet.emplace_back(0); }

	cout << "\nIc for each column: ";
	for (int C = 0; C < Columns; C++)
	{
		sum = 0;
		Set_N_and_ni(CryptedText, alphabet, Columns, N, C);
		for (int letter = 0; letter < 26; letter++)
		{
			sum += alphabet[letter] * (alphabet[letter] - 1);
		}
		cout << (sum / (N * (N - 1))) << ", ";
	}
}

void MIc(vector<char> &CryptedText, int Columns)
{
	double sum = 0;
	vector<double> N;

	N.reserve(Columns);
	for (auto i = 0; i < Columns; i++)
	{
		N.emplace_back(0);
	}

	vector<vector<int>> alphabet;
	alphabet.reserve(Columns);
	for (int i = 0; i < Columns; i++)
	{
		vector<int> temp;
		temp.reserve(26);
		for (int j = 0; j < 26; j++)
		{
			temp.emplace_back(0);
		}
		alphabet.emplace_back(temp);
	}

	for (auto C = 0; C < Columns; C++) { Set_N_and_ni(CryptedText, alphabet[C], Columns, N[C], C); }

	cout << "\n";

	for (int i = 0; i < Columns - 1; i++)
	{
		for (int j = 1 + i; j < Columns; j++)
		{
			if (i == j) continue;
			sum = 0;
			for (int letter = 0; letter < 26; letter++)
			{
				sum += alphabet[i][letter] * alphabet[j][letter];
			}
			cout << "\n MIc(X^" << i << ", X^" << j << "): " << (sum / (N[i] * N[j])) << "\n";
		}
	}
}

int main()
{
	vector<char> Text;
	if (!Read(Text, "tekst.txt"))
	{
		if (Text.size() != 0)
		{
			for (auto i : Text) { cout << i; }
			string key;
			cout << "\nPlease give me a key: ";
			cin >> key;
			cout << "Your key is: \"" << key << "\"\n";
			Crypting(Text, key);
			for (auto i : Text) { cout << i; }
			Write(Text);
			cout << endl;

			int Col = 0;
			cout << "How many columns do you want: ";
			cin >> Col;

			Ic(Text, Col);
			MIc(Text, Col);
		}
		else
		{
			cout << "tekst.txt is empty!" << endl;
		}
	}

	return 0;
}
