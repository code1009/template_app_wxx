#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace bl
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class stopwatch
{
public:
	std::chrono::system_clock::time_point _start{};
	std::chrono::system_clock::time_point _stop{};
	std::chrono::microseconds _duration{};
	std::string _name;

public:
	explicit stopwatch(std::string name) :
		_name{ name }
	{
	}

	~stopwatch()
	{
	}

	void start(void)
	{
		_start = std::chrono::system_clock::now();
	}

	void stop(void)
	{
		_stop = std::chrono::system_clock::now();
	}

	void measure(void)
	{
		_duration = std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start);
	}

	void print(void)
	{
		std::ostringstream _oss;


		_oss
			<< "["
			<< _name
			<< "] "
			<< "duration: "
			<< _duration.count()
			<< "usec"
			<< std::endl;


		OutputDebugStringA(_oss.str().c_str());
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class scoped_time_measurer
{
private:
	stopwatch* _stopwatch;

public:
	explicit scoped_time_measurer(stopwatch* sw) :
		_stopwatch{ sw }
	{
		_stopwatch->start();
	}

	~scoped_time_measurer()
	{
		_stopwatch->stop();
		_stopwatch->measure();
		_stopwatch->print();
	}
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
