/**
 * @file String.hpp
 * @author kota-kota
 * @brief 文字列を扱うクラスの定義
 * @version 0.1
 * @date 2020-05-01
 * 
 * @copyright Copyright (c) 2020
 */
#ifndef INCLUDED_STRING_HPP
#define INCLUDED_STRING_HPP

#include <string>

namespace my {

	class String;
	class WString;

	/**
	 * @class String
	 * @brief マルチバイト文字の文字列を扱うクラス
	 */
	class String final : public std::string {

	public:
		/**
		 * @enum CharCode
		 * @brief 文字コード定義
		 */
		enum class CharCode {
			UTF8,	//!< UTF-8
			SJIS,	//!< Shift-JIS
		};

	private:
		CharCode	m_charCode;		//!< 文字コード

	public:
		//! デフォルトコンストラクタ
		String();
		//! コンストラクタ
		String(const char* str, const CharCode code = CharCode::UTF8);
		//! コピーコンストラクタ
		String(const String& str);

		//! コピー代入演算子のオーバーロード
		String& operator=(const char* str);
		//! コピー代入演算子のオーバーロード
		String& operator=(const String& str);

		//! 文字コードを設定する
		void setCharCode(const CharCode code);

		//! 文字列の文字コードをUTF8に変換する
		String convertUTF8();

		//! 文字列の文字コードをUTF16BEに変換する
		WString convertUTF16BE();

		//! 文字列の文字コードをSJISに変換する
		String convertSJIS();

	public:
		//! 正解データを入力して、結果を出力する
		void result(const String& current);
	};
}

namespace my {

	/**
	 * @class WString
	 * @brief ワイド文字の文字列を扱うクラス
	 */
	class WString final : public std::wstring {

	public:
		/**
		 * @enum CharCode
		 * @brief 文字コード定義
		 */
		enum class CharCode {
			UTF16BE,	//!< UTF-16(BigEndian)
		};

	private:
		CharCode	m_charCode;		//!< 文字コード

	public:
		//! デフォルトコンストラクタ
		WString();
		//! コンストラクタ
		WString(const wchar_t* str, const CharCode code = CharCode::UTF16BE);
		//! コピーコンストラクタ
		WString(const WString& str);

		//! コピー代入演算子のオーバーロード
		WString& operator=(const wchar_t* str);
		//! コピー代入演算子のオーバーロード
		WString& operator=(const WString& str);

		//! 文字コードを設定する
		void setCharCode(const CharCode code);

	public:
		//! 正解データを入力して、結果を出力する
		void result(const WString& current);
	};
}


#endif //INCLUDED_STRING_HPP
