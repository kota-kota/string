#include "String.hpp"
#include "String_sjis_to_utf16be.hpp"
#include "String_utf16be_to_sjis.hpp"
#include <iostream>
#include <sstream>


namespace {
	const char* enumToString(my::String::CharCode code)
	{
		char* ret = "NONE";
		switch(code) {
			case my::String::CharCode::UTF8:	ret = "UTF8";		break;
			case my::String::CharCode::SJIS:	ret = "SJIS";		break;
			default:	break;
		};
		return ret;
	}

	const char* enumToString(my::WString::CharCode code)
	{
		char* ret = "NONE";
		switch (code) {
		case my::WString::CharCode::UTF16BE:	ret = "UTF16BE";	break;
		default:	break;
		};
		return ret;
	}
}


namespace my {

	//コンストラクタ
	String::String() : std::string(), m_charCode(CharCode::UTF8) {}
	String::String(const char* str, const CharCode code) : std::string(str), m_charCode(code) {}
	String::String(const String& str) : std::string(str), m_charCode(str.m_charCode) {}

	//代入演算子
	String& String::operator=(const char* str)
	{
		std::string::operator=(str);
		this->m_charCode = CharCode::UTF8;
		return *this;
	}
	String& String::operator=(const String& str)
	{
		std::string::operator=(str);
		this->m_charCode = str.m_charCode;
		return *this;
	}

	//文字コード設定
	void String::setCharCode(const CharCode code)
	{
		this->m_charCode = code;
	}

	//UTF16BEへ文字コード変換
	WString String::convertUTF16BE()
	{
		WString ret;
		switch (this->m_charCode) {
		case CharCode::UTF8:	ret = this->convert_UTF8_to_UTF16BE();		break;
		default:	break;
		};
		return ret;
	}

	//SJISへ文字コード変換
	String String::convertSJIS()
	{
		String ret;
		switch (this->m_charCode) {
		case CharCode::UTF8:	ret = this->convert_UTF8_to_SJIS();			break;
		default:	break;
		};
		return ret;
	}

	//文字コード変換(UTF8->UTF16BE)
	WString String::convert_UTF8_to_UTF16BE()
	{
		WString utf16be;

		//変換前(UTF8)の文字情報1バイト文字の並びの数を取得
		const char* utf8 = this->c_str();
		const std::size_t utf8Size = this->size();

		//変換
		std::size_t readSize = 0;
		while (readSize < utf8Size) {
			const std::uint8_t c1 = static_cast<std::uint8_t>(utf8[readSize]);

			std::uint16_t u = 0x0000;
			if (c1 <= 0x7F) {
				//1バイト文字
				u = static_cast<std::uint16_t>(c1);

				readSize += 1;
			}
			else if (c1 <= 0xDF) {
				//2バイト文字
				const std::uint8_t c2 = static_cast<std::uint8_t>(utf8[readSize + 1]);
				u |= static_cast<std::uint16_t>((c1 & 0x1F) << 6);
				u |= static_cast<std::uint16_t>(c2 & 0x3F);

				readSize += 2;
			}
			else {
				//3バイト文字
				const std::uint8_t c2 = static_cast<std::uint8_t>(utf8[readSize + 1]);
				const std::uint8_t c3 = static_cast<std::uint8_t>(utf8[readSize + 2]);
				u |= static_cast<std::uint16_t>((c1 & 0x0F) << 12);
				u |= static_cast<std::uint16_t>((c2 & 0x3F) << 6);
				u |= static_cast<std::uint16_t>(c3 & 0x3F);

				readSize += 3;
			}

			utf16be.push_back(static_cast<char16_t>(u));
		}

		//変換後(UTF16)の文字コードを設定
		utf16be.setCharCode(my::WString::CharCode::UTF16BE);
		return utf16be;
	}

	//文字コード変換(UTF8->SJIS)
	String String::convert_UTF8_to_SJIS()
	{
		String sjis;

		//まずはUTF16BEに変換
		WString utf16be = this->convert_UTF8_to_UTF16BE();

		//変換前(UTF16BE)の1バイト文字の並びの数を取得
		const std::size_t utf16beSize = utf16be.size();

		//変換
		std::size_t readSize = 0;
		while (readSize < utf16beSize) {
			const std::uint16_t u = static_cast<std::uint16_t>(utf16be.c_str()[readSize]);

			std::uint16_t s = 0x0000;
			if ((u >= 0x0000) && (u <= 0x007F)) {
				//UTF16BE->SJIS変換テーブルを使用して変換
				//0x0000がテーブルの0番目となる
				s = utf16be_to_sjis_0000_007F[u];
			}
			else if ((u >= 0x2000) && (u <= 0x9FFF)) {
				//UTF16BE->SJIS変換テーブルを使用して変換
				//0x2000がテーブルの0番目となる
				s = utf16be_to_sjis_2000_9FFF[u - 0x2000];
			}
			else {
				//UTF16BE->SJIS変換テーブルを使用して変換
				//0xFF00がテーブルの0番目となる
				s = utf16be_to_sjis_FF00_FFFF[u - 0xFF00];
			}
			readSize += 1;

			//SJISの1バイト目
			std::uint8_t s1 = static_cast<std::uint8_t>((s & 0xFF00) >> 8);
			sjis.push_back(static_cast<char>(s1));
			//SJISの2バイト目
			std::uint8_t s2 = static_cast<std::uint8_t>(s & 0x00FF);
			sjis.push_back(static_cast<char>(s2));
		}

		//変換後(SJIS)の文字コードを設定
		sjis.setCharCode(my::String::CharCode::SJIS);
		return sjis;
	}

	//ログ出力
	void String::printLog()
	{
		std::string out;
		for (std::int32_t i = 0; i < this->size(); i++) {
			std::uint8_t t = static_cast<std::uint8_t>(this->c_str()[i]);
			std::stringstream ss;
			ss << std::hex << static_cast<std::int32_t>(t);
			out = out + "0x" + ss.str() + ",";
		}
		out = out + "0x00 (" + enumToString(this->m_charCode) + ")";
		std::cout << out << std::endl;
	}

}

namespace my {

	//コンストラクタ
	WString::WString() : std::wstring(), m_charCode(CharCode::UTF16BE) {}
	WString::WString(const wchar_t* str, const CharCode code) : std::wstring(str), m_charCode(code) {}
	WString::WString(const WString& str) : std::wstring(str), m_charCode(str.m_charCode) {}

	//代入演算子
	WString& WString::operator=(const wchar_t* str)
	{
		std::wstring::operator=(str);
		this->m_charCode = CharCode::UTF16BE;
		return *this;
	}
	WString& WString::operator=(const WString& str)
	{
		std::wstring::operator=(str);
		this->m_charCode = str.m_charCode;
		return *this;
	}

	//文字コード設定
	void WString::setCharCode(const CharCode code)
	{
		this->m_charCode = code;
	}

	//ログ出力
	void WString::printLog()
	{
		std::string out;
		for (std::int32_t i = 0; i < this->size(); i++) {
			std::uint16_t t = static_cast<std::uint16_t>(this->c_str()[i]);
			std::stringstream ss;
			ss << std::hex << static_cast<std::int32_t>(t);
			out = out + "0x" + ss.str() + ",";
		}
		out = out + "0x0000 (" + enumToString(this->m_charCode) + ")";
		std::cout << out << std::endl;
	}

}
