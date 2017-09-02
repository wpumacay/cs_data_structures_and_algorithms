
#pragma once

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#define MAX_ITERATIONS 10

class LLogger
{

	private :

	string m_filePath;
	ofstream m_fileHandle;

	int m_iterations;

	public :

	bool isWorking;

	LLogger()
	{
		isWorking = false;
	}

	void reset( string logCase, int n )
	{
		m_filePath = "./log_";
		m_filePath += logCase;
		m_filePath += "_";
		m_filePath += to_string( n );
		m_filePath += ".txt";

		m_iterations = 0;
		isWorking = false;		
	}

	void onStartLog()
	{
		m_fileHandle.open( m_filePath.c_str() );
		isWorking = true;
	}

	void log( float radius )
	{
		m_iterations++;

		m_fileHandle << "R: " << to_string( radius ) << " \n";

		if ( m_iterations == MAX_ITERATIONS )
		{
			onEndLog();
		}
	}

	void onEndLog()
	{
		m_fileHandle.close();
		isWorking = false;
	}

};