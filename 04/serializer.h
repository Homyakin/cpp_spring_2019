#pragma once

#include <iostream>


enum class Error 
{
	NoError,
	CorruptedArchive
};

class Serializer
{
public:
	explicit Serializer(std::ostream& out) : out_(out) {}
	
	template <class T>
	Error save(T& object) { return object.serialize(*this); }

	template <class... Args>
	Error operator()(Args&&... args) { return process(std::forward<Args>(args)...); }

	
private:
	std::ostream& out_;
	static constexpr char SEPARATOR = ' ';

	template<class T, class... Args>
	Error process(T&& value, Args&& ... args) 
	{
		return ((process(std::forward<T>(value)) == Error::NoError) ?
			process(std::forward<Args>(args)...) :
			Error::CorruptedArchive);
	}
	
	Error process(bool value)
	{
		out_ << (value ? "true" : "false") << SEPARATOR;
		return Error::NoError;
	}

	Error process(uint64_t value)
	{
		out_ << value << SEPARATOR;
		return Error::NoError;
	}
};

class Deserializer 
{
public:
	Deserializer(std::istream& in) : in_(in) {}
	
	template <class T>
	Error load(T& object) { return object.serialize(*this); }

	template <class... Args>
	Error operator() (Args&&... args) { return process(std::forward<Args>(args)...); }

private:
	std::istream& in_;	
	static constexpr char SEPARATOR = ' ';
	
	template<class T, class... Args>
	Error process(T&& value, Args&&... args) 
	{
		if (process(std::forward<T>(value)) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		else
			return process(std::forward<Args>(args)...);
	}
	
	Error process(bool& value) 
	{
		std::string in;
		in_ >> in;
		if(!in.compare("true"))
			value = true;
		else if(!in.compare("false"))
			value = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}

	Error process(uint64_t& value) 
	{
		std::string in;
		in_ >> in;
		value = 0;
		for(auto i : in)
		{
			if(i >= '0' && i <= '9') 
			{
				value *= 10;
				value += i - '0';
			} 
			else
				return Error::CorruptedArchive;
		}
		return Error::NoError;
	}
};
