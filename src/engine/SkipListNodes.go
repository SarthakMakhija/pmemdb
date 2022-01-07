package engine

type nodes struct {
	nodes []*skipListNode
}

func (nodes *nodes) add(n *skipListNode) {
	nodes.nodes = append(nodes.nodes, n)
}

func (nodes *nodes) pop() *skipListNode {
	n := nodes.nodes[len(nodes.nodes)-1]
	nodes.nodes = nodes.nodes[0 : len(nodes.nodes)-1]
	return n
}

func (nodes *nodes) isEmpty() bool {
	return len(nodes.nodes) == 0
}
