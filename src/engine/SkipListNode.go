package engine

import "bytes"

type skipListNode struct {
	key, value  []byte
	right, down *skipListNode
}

func (node *skipListNode) updateRight(right *skipListNode) *skipListNode {
	node.right = right
	return node
}

func (node *skipListNode) updateDown(down *skipListNode) *skipListNode {
	node.down = down
	return node
}

func (node *skipListNode) matchesKey(key []byte) bool {
	return bytes.Equal(node.key, key)
}

func (node *skipListNode) isKeyLessEqualTo(key []byte) bool {
	return bytes.Compare(node.key, key) <= 0
}

func (node *skipListNode) addWith(key, value []byte) *skipListNode {
	newNode := &skipListNode{key: key, value: value}
	newNode.updateRight(node.right)
	node.updateRight(newNode)
	return newNode
}
