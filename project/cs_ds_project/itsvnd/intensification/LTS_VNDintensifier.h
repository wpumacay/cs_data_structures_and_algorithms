


#pragma once


#include "LBaseIntensifier.h"
#include "LVNDintensifier.h"
#include "LTSintensifier.h"
#include <iostream>

using namespace std;




namespace intensifiers
{




	class LTS_VNDintensifier : public LBaseIntensifier
	{

	private :

		LVNDintensifier* m_vnd_intensifier;
		LTSintensifier* m_ts_intensifier;

	public :

		LTS_VNDintensifier( optimizers::LBaseOptimizer* pOptimizer )
		{
			m_optimizer = pOptimizer;

			m_vnd_intensifier = new LVNDintensifier( pOptimizer );
			m_ts_intensifier = new LTSintensifier( pOptimizer );
		}

		~LTS_VNDintensifier()
		{
			m_optimizer = nullptr;

			if ( m_vnd_intensifier != nullptr )
			{
				delete m_vnd_intensifier;
				m_vnd_intensifier = nullptr;
			}

			if ( m_ts_intensifier != nullptr )
			{
				delete m_ts_intensifier;
				m_ts_intensifier = nullptr;
			}
		}

		void setTabuIterations( int pTabuIterations )
		{
			m_ts_intensifier->setTabuIterations( pTabuIterations );
		}


		void run( engine::LConfiguration* pConfiguration )
		{
			cout << "VND-Intensifier ----" << endl;
			m_vnd_intensifier->run( pConfiguration );
			cout << "TS-Intensifier ----" << endl;
			m_ts_intensifier->run( pConfiguration );
		}

	};









}