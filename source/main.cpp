/**
 * @file main.cpp
 * @author kota-kota
 * @brief 
 * @version 0.1
 * @date 2020-05-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "String.hpp"
#include <iostream>
#include <cstdlib>

char test_utf8[] = {
	char(33),							//!		0x21
	char(49),							//1		0x31
	char(65),							//A		0x41
	char(97),							//a		0x61
	char(-62), char(-79),				//±		0xc2, 0xb1
	char(-50), char(-79),				//α		0xce, 0xb1
	char(-30), char(-111), char(-96),	//①		0xe2, 0x91, 0xa0
	char(-29), char(-128), char(-127),	//、	0xe3, 0x80, 0x81
	char(-29), char(-127), char(-96),	//だ	0xe3, 0x81, 0xa0
	char(-29), char(-125), char(-128),	//ダ	0xe3, 0x83, 0x80
	char(-29), char(-115), char(-69),	//㍻	0xe3, 0x8d, 0xbb
	char(-26), char(-99), char(-79),	//東	0xe6, 0x9d, 0xb1
	char(-28), char(-70), char(-84),	//京	0xe4, 0xba, 0xac
	char(0) };

wchar_t test_utf16be[] = {
	wchar_t(0x0021),		//!		0x0021
	wchar_t(0x0031),		//1		0x0031
	wchar_t(0x0041),		//A		0x0041
	wchar_t(0x0061),		//a		0x0061
	wchar_t(0x00b1),		//±		0x00b1
	wchar_t(0x03B1),		//α		0x03B1
	wchar_t(0x2460),		//①		0x2460
	wchar_t(0x3001),		//、	0x3001
	wchar_t(0x3060),		//だ	0x3060
	wchar_t(0x30C0),		//ダ	0x30C0
	wchar_t(0x337B),		//㍻	0x337B
	wchar_t(0x6771),		//東	0x6771
	wchar_t(0x4EAC),		//京	0x4EAC
	wchar_t(0x0000) };

char test_sjis[] = {
	char(33),					//!		0x21
	char(49),					//1		0x31
	char(65),					//A		0x41
	char(97),					//a		0x61
	char(-127), char(125),		//±		0x81, 0x7d
	char(-125), char(-65),		//α		0x83, 0xbf
	char(-121), char(64),		//①		0x87, 0x40
	char(-127), char(65),		//、	0x81, 0x41
	char(-126), char(-66),		//だ	0x82, 0xbe
	char(-125), char(95),		//ダ	0x83, 0x5f
	char(-121), char(126),		//㍻	0x87, 0x7e
	char(-109), char(-116),		//東	0x93, 0x8c
	char(-117), char(-98),		//京	0x8b, 0x9e
	char(0) };

/**
 * @brief テスト1：UTF8の文字コード変換
 * 
 */
void test1()
{
	std::cout << "Test1: UTF8" << std::endl;

	std::cout << "* UTF8" << std::endl;
	my::String utf8 = test_utf8;
	utf8.setCharCode(my::String::CharCode::UTF8);
	utf8.result(test_utf8);

	std::cout << "* UTF8 -> UTF16BE" << std::endl;
	my::WString utf16be = utf8.convertUTF16BE();
	utf16be.result(test_utf16be);

	std::cout << "* UTF8 -> SJIS" << std::endl;
	my::String sjis = utf8.convertSJIS();
	sjis.result(test_sjis);
}

/**
 * @brief テスト2：SJISの文字コード変換
 * 
 */
void test2()
{
	std::cout << "Test2: SJIS" << std::endl;

	std::cout << "* SJIS" << std::endl;
	my::String sjis = test_sjis;
	sjis.setCharCode(my::String::CharCode::SJIS);
	sjis.result(test_sjis);

	std::cout << "* SJIS -> UTF8" << std::endl;
	my::String utf8 = sjis.convertUTF8();
	utf8.result(test_utf8);

	std::cout << "* SJIS -> UTF16BE" << std::endl;
	my::WString utf16be = sjis.convertUTF16BE();
	utf16be.result(test_utf16be);
}

/**
 * @brief テスト3：UTF16BEの文字コード変換
 * 
 */
void test3()
{
	std::cout << "Test3: UTF16BE" << std::endl;
	my::WString utf16be = test_utf16be;
	utf16be.setCharCode(my::WString::CharCode::UTF16BE);
	utf16be.result(test_utf16be);

	std::cout << "* UTF16BE -> UTF8" << std::endl;
	std::cout << "* UTF16BE -> SJIS" << std::endl;
}

/**
 * @brief メイン関数：文字列を扱うクラスのテスト実行
 * 
 * @return int 0
 */
int main()
{
	test1(); std::cout << std::endl;
	test2(); std::cout << std::endl;
	test3(); std::cout << std::endl;
	system("pause");
	return 0;
}