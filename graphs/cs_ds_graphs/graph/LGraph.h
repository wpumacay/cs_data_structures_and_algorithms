
#pragma once

#include <iostream>
#include <vector>
#include <list>

#include "LNode.h"
#include "LEdge.h"

using namespace std;

namespace DS
{
	template<class N, class E>
	class LGraph
	{

		public :

		typedef LGraph<N,E> self;
		typedef LNode<self> Node;
		typedef LEdge<self> Edge;

		typedef N NodeData;
		typedef E EdgeData;

		vector<Node*> nodes;

        void insertNode( N node_data, double x, double y );
		void insertEdge( Node* from, Node* to, E edge_data );
		void removeEdge( Edge* edge );
		void removeNode( Node* node );


		void print();
	};

	template<class N, class E>
    void LGraph<N,E>::insertNode( N node_data, double x, double y )
	{
        nodes.push_back( new LNode<LGraph<N,E> >( node_data, x, y ) );
	}

	template<class N, class E>
	void LGraph<N,E>::insertEdge( LNode<LGraph<N,E> >* from,
								  LNode<LGraph<N,E> >* to,
								  E edge_data )
	{
		LEdge<LGraph<N,E> >* _edge_from = new LEdge<LGraph<N,E> >( edge_data );

		_edge_from->nodes[0] = from;
		_edge_from->nodes[1] = to;
		from->edges.push_back( _edge_from );

		LEdge<LGraph<N,E> >* _edge_to = new LEdge<LGraph<N,E> >( edge_data );
		_edge_to->nodes[0] = to;
		_edge_to->nodes[1] = from;
		to->edges.push_back( _edge_to );
	}

	template<class N, class E>
	void LGraph<N,E>::removeEdge( LEdge<LGraph<N,E> >* edge )
	{
		// Remove from the "from" node's edges
		LNode<LGraph<N,E> >* _node_from = edge->nodes[0];
		LNode<LGraph<N,E> >* _node_to = edge->nodes[1];
		
		int _indx = -1;
		for ( int q = 0; q < _node_from->edges.size(); q++ )
		{
			if ( _node_from->edges[q]->nodes[1] == _node_to )
			{
				_indx = q;
				break;
			}
		}
		
		if ( _indx != -1 )
		{
			_node_from->edges.erase( _node_from->edges.begin() + _indx );
		}

		// Remove from the "to" node's edges
		
		_indx = -1;
		for ( int q = 0; q < _node_to->edges.size(); q++ )
		{
			if ( _node_to->edges[q]->nodes[1] == _node_from )
			{
				_indx = q;
				break;
			}
		}
		
		if ( _indx != -1 )
		{
			_node_to->edges.erase( _node_to->edges.begin() + _indx );
		}

        delete edge;
	}

	template<class N, class E>
	void LGraph<N,E>::removeNode( LNode<LGraph<N,E> >* node )
	{
		// Remove edges from the "from" node and, if bidir, from the "to" this node
		vector<Edge*> _edges = node->edges;
		// TODO: Check issue here
		for ( int q = 0; q < _edges.size(); q++ )
		{
			removeEdge( _edges[q] );
		}

		// Remove the node
		int _indx = -1;
		for ( int q = 0; q < nodes.size(); q++ )
		{
			if ( nodes[q] == node )
			{
				_indx = q;
				break;
			}
		}

		if ( _indx != -1 )
		{
			nodes.erase( nodes.begin() + _indx );
		}
	}


	template<class N, class E>
	void LGraph<N,E>::print()
	{
		cout << "graph: " << endl;
		for ( int q = 0; q < nodes.size(); q++ )
		{
			cout << " node ( " << nodes[q]->data << " ): ";
			vector<LEdge<LGraph<N,E> >* > _node_edges = nodes[q]->edges;
			
			for ( int p = 0; p < _node_edges.size(); p++ )
			{
				cout << ( _node_edges[p]->nodes[1] )->data << "(" << _node_edges[p]->data << ") ";
			}
			cout << endl;
		}
		cout << "********" << endl;
	}
}
