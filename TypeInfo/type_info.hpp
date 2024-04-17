#ifndef TYPE_INFO
#define TYPE_INFO

#include <iostream>
#include <print>
#include <format>
#include <iomanip>    
#include <vector>
#include <cmath>      
#include <functional> 
#include <string>

#define disp(expr)  std::cerr << __FILE__ << ":" << __LINE__ << ":" \
	<< " ; " <<  #expr << " = " << (expr)  <<  "\n"

template<class T>
struct isPointer{
	static constexpr bool value = false;
	constexpr bool operator()() const { return false; }
};

template<class T>
struct isPointer<T*>{
	static constexpr bool value = true;
	constexpr bool operator()() const { return true; }
};

template<class T> struct isConst{
	static constexpr bool value = false;
};
template<class T> struct isConst<const T>{
	static constexpr bool value = true;
}; 
template<class T> struct isConst<const T*>{
	static constexpr bool value = true;
};
template<class T> struct isConst<const T&>{
	static constexpr bool value = true;
};

template<class T> struct removePointer{
	typedef T type;
};

template<class T> struct removePointer<T*>{
	typedef T type;
};

template<typename T>
struct Typeinfo{

	static constexpr const char* name		= "unknown";
	static constexpr size_t      size		= sizeof(T);
	static constexpr bool        isNumber	= false;
	static constexpr bool        isPointer	= ::isPointer<T>::value;
	static constexpr bool        isConst	= ::isConst<T>::value;		
};

#define REGISTER_TYPE_INFO(type, isNumberFlag) \
	template<> struct Typeinfo<type>{ \
		static constexpr const char* name		= #type;		\
		static constexpr size_t      size		= sizeof(type); \
		static constexpr bool        isNumber	= isNumberFlag;	\
		static constexpr bool        isPointer	= ::isPointer<type>::value; \
		static constexpr bool        isConst	= ::isConst<type>::value;	\
	}

template<> struct Typeinfo<int>{
	static constexpr const char* name		= "int";
	static constexpr size_t      size		= sizeof(int);
	static constexpr bool        isNumber	= true;
	static constexpr bool        isPointer	= ::isPointer<int>::value;
	static constexpr bool        isConst	= ::isConst<int>::value;
};

REGISTER_TYPE_INFO(bool,				false);
REGISTER_TYPE_INFO(char,				false);
REGISTER_TYPE_INFO(const char*,			true);
REGISTER_TYPE_INFO(const char,			false);
REGISTER_TYPE_INFO(short,				true);
REGISTER_TYPE_INFO(short*,				false);
REGISTER_TYPE_INFO(float,				true);
REGISTER_TYPE_INFO(float*,				false);
REGISTER_TYPE_INFO(const float&,		false);
REGISTER_TYPE_INFO(double,				true);
REGISTER_TYPE_INFO(double*,				false);
REGISTER_TYPE_INFO(const double&,		false);
REGISTER_TYPE_INFO(long,				true);
REGISTER_TYPE_INFO(std::string,			false);
REGISTER_TYPE_INFO(const std::string&,	false);

template<typename T>
void showTypeInfo(){
    std::print("{:>5}", "Type Info: name = ");
    std::print("{:>15}",  Typeinfo<T>::name);
    std::print("{:>4}",   Typeinfo<T>::size);
    std::print("{:>5}", " ; isNumber = ");
    std::print("{:>5}",   Typeinfo<T>::isNumber);
    std::print("{:>5}", " ; isPointer = ");
    std::print("{:>5}",  Typeinfo<T>::isPointer);
    std::print("{:>5}", " ; isConst = ");
    std::print("{:>5}",  Typeinfo<T>::isConst);
}

#endif