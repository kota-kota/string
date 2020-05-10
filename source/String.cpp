/**
 * @file String.cpp
 * @author kota-kota
 * @brief 文字列を扱うクラスの実装
 * @version 0.1
 * @date 2020-05-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "String.hpp"
#include "String_sjis_to_utf16be.hpp"
#include "String_utf16be_to_sjis.hpp"
#include <iostream>
#include <sstream>

namespace {
	/**
	 * 
	 * @brief 指定されたSJISコードからUTF16BEコードを取得する
	 * 
	 * @param [in] sjis SJISコード
	 * @retval 0x0000 取得失敗
	 * @retval >0x0000 取得成功(UTF16BEコード)
	 * 
	 * @par 詳細
	 *      SJISコードを入力として、SJIS->UTF16BE変換テーブル(sjis_to_utf16be[])から検索する。
	 *      ヒットした場合、UTF16BEコードを返す。ヒットしなかった場合、0x0000を返す。
	 * @todo
	 *      線形探索になっているため、二分木探索をする
	 */
	std::uint16_t searchTable_SJIS_to_UTF16BE(const std::uint16_t sjis)
	{
		//TODO: binary search
		std::uint16_t utf16be = 0x0000;
		for (std::int32_t i = 0; i < num_sjis_to_utf16be; i++) {
			SJIS_TO_UTF16BE target = sjis_to_utf16be[i];
			if (target.sjis == sjis) {
				utf16be = target.utf16be;
				break;
			}
		}
		return utf16be;
	}

	/**
	 * @brief 指定されたUTF16BEコードからSJISコードを取得する
	 * 
	 * @param [in] utf16be UTF16BEコード
	 * @retval 0x0000 取得失敗
	 * @retval >0x0000 取得成功(SJISコード)
	 * 
	 * @par 詳細
	 *      UTF16BEコードを入力として、UTF16BE->SJIS変換テーブル(utf16be_to_sjis[])から検索する。
	 *      ヒットした場合、SJISコードを返す。ヒットしなかった場合、0x0000を返す。
	 * @todo
	 *      線形探索になっているため、二分木探索をする
	 */
	std::uint16_t searchTable_UTF16BE_to_SJIS(const std::uint16_t utf16be)
	{
		//TODO: binary search
		std::uint16_t sjis = 0x0000;
		for (std::int32_t i = 0; i < num_utf16be_to_sjis; i++) {
			UTF16BE_TO_SJIS target = utf16be_to_sjis[i];
			if (target.utf16be == utf16be) {
				sjis = target.sjis;
				break;
			}
		}
		return sjis;
	}
}

namespace {
	/**
	 * @brief 文字コード列挙体定義を文字列に変換する
	 * 
	 * @param [in] code 文字コード
	 * @retval "UTF8" my::String::CharCode::UTF8
	 * @retval "SJIS" my::String::CharCode::SJIS
	 * @retval "NONE" 上記以外
	 * 
	 * @par 詳細
	 *      なし
	 */
	const char* enumToString(const my::String::CharCode code)
	{
		const char* ret = "NONE";
		switch(code) {
			case my::String::CharCode::UTF8:	ret = "UTF8";		break;
			case my::String::CharCode::SJIS:	ret = "SJIS";		break;
			default:	break;
		};
		return ret;
	}

	/**
	 * @brief 文字コード列挙体定義を文字列に変換する
	 * 
	 * @param [in] code 文字コード
	 * @retval "UTF16BE" my::WString::CharCode::UTF16BE
	 * @retval "NONE" 上記以外
	 * 
	 * @par 詳細
	 *      なし
	 */
	const char* enumToString(const my::WString::CharCode code)
	{
		const char* ret = "NONE";
		switch (code) {
		case my::WString::CharCode::UTF16BE:	ret = "UTF16BE";	break;
		default:	break;
		};
		return ret;
	}
}

