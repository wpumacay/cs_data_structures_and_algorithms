

#pragma once


#include <iostream>
#include <vector>

#include "../engine/LSolver.h"

using namespace std;


class LConsoleMenu
{

private :


	int m_optimizerId;
	int m_intensifierId;


	int m_instanceSize;
	int m_instanceId;


public :


	LConsoleMenu()
	{
		m_optimizerId = 0;
		m_intensifierId = 0;

		m_instanceSize = 26;
		m_instanceId = 0;
	}

	void showMainMenu()
	{
		cout << "Welcome to my test-app *****************" << endl;
		cout << "select an option: " << endl;

		cout << "1) Run a test." << endl;
		//cout << "2) Show instances bests." << endl;
		//cout << "3) About." << endl;
		//cout << "4) Help." << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 1, 1, _option ) );

		cout << "..." << endl;

		switch ( _option )
		{

			case 1 : 

			showTestMenu();

			break;

			default :

			showTestMenu();

			break;

		}
	}



	void showTestMenu()
	{
		cout << "****************************************" << endl;
		cout << "Select Options: " << endl;

		cout << "1) Optimizer-> ";
		switch ( m_optimizerId )
		{
			case 0 : cout << "GradientDescent based optimizer" << endl; break;
			case 1 : cout << "BFGS based optimizer" << endl; break;
		}

		cout << "2) Intensifier-> ";
		switch ( m_intensifierId )
		{
			case 0 : cout << "Variable Neighborhood Descent (VND)" << endl; break;
			case 1 : cout << "Tabu Search (TS)" << endl; break;
			case 2 : cout << "TS-VND" << endl; break;
		}

		cout << "3) Instance size-> " << m_instanceSize << endl;;

		cout << "4) Instance Id-> ";
		switch ( m_instanceId )
		{
			case 0 : cout << "r = i" << endl; break;
			case 1 : cout << "r = sqrt(i)" << endl; break;
			case 2 : cout << "r = N/sqrt(i)" << endl; break;
		}

		cout << "5) TEST!" << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 1, 5, _option ) );
		

		switch ( _option )
		{

			case 1 : showMenuSelectOptimizer(); break;
			case 2 : showMenuSelectIntensifier(); break;
			case 3 : showMenuSelectInstanceSize(); break;
			case 4 : showMenuSelectInstanceId(); break;
			case 5 : testSolver(); break;
		}
	}


	void showAbout()
	{
		showMainMenu();
	}

	bool askEnterValid( int pMin, int pMax, int pValue )
	{
		cout << endl;
		if ( pValue < pMin || pValue > pMax )
		{
			cout << "Please, enter in valid range :'(" << endl;
			return true;
		}
		return false;
	}


	void showMenuSelectOptimizer()
	{
		cout << "****************************************" << endl;

		cout << "Optimizer:" << endl;
		cout << "1) GradientDescent based optimizer" << endl;
		cout << "2) BFGS based optimizer" << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 1, 2, _option ) );

		m_optimizerId = _option - 1;

		showTestMenu();
	}

	void showMenuSelectIntensifier()
	{
		cout << "****************************************" << endl;

		cout << "Intensifier:" << endl;
		cout << "1) Variable Neighborhood Descent (VND)" << endl;
		cout << "2) Tabu Search (TS)" << endl;
		cout << "3) TS-VND" << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 1, 3, _option ) );

		m_intensifierId = _option - 1;

		showTestMenu();
	}

	void showMenuSelectInstanceSize()
	{
		cout << "****************************************" << endl;

		cout << "Instance size ( 26-50 ):" << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 26, 50, _option ) );

		m_instanceSize = _option;

		showTestMenu();
	}

	void showMenuSelectInstanceId()
	{
		cout << "****************************************" << endl;

		cout << "Instance Id:" << endl;
		cout << "1) r = i" << endl;
		cout << "2) r = sqrt(i)" << endl;
		cout << "3) r = N/sqrt(i)" << endl;

		cout << "****************************************" << endl;

		int _option;
		do
		{
			cout << "Opt: ";
			cin >> _option;

		} while( askEnterValid( 1, 3, _option ) );

		m_instanceId = _option - 1;

		showTestMenu();
	}

	void testSolver()
	{
		engine::LSolver::instance->reset( ( engine::options::optimizer::_optimizer ) m_optimizerId,
										  ( engine::options::intensifier::_intensifier ) m_intensifierId );
		engine::LSolver::instance->init( ( engine::circleInstance::_circleInstance ) m_instanceId,
										 m_instanceSize );
		engine::LSolver::instance->solve();

		showTestMenu();
	}
};

