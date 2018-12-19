#include "MapNode.h"



MapNode::MapNode()
{
	m_isLeaf = false;
	m_NodeDepth = 0;
	
	m_Box.Center = Vector3::Zero;
	m_Box.Max	 = Vector3::One;
	m_Box.Min	 = -Vector3::One;
}


MapNode::~MapNode()
{

}
