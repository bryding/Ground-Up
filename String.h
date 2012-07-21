#ifndef STRING_H
#define STRING_H

#include <iostream>

struct String_exception {
	String_exception(const char * in_msg) : msg(in_msg)
		{}
	const char * msg;
};


class String {
public:
	

    String(const char * in_cstr = "") : len(0), mem_alloc(0), doubled(false) {
        if(*in_cstr || *in_cstr != '\0'){
            for(const char* it = in_cstr; *it || *it != '\0'; ++it, ++len){}
            copy_internal(in_cstr);
        }
    }

   ~String();

    String(const String& str) : len(str.len), mem_alloc(0), doubled(false)
    {copy_internal(str.cstr);}


    String& operator=(const String& rhs);
    String& operator=(const char* rhs);


	
	const char * c_str() const
		{return cstr;}

	int size() const
		{return len;}

	int get_allocation() const
		{return mem_alloc;}


	char& operator[] (int i);
	const char& operator[] (int i) const;	// const version for const Strings

	
	String substring(int i, int nlen) const;

	//Modifiers
	//Set to an empty string with minimum allocation by create/swap with an empty string.
	void clear();


	void remove(int i, int nlen);

        void insert_before(int i, const String& src);

	

        String& operator += (char rhs);
	String& operator += (const char * rhs);
	String& operator += (const String& rhs);

	
	void swap(String& other);


	static int get_number()
		{return number;}
	
	static int get_total_allocation()
		{return total_allocation;}

    /* Constants used by this class */
    const static int SINGLE_CHAR = 1;
    const static int NULL_ALLOC = 1;
    const static char NULL_CHAR;
    const static char SPACE;

private:

	char* cstr;
	int len;
	int mem_alloc;
	bool doubled;

	/* Variables for monitoring functions - not part of a normal implementation. */
	static int number;				// counts number of String objects in existence
	static int total_allocation;	// counts total amount of memory allocated


    /* Private Utility Functions */
    void copy_internal(const char* in_cstr);
    char& get_char_at(int i) const;
    int get_new_alloc(int n);
    char* make_new_cstr(int new_alloc);
    char* prepare_cstr_assign(int n);
    int find_len(const char* cstr) const;
    void append(const char* app, int nlen);


};

// non-member overloaded operators

bool operator== (const String& lhs, const String& rhs);
bool operator!= (const String& lhs, const String& rhs);
bool operator< (const String& lhs, const String& rhs);
bool operator> (const String& lhs, const String& rhs);


String operator+ (const String& lhs, const String& rhs);

std::ostream& operator<< (std::ostream& os, const String& str);


std::istream& operator>> (std::istream& is, String& str);

std::istream& getline(std::istream& is, String& str);

char* get_input(bool getline, std::istream& is);

#endif