namespace {
	/**
	 * @brief UTF8からUTF16Eへ文字コードを変換する
	 * 
	 * @param [in] utf8 文字コードがUTF8の文字列を持つStringインスタンス
	 * @return WString 文字コードがUTF16BEの文字列を持つWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	my::WString convert_UTF8_to_UTF16BE(const my::String& utf8)
	{
		my::WString utf16be;

		//変換
		std::size_t readSize = 0;
		while (readSize < utf8.size()) {
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

	/**
	 * @brief UTF8からSJISへ文字コードを変換する
	 * 
	 * @param [in] utf8 文字コードがUTF8の文字列を持つStringインスタンス
	 * @return String 文字コードがSJISの文字列を持つWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	my::String convert_UTF8_to_SJIS(const my::String& utf8)
	{
		my::String sjis;

		//まずはUTF16BEに変換
		my::WString utf16be = convert_UTF8_to_UTF16BE(utf8);

		//UTF16BE->SJIS変換
		std::size_t readSize = 0;
		while (readSize < utf16be.size()) {
			const std::uint16_t u = static_cast<std::uint16_t>(utf16be.c_str()[readSize]);

			//UTF16BE->SJIS変換テーブルから検索する
			//変換テーブルからヒットしなかった場合は空白文字とする
			std::uint16_t s = searchTable_UTF16BE_to_SJIS(u);
			if (s == 0x0000) {
				s = 0x8140;
			}

			readSize += 1;

			if (s <= 0x007F) {
				//SJISの1バイト目
				std::uint8_t s1 = static_cast<std::uint8_t>(s & 0x00FF);
				sjis.push_back(static_cast<char>(s1));
			}
			else {
				//SJISの1バイト目
				std::uint8_t s1 = static_cast<std::uint8_t>((s & 0xFF00) >> 8);
				sjis.push_back(static_cast<char>(s1));
				//SJISの2バイト目
				std::uint8_t s2 = static_cast<std::uint8_t>(s & 0x00FF);
				sjis.push_back(static_cast<char>(s2));
			}
		}

		//変換後(SJIS)の文字コードを設定
		sjis.setCharCode(my::String::CharCode::SJIS);
		return sjis;
	}

	/**
	 * @brief SJISからUTF16BEへ文字コードを変換する
	 * 
	 * @param [in] sjis 文字コードがSJISの文字列を持つStringインスタンス
	 * @return WString 文字コードがUTF16BEの文字列を持つWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	my::WString convert_SJIS_to_UTF16BE(const my::String& sjis)
	{
		my::WString utf16be;

		//変換
		std::size_t readSize = 0;
		while (readSize < sjis.size()) {
			std::uint16_t s = 0x0000;

			//SJIS1バイト目
			const std::uint8_t s1 = static_cast<std::uint8_t>(sjis[readSize]);
			if (((s1 >= 0x81) && (s1 <= 0x9F)) || ((s1 >= 0xE0) && (s1 <= 0xFC))) {
				//SJIS全角文字の1バイト目なら2バイト文字

				//SJIS2バイト目を取得してSJIS全角文字コードに変換
				const std::uint8_t s2 = static_cast<std::uint8_t>(sjis[readSize + 1]);
				s = static_cast<std::uint16_t>((s1 << 8) | s2);

				readSize += 2;
			}
			else {
				//SJIS半角文字なら1バイト文字

				//SJIS1バイト目がSJIS半角文字コード
				s = s1;

				readSize += 1;
			}

			//SJIS->UTF16BE変換テーブルから検索する
			//変換テーブルからヒットしなかった場合は空白文字とする
			std::uint16_t u = searchTable_SJIS_to_UTF16BE(s);
			if (u == 0x0000) {
				u = 0x3000;
			}

			utf16be.push_back(static_cast<char16_t>(u));
		}

		//変換後(UTF16)の文字コードを設定
		utf16be.setCharCode(my::WString::CharCode::UTF16BE);
		return utf16be;
	}

	/**
	 * @brief SJISからUTF8へ文字コードを変換する
	 * 
	 * @param [in] sjis 文字コードがSJISの文字列を持つStringインスタンス
	 * @return String 文字コードがUTF8の文字列を持つWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	my::String convert_SJIS_to_UTF8(const my::String& sjis)
	{
		my::String utf8;

		//まずはUTF16BEに変換
		my::WString utf16be = convert_SJIS_to_UTF16BE(sjis);

		//UTF16BE->UTF8変換
		for (std::size_t i = 0; i < utf16be.size(); i++) {
			const std::uint16_t u = static_cast<std::uint16_t>(utf16be[i]);

			//1バイト毎に分割
			const std::uint8_t u1 = static_cast<std::uint8_t>((u & 0xFF00) >> 8);
			const std::uint8_t u2 = static_cast<std::uint8_t>(u & 0x00FF);

			const std::uint16_t c = static_cast<std::uint16_t>((u1 * 0x100) + u2);
			if (c <= 0x007F) {
				//0x0000-0x007F
				utf8.push_back(static_cast<char>(c));
			}
			else if (c <= 0x07FF) {
				//0x0080-0x07FF
				const char c1 = static_cast<char>(0xC0 | (c >> 6));
				const char c2 = static_cast<char>(0x80 | (c & 0x003F));
				utf8.push_back(c1);
				utf8.push_back(c2);
			}
			else {
				//0x800-0xFFFF
				const char c1 = static_cast<char>(0xE0 | ((c >> 12) & 0x000F));
				const char c2 = static_cast<char>(0x80 | ((c >> 6) & 0x003F));
				const char c3 = static_cast<char>(0x80 | (c & 0x003F));
				utf8.push_back(c1);
				utf8.push_back(c2);
				utf8.push_back(c3);
			}
		}

		//変換後(UTF8)の文字コードを設定
		utf8.setCharCode(my::String::CharCode::UTF8);
		return utf8;
	}
}


namespace my {

	/**
	 * @brief デフォルトコンストラクタ
	 * 
	 * @par 詳細
	 *      なし
	 */
	String::String() : std::string(), m_charCode(CharCode::UTF8) {}
	/**
	 * @brief コンストラクタ
	 * 
	 * @param [in] str 文字列ポインタ
	 * @param [in] code 文字コード
	 * 
	 * @par 詳細
	 *      なし
	 */
	String::String(const char* str, const CharCode code) : std::string(str), m_charCode(code) {}

