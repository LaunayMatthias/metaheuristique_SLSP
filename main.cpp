#include <iostream>
#include <string>
#include <chrono>
#include "slsp.cpp"
using namespace std::chrono;

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

  auto start = high_resolution_clock::now();

  TabuSearch(p, 10000);

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);

  cout << "Time taken by function: "
          << duration.count() << " milliseconds" << endl;

	return 0;
}
