#include "String.hpp"
#include <cstdlib>

void test1()
{
	my::String utf8 = "テスト";
	my::WString utf16be = utf8.convertUTF16BE();
	my::String sjis = utf8.convertSJIS();
	utf8.printLog();
	utf16be.printLog();
	sjis.printLog();

	my::String sjis2 = sjis;
	sjis2.printLog();
}

int main()
{
	test1();
	system("pause");
	return 0;
}