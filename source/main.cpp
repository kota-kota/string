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
	char(0x21),							//!
	char(0x31),							//1
	char(0x41),							//A
	char(0x61),							//a
	char(0xc2), char(0xb1),				//±
	char(0xce), char(0xb1),				//α
	char(0xe2), char(0x91), char(0xa0),	//①
	char(0xe3), char(0x80), char(0x81),	//、
	char(0xe3), char(0x81), char(0xa0),	//だ
	char(0xe3), char(0x83), char(0x80),	//ダ
	char(0xe3), char(0x8d), char(0xbb),	//㍻
	char(0xe6), char(0x9d), char(0xb1),	//東
	char(0xe4), char(0xba), char(0xac),	//京
	char(0x00) };

wchar_t test_utf16be[] = {
	0x0021,				//!
	0x0031,				//1
	0x0041,				//A
	0x0061,				//a
	0x00b1,				//±
	0x03B1,				//α
	0x2460,				//①
	0x3001,				//、
	0x3060,				//だ
	0x30C0,				//ダ
	0x337B,				//㍻
	0x6771,				//東
	0x4EAC,				//京
	0x0000 };

char test_sjis[] = {
	char(0x21),					//!
	char(0x31),					//1
	char(0x41),					//A
	char(0x61),					//a
	char(0x81), char(0x7d),		//±
	char(0x83), char(0xbf),		//α
	char(0x87), char(0x40),		//①
	char(0x81), char(0x41),		//、
	char(0x82), char(0xbe),		//だ
	char(0x83), char(0x5f),		//ダ
	char(0x87), char(0x7e),		//㍻
	char(0x93), char(0x8c),		//東
	char(0x8b), char(0x9e),		//京
	char(0x00) };

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