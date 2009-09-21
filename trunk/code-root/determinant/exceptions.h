#ifndef __exceptions_h__
#define __exceptions_h__

#include <string>
#include <stdexcept>


/**
 * ������������ ����� ��� ��������� ����������
 */
class Exception {
	std::string m_what;
	int m_code;
public:
	Exception() : m_what(), m_code(0) { }
	Exception(const std::string & what) : m_what(what), m_code(0) { }
	Exception(const std::string & what, int code) : m_what(what), m_code(code) { }

	virtual ~Exception() { }

	const std::string & what() const {
		return m_what;
	}
	
	const int code() const {
		return m_code;
	}
};


/**
 * ����� ����������� �������������� �������� ��������� � ��������� ���������� �������
 */
class IllegalStateException : public Exception {
public:
	IllegalStateException() { }
	IllegalStateException(const std::string & what) : Exception(what) { }
	IllegalStateException(const std::string & what, int code) : Exception(what, code) { }
};


/**
 * ����� ����������� �������������� �������� ��������� � �������� �� ������� ������
 */
class IllegalArgumentException : public Exception {
public:
	IllegalArgumentException() { }
	IllegalArgumentException(const std::string & what) : Exception(what) { }
	IllegalArgumentException(const std::string & what, int code) : Exception(what, code) { }
};


/**
 * ����� ����������� �������������� �������� ��������� � ������� �����/������
 */
class IOException : public Exception {
public:
	IOException() { }
	IOException(const std::string & what) : Exception(what) { }
	IOException(const std::string what, int code) : Exception(what, code) { }
};


/**
 * ����� ����������� �������������� �������� ��������� � ����������� ������ �����
 */
class EOFException : public IOException {
public:
	EOFException() {}
	EOFException(const std::string & what) : IOException(what) { }
	EOFException(const std::string & what, int code) : IOException(what, code) { }
};


#endif //_exceptions_h__
