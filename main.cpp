#include <Database/SQL.h>
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
	SQL sql;
	sql.ExecuteFile("test.txt");
	sql.Interactive();

	cout << endl;
	return 0;
}