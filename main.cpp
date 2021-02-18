#include <iostream>
#include <string>
#include "slsp.cpp"
using namespace std;

int main(int argc, char **argv)
{

  int problem_size;
	if (argv[1] == nullptr)
	{
		cout << "paramètre non renseigné" << endl;
		return 0;
	}
	else
	{
		problem_size = stoi(argv[1]);
	}

  SLSP p(problem_size);

	return 0;
}
