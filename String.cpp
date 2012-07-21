#include "String.h"
#include <cctype>
#include <cstring>


int String::total_allocation = 0;
int String::number = 0;

const char String::NULL_CHAR = '\0';
const char String::SPACE = ' ';

String::~String(){
    total_allocation -= mem_alloc;
    --number;
    if(mem_alloc > 0) delete [] cstr;
}

void String::swap(String& other){

    char* cstr_holder = other.cstr;
    int len_holder = other.len,
    mem_alloc_holder = other.mem_alloc;

    other.len = len;
    other.cstr = cstr;
    other.mem_alloc = mem_alloc;

    cstr = cstr_holder;
    len = len_holder;
    mem_alloc = mem_alloc_holder;
}

void String::copy_internal(const char* in_cstr){
    mem_alloc = len + NULL_ALLOC;
    cstr = new char[mem_alloc];

    for(int i = 0; i < len; ++i){ cstr[i] = in_cstr[i]; }

    cstr[len] = '\0';
    total_allocation += mem_alloc;
    ++number;
}

String& String::operator=(const String& rhs){
    String temp(rhs);
    swap(temp);
    return *this;
}

String& String::operator=(const char* rhs){
    String temp(rhs);
    swap(temp);
    return *this;

}

char& String::operator[](int i){
    return get_char_at(i);
}

const char& String::operator[](int i)const{
    return get_char_at(i);
}

String String::substring(int i, int nlen) const {
    if(nlen == 0) {
        String ret("");
        return ret;
    }
    if(i >= 0 && nlen > 0 && i < len && (i + nlen) < len){
        char* temp = new char[len+1];
        for(int k = 0; k < len; ++k, ++i){
            temp[k] = cstr[k];
        }

        temp[len] = NULL_CHAR;
        String ret(temp);
        delete temp;
        return ret;
    } else {
        throw String_exception("Invalid parameters in String::substring(..).\n");
    }
}

void String::remove(int i, int nlen){
    if(i >= 0 && nlen > 0 && i < len && (i + nlen) < len){
        for(int j = 0; j < len; ++j, ++i){
            if(i+nlen < len) cstr[i] = cstr[i+nlen];

        }
        len -= nlen;
        cstr[len] = '\0';
    } else {
        throw String_exception("Invalid parameters in String::substring(..).\n");
    }
}


void String::insert_before(int i, const String& src){
    if (i < 0 || i > len) throw String_exception("Invalid parameters in String::insert_before(..).\n");

    char* new_cstr = prepare_cstr_assign(src.len);
    int new_length = len + src.len;

    int ins = 0;
    for(int before = 0; before < i; ++before, ++ins){
        new_cstr[ins] = cstr[before];
    }

    for(const char* src_it = src.cstr; *src_it || *src_it != '\0'; ++src_it, ++ins){
        new_cstr[ins] = *src_it;
    }

    for(const char* this_it = &cstr[i]; *this_it || *this_it !='\0'; ++this_it, ++ins){
        new_cstr[ins] = *this_it;
    }

    new_cstr[new_length] = NULL_CHAR;

    delete [] cstr;

    cstr = new_cstr;
    len = new_length;
}

String& String::operator+=(char rhs){

    char* rhsp = &rhs;
    append(rhsp, SINGLE_CHAR);
    return *this;
}

String& String::operator+=(const char* rhs){
    append(rhs, find_len(rhs));
    return *this;
}

String& String::operator+=(const String& rhs){
    append(rhs.cstr, rhs.len);
    return *this;
}

void String::clear(){
    String empty("");
    swap(empty);
}

/*************************************/
/** Non-member overloaded functions **/


bool operator==(const String& lhs, const String& rhs){
    return(!std::strcmp(lhs.c_str(), rhs.c_str()));
}

bool operator !=(const String & lhs, const String& rhs){
    return(std::strcmp(lhs.c_str(), rhs.c_str()));
}

bool operator< (const String& lhs, const String& rhs){
    return(std::strcmp(lhs.c_str(), rhs.c_str()) < 0);
}

bool operator> (const String& lhs, const String& rhs){
    return(std::strcmp(lhs.c_str(), rhs.c_str()) > 0);
}

String operator+ (const String& lhs, const String& rhs){
    int new_alloc = lhs.size() + rhs.size() + 1;
    char* conc = new char[new_alloc];
    for(int i = 0; i < lhs.size(); ++i){
        conc[i] = lhs.c_str()[i];
    }

    for(int i = 0, j = lhs.size(); i < rhs.size(); ++i, ++j){
        conc[j] = rhs.c_str()[i];
    }
    conc[new_alloc-1] = String::NULL_CHAR;
    String ret(conc);
    delete conc;
    return ret;
}

/*Input/Output overloaded operators*/
std::ostream& operator<< (std::ostream& os, const String& str){
    os << str.c_str();
    return os;

}

std::istream& operator>> (std::istream& is, String& str){
    str.clear();
    char* cstr = get_input(false, is);
    str = cstr;
    return is;
}

char* get_input(bool getline, std::istream& is){
    int size(String::SINGLE_CHAR + String::NULL_ALLOC);
    char in;
    char* cstr = "";
    is >> in;
    if(is.good()){
        cstr = new char[size];
        cstr[0] = in;
        cstr[String::NULL_ALLOC] = String::NULL_CHAR;
    } else {
        is.clear();
        return cstr;
    }

    while(is.peek() != '\n'){
        if(!getline && isspace(is.peek())) break;
        in = is.get();
        if(is.fail()) break;
        ++size;
        char* prior = cstr;
        cstr = new char[size];
        for(int i = 0; i < size-2; ++i){
            cstr[i] = prior[i];
        }
        cstr[size-2] = in;
        cstr[size-1] = String::NULL_CHAR;
        delete prior;
    }

    return cstr;

}

std::istream& getline(std::istream& is, String& str){
    str.clear();
    char* cstr = get_input(true, is);
    str = cstr;
    return is;

}



/*******************************/
/** Private Utility Functions **/

int String::get_new_alloc(int n)  {
    int ret;
    if(mem_alloc < len + n + NULL_ALLOC){
        ret = 2 * (len + n + NULL_ALLOC);
        total_allocation = total_allocation - mem_alloc + ret;
        doubled = true;
    } else {
        ret = mem_alloc;
    }

    return ret;

}

char* String::make_new_cstr(int new_alloc){
    char* temp;
    if(new_alloc != mem_alloc){
        temp = new char[new_alloc];
        mem_alloc = new_alloc;
    } else {
        temp = cstr;
    }

    return temp;
}

char* String::prepare_cstr_assign(int n) {
    char* ret = make_new_cstr(get_new_alloc(n));
    return ret;
}

char& String::get_char_at(int i)const{
    if(0 > i || i >= len) throw String_exception("Out of String bounds");

    return cstr[i];
}

void String::append(const char* app, int nlen){

    char* new_cstr = prepare_cstr_assign(nlen);

    for(int i = 0; i < len; ++i){
        new_cstr[i] = cstr[i];
    }

    for(int j(len), k(0); k < nlen; ++j, ++k){
        new_cstr[j] = app[k];
    }

    len += nlen;
    new_cstr[len] = '\0';

    if(doubled){
        delete [] cstr;
        cstr = new_cstr;
        doubled = false;
    }

}

int String::find_len(const char* cstr) const{
    int nlen = 0;
    for(const char* it = cstr; *it || *it != '\0'; ++it, ++nlen){}
    return nlen;
}
