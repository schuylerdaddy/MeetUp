#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

typedef std::map<std::string,std::string> record;

struct CSVReader{
	  std::vector<record> rows;
  std::vector<std::string> schema; 

  void loadRecords(std::string filename){
    char c;
    std::ifstream is (filename,std::ios::binary);
    std::string buff = "";
    int ctr=0;

    while(is>>std::noskipws>>c && c != '\r' && c!= '\n'){
      if(c==','){
        schema.push_back(trim_end(buff));
        buff = "";
      }
      else if(!isspace(c) || buff != "")
        buff += c;
    }

    if(buff != ""){
       schema.push_back(trim_end(buff));
        buff = "";
    }

    int colNum = 0;
    record rec;

    while(isspace(c))
      is>>std::noskipws>>c;
    buff += c;

    while(is>>std::noskipws>>c){
      if(c == '\r' || c == '\n'){
        rec[schema.at(colNum++)] = buff;
        rows.push_back(rec);
        rec = record();
        buff = "";
        colNum = 0;
      }
      else if(c==','){
        rec[schema.at(colNum++)] = buff;
        buff = "";
      }
      else if(!isspace(c) || buff != "")
        buff += c;
    }

    if(buff != ""){
    	        rec[schema.at(colNum++)] = buff;
    	        rows.push_back(rec);
    }
  }

  void JSON_dump(){
  	std::cout<<'[';
    auto row = begin(rows);
    print_JSON_row(*row);
  	while(++row != end(rows)){
      std::cout<<','<<std::endl;
      print_JSON_row(*row);
  	}
  	std::cout<<std::endl<<']'<<std::endl;
  } 

  private:
  bool is_bool_or_number(const std::string& s)
  {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end()
        || s == "true" || s == "false";
  }  

  std::string trim_end(std::string& s){
  	size_t l = s.size();
  	while(l > 0 && isspace(s[l-1]));
  	s.resize(l);
  	return s;
  }

  void print_JSON_row(const record& rec){
    std::cout<<"\t{"<<std::endl;
    auto p = begin(rec);
      std::cout<<"\t\t"<<p->first<<":";
  	  if(is_bool_or_number(p->second))
  		std::cout<<p->second;
      else
      	std::cout<<'"'<<p->second<<'"';
    while(++p != end(rec)){
      std::cout<<","<<std::endl;
  	  std::cout<<"\t\t"<<p->first<<":";
  	  if(is_bool_or_number(p->second))
  		std::cout<<p->second;
      else
      	std::cout<<'"'<<p->second<<'"';
    }
    std::cout<<std::endl<<"\t}";
  }  
};