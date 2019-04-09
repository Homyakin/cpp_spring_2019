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
	Error process(T&& valueue, Args&& ... args) 
	{
		return ((process(std::forward<T>(valueue)) == Error::NoError) ?
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
	Error process(T&& valueue, Args&&... args) 
	{
		if (process(std::forward<T>(valueue)) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		else
			return process(std::forward<Args>(args)...);
	}
	
	Error process(bool& valueue) 
	{
		std::string in;
		in_ >> in;
		if(!in.compare("true"))
			valueue = true;
		else if(!in.compare("false"))
			valueue = false;
		else
			return Error::CorruptedArchive;
		return Error::NoError;
	}

	Error process(uint64_t& valueue) 
	{
		std::string in;
		in_ >> in;
		valueue = 0;
		for(auto i : in)
		{
			if(i >= '0' && i <= '9') 
			{
				valueue *= 10;
				valueue += i - '0';
			} 
			else
				return Error::CorruptedArchive;
		}
		return Error::NoError;
	}
};
