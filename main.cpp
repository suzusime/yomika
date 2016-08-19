#include <iostream>
#include <string>
#include <fstream>
#include <regex>

//ストリームから全文字列を読み取って返す函数
//改行はLFに統一する。文字列の最後にもLFが一つ入る
std::string get_text(std::ifstream& ifs_){
	using namespace std;
	string str,tmp;
	while(getline(ifs_,tmp)){
		str+=tmp + '\n';//LFで改行する。文字列の最後にもLFが入ることに注意
	}
	string::size_type pos;
	while( (pos = str.find("\r")) != string::npos ){//CRを削除する
		str.erase(pos,1);
	}
	return str;
}

std::string remove_comment(std::string src){
	using namespace std;
	regex re( R"(≪.*?≫)" );
	string dest = regex_replace(src, re, "");
	return dest;
}

std::string style_h1(std::string src){
	using namespace std;
	regex re( R"(■(.*))" );//文字列の先頭にしか^がマッチしないので行頭にマッチさせるのどうすればいいんだろうなぁ……
	string dest = regex_replace(src, re, "<h1>$1</h1>");
	return dest;
}
std::string style_h2(std::string src){
	using namespace std;
	regex re( R"(◆(.*))" );
	string dest = regex_replace(src, re, "<h2>$1</h2>");
	return dest;
}
std::string style_h3(std::string src){
	using namespace std;
	regex re( R"(◎(.*))" );
	string dest = regex_replace(src, re, "<h3>$1</h3>");
	return dest;
}
std::string style_h4(std::string src){
	using namespace std;
	regex re( R"(○(.*))" );
	string dest = regex_replace(src, re, "<h4>$1</h4>");
	return dest;
}
std::string style_h5(std::string src){
	using namespace std;
	regex re( R"(△(.*))" );
	string dest = regex_replace(src, re, "<h5>$1</h5>");
	return dest;
}
std::string style_h6(std::string src){
	using namespace std;
	regex re( R"(☆(.*))" );
	string dest = regex_replace(src, re, "<h6>$1</h6>");
	return dest;
}


std::string process_text(std::string src){
	using namespace std;
	string dest = remove_comment(src);
	dest = style_h1(dest);
	dest = style_h2(dest);
	dest = style_h3(dest);
	dest = style_h4(dest);
	dest = style_h5(dest);
	dest = style_h6(dest);
	return dest;
}

std::string embed_to_html(std::string src){
	using namespace std;
	ifstream ifs("template.html");
	if(ifs.fail()){
		cerr << "テンプレートの読み込みに失敗しました" << endl;
		return "";
	}
	string str = get_text(ifs);
	regex re( R"(＠本文)" );
	string dest = regex_replace(str, re, src);
	return dest;
}

//開始地点
int main(int argc, char* argv[]){
	using namespace std;
	if(argc!=2){
		cerr << "引数の数は1つにしてください" <<endl;
		return -1;
	}
	ifstream ifs(argv[1]);
	if(ifs.fail()){
		cerr << "ファイルの読み込みに失敗しました" << endl;
		return -1;
	}
	string str = get_text(ifs);
	string pstr = process_text(str);
	cout << embed_to_html(pstr);
	return 0;
}