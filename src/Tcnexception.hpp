#ifndef _TCNEXCEPTION_HPP_
#define _TCNEXCEPTION_HPP_

/*!
 * \file Tcnexception.hpp
 * \brief описание класса исключения, используемого в этом проекте
 */

/*!
 * \brief Класс для генерации исключений
 *
 * \full В настоящее время сложной иерархии исключений нет, но, возможно, она появится в будущем
 */
class Tcnexception : public std::exception
{
public:
	std::string s;
	Tcnexception(std::string ss) : s(ss){}
	~Tcnexception() throw() {}
	const char * what() const throw() {return s.c_str();}
};

#endif
