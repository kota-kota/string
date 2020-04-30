#ifndef INCLUDED_STRING_HPP
#define INCLUDED_STRING_HPP

#include <string>

namespace my {

	class String;
	class WString;

	class String final : public std::string {

	public:
		//文字コード
		enum class CharCode {
			UTF8,
			SJIS,
		};

	private:
		CharCode	m_charCode;		//文字コード

	public:
		//コンストラクタ
		String();
		String(const char* str, const CharCode code = CharCode::UTF8);
		String(const String& str);

		//代入演算子
		String& operator=(const char* str);
		String& operator=(const String& str);

		//文字コード設定
		void setCharCode(const CharCode code);

		//UTF8へ文字コード変換
		String convertUTF8();

		//UTF16BEへ文字コード変換
		WString convertUTF16BE();

		//SJISへ文字コード変換
		String convertSJIS();

	private:
		//文字コード変換(UTF8->UTF16BE)
		WString convert_UTF8_to_UTF16BE();

		//文字コード変換(UTF8->SJIS)
		String convert_UTF8_to_SJIS();

		//文字コード変換(SJIS->UTF16BE)
		WString convert_SJIS_to_UTF16BE();

		//文字コード変換(SJIS->UTF8)
		String convert_SJIS_to_UTF8();

	public:
		//結果
		void result(const String& current);
	};
}

namespace my {

	class WString final : public std::wstring {

	public:
		//文字コード
		enum class CharCode {
			UTF16BE,
		};

	private:
		CharCode	m_charCode;		//文字コード

	public:
		//コンストラクタ
		WString();
		WString(const wchar_t* str, const CharCode code = CharCode::UTF16BE);
		WString(const WString& str);

		//代入演算子
		WString& operator=(const wchar_t* str);
		WString& operator=(const WString& str);

		//文字コード設定
		void setCharCode(const CharCode code);

		//文字コード変換


		//結果
		void result(const WString& current);
	};
}


#endif //INCLUDED_STRING_HPP
