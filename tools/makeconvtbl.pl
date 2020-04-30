# main
{
	# リスト作成
	&jisx0201_to_unicode();
	&jisx0208_to_unicode();
	&jiswin_to_unicode();

	# テーブル作成(SJIS to UTF16BE)
	$file_s2u = ">String_sjis_to_utf16be.hpp";
	if (!open(FILE_OUT_S2U, $file_s2u)) {
		printf("File open error.\n");
		exit(1);
	}
	print FILE_OUT_S2U "struct SJIS_TO_UTF16BE {\n";
	print FILE_OUT_S2U "\tstd::uint16_t sjis;\n";
	print FILE_OUT_S2U "\tstd::uint16_t utf16be;\n";
	print FILE_OUT_S2U "};\n";
	print FILE_OUT_S2U "constexpr SJIS_TO_UTF16BE sjis_to_utf16be[] = {\n";
	foreach $sjis (sort keys %sjis2utf16be) {
		print FILE_OUT_S2U "\t{ $sjis,\t$sjis2utf16be{$sjis} },\n";
	}
	print FILE_OUT_S2U "};\n";
	print FILE_OUT_S2U "constexpr std::int32_t num_sjis_to_utf16be = sizeof(sjis_to_utf16be) / sizeof(sjis_to_utf16be[0]);\n";
	close(FILE_OUT_S2U);

	# テーブル作成(UTF16BE to SJIS)
	$file_u2s = ">String_utf16be_to_sjis.hpp";
	if (!open(FILE_OUT_U2S, $file_u2s)) {
		printf("File open error.\n");
		exit(1);
	}
	print FILE_OUT_U2S "struct UTF16BE_TO_SJIS {\n";
	print FILE_OUT_U2S "\tstd::uint16_t utf16be;\n";
	print FILE_OUT_U2S "\tstd::uint16_t sjis;\n";
	print FILE_OUT_U2S "};\n";
	print FILE_OUT_U2S "constexpr UTF16BE_TO_SJIS utf16be_to_sjis[] = {\n";
	foreach $utf (sort keys %utf16be2sjis) {
		print FILE_OUT_U2S "\t{ $utf,\t$utf16be2sjis{$utf} },\n";
	}
	print FILE_OUT_U2S "};\n";
	print FILE_OUT_U2S "constexpr std::int32_t num_utf16be_to_sjis = sizeof(utf16be_to_sjis) / sizeof(utf16be_to_sjis[0]);\n";
	close(FILE_OUT_U2S);
}

# JIS X 0201コードからUnicodeへの変換テーブルの作成
sub jisx0201_to_unicode{
	my $cj; # SJIS char
	my $cu; # Unicode char
	my $s;  # string buffer

	# 変換テーブルのオープン
	$file_open = "<jis0201.txt";
	if (!open(FILE_IN, $file_open)) {
		printf("File open error.\n");
		exit(1);
	}

	while ($s = <FILE_IN>) {
		if ($s =~ /^#/) {next;} # 先頭が'#'の行を無視

		$cj = "0x".substr($s, 2, 4);
		$cu = "0x".substr($s, 9, 4);
		$sjis2utf16be{$cj} = $cu;
		$utf16be2sjis{$cu} = $cj;
	}

	# 変換テーブルのクローズ
	close(FILE_IN);
}

#
# JIS X 0208コードからUnicodeへの変換テーブルの作成
#
sub jisx0208_to_unicode {
	my $cj; # SJIS char
	my $cu; # Unicode char
	my $s;  # string buffer

	# 変換テーブルのオープン
	$file_open = "<jis0208.txt";
	if (!open(FILE_IN, $file_open)) {
		printf("File open error.\n");
		exit(1);
	}

	while ($s = <FILE_IN>) {
		if ($s =~ /^#/) {next;} # 先頭が'#'の行を無視

		$cj = "0x".substr($s, 2, 4);
		$cu = "0x".substr($s, 16, 4);
		$sjis2utf16be{$cj} = $cu;
		$utf16be2sjis{$cu} = $cj;
	}

	# 変換テーブルのクローズ
	close(FILE_IN);
}

#
# Windows特殊文字からUnicodeへの変換テーブルの作成
#
sub jiswin_to_unicode {
	my $cj; # SJIS char
	my $cu; # Unicode char
	my $s;  # string buffer

	# 変換テーブルのオープン
	$file_open = "<jiswin.txt";
	if (!open(FILE_IN, $file_open)) {
		printf("File open error.\n");
		exit(1);
	}

	while ($s = <FILE_IN>) {
		if ($s =~ /^#/) {next;} # 先頭が'#'の行を無視

		$cj = "0x".substr($s, 2, 4);
		$cu = "0x".substr($s, 9, 4);
		$sjis2utf16be{$cj} = $cu;
		$utf16be2sjis{$cu} = $cj;
	}

	# 変換テーブルのクローズ
	close(FILE_IN);
}