	/**
	 * @brief コピーコンストラクタ
	 * 
	 * @param [in] str コピー元のStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	String::String(const String& str) : std::string(str), m_charCode(str.m_charCode) {}

	/**
	 * @brief コピー代入演算子のオーバーロード
	 * 
	 * @param [in] str コピー元の文字列ポインタ
	 * @return String& コピー先のStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	String& String::operator=(const char* str)
	{
		std::string::operator=(str);
		this->m_charCode = CharCode::UTF8;
		return *this;
	}
	/**
	 * @brief コピー代入演算子のオーバーロード
	 * 
	 * @param [in] str コピー元のStringインスタンス
	 * @return String& コピー先のStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	String& String::operator=(const String& str)
	{
		std::string::operator=(str);
		this->m_charCode = str.m_charCode;
		return *this;
	}

	/**
	 * @brief 文字コードを設定する
	 * 
	 * @param [in] code 文字コード
	 * 
	 * @par 詳細
	 *      なし
	 */
	void String::setCharCode(const CharCode code)
	{
		this->m_charCode = code;
	}

	/**
	 * @brief 文字列の文字コードをUTF8に変換する
	 * 
	 * @return String 文字コードがUTF8の文字列を持つStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	String String::convertUTF8()
	{
		String ret;
		switch (this->m_charCode) {
		case CharCode::UTF8:	break;
		case CharCode::SJIS:	ret = convert_SJIS_to_UTF8(*this);		break;
		default:	break;
		};
		return ret;
	}

	/**
	 * @brief 文字列の文字コードをUTF16BEに変換する
	 * 
	 * @return String 文字コードがUTF16BEの文字列を持つStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString String::convertUTF16BE()
	{
		WString ret;
		switch (this->m_charCode) {
		case CharCode::UTF8:	ret = convert_UTF8_to_UTF16BE(*this);		break;
		case CharCode::SJIS:	ret = convert_SJIS_to_UTF16BE(*this);		break;
		default:	break;
		};
		return ret;
	}

	/**
	 * @brief 文字列の文字コードをSJISに変換する
	 * 
	 * @return String 文字コードがSJISの文字列を持つStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	String String::convertSJIS()
	{
		String ret;
		switch (this->m_charCode) {
		case CharCode::UTF8:	ret = convert_UTF8_to_SJIS(*this);			break;
		case CharCode::SJIS:	break;
		default:	break;
		};
		return ret;
	}

	/**
	 * @brief 正解データを入力して、結果を出力する
	 * 
	 * @return String 正解文字列のStringインスタンス
	 * 
	 * @par 詳細
	 *      入力された正解文字列と比較して、一致するか判定します。
	 *      一致していれば"[OK]"の文字列を出力します。
	 *      不一致であれば"[NG]"の文字列を出力します。
	 *      その後、結果に関わらず、自身の文字列を出力します。
	 *      出力先は標準出力になります。
	 */
	void String::result(const String& current)
	{
		bool res = (*this == current) ? true : false;
		std::string out;
		out = res ? "[OK] " : "[NG] ";
		for (std::size_t i = 0; i < this->size(); i++) {
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

	/**
	 * @brief デフォルトコンストラクタ
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString::WString() : std::wstring(), m_charCode(CharCode::UTF16BE) {}
	/**
	 * @brief コンストラクタ
	 * 
	 * @param [in] str 文字列ポインタ
	 * @param [in] code 文字コード
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString::WString(const wchar_t* str, const CharCode code) : std::wstring(str), m_charCode(code) {}

	/**
	 * @brief コピーコンストラクタ
	 * 
	 * @param [in] str コピー元のStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString::WString(const WString& str) : std::wstring(str), m_charCode(str.m_charCode) {}

	/**
	 * @brief コピー代入演算子のオーバーロード
	 * 
	 * @param [in] str コピー元の文字列ポインタ
	 * @return WString& コピー先のWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString& WString::operator=(const wchar_t* str)
	{
		std::wstring::operator=(str);
		this->m_charCode = CharCode::UTF16BE;
		return *this;
	}
	/**
	 * @brief コピー代入演算子のオーバーロード
	 * 
	 * @param [in] str コピー元のWStringインスタンス
	 * @return WString& コピー先のWStringインスタンス
	 * 
	 * @par 詳細
	 *      なし
	 */
	WString& WString::operator=(const WString& str)
	{
		std::wstring::operator=(str);
		this->m_charCode = str.m_charCode;
		return *this;
	}

	/**
	 * @brief 文字コードを設定する
	 * 
	 * @param [in] code 文字コード
	 * 
	 * @par 詳細
	 *      なし
	 */
	void WString::setCharCode(const CharCode code)
	{
		this->m_charCode = code;
	}

	/**
	 * @brief 正解データを入力して、結果を出力する
	 * 
	 * @return WString 正解文字列のWStringインスタンス
	 * 
	 * @par 詳細
	 *      入力された正解文字列と比較して、一致するか判定します。
	 *      一致していれば"[OK]"の文字列を出力します。
	 *      不一致であれば"[NG]"の文字列を出力します。
	 *      その後、結果に関わらず、自身の文字列を出力します。
	 *      出力先は標準出力になります。
	 */
	void WString::result(const WString& current)
	{
		bool res = (*this == current) ? true : false;
		std::string out;
		out = res ? "[OK] " : "[NG] ";
		for (std::size_t i = 0; i < this->size(); i++) {
			std::uint16_t t = static_cast<std::uint16_t>(this->c_str()[i]);
			std::stringstream ss;
			ss << std::hex << static_cast<std::int32_t>(t);
			out = out + "0x" + ss.str() + ",";
		}
		out = out + "0x0000 (" + enumToString(this->m_charCode) + ")";
		std::cout << out << std::endl;
	}
}
