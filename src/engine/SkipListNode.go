package engine

import "bytes"

type skipListNode struct {
	key, value  []byte
	right, down *skipListNode
}

func newNode(key, value []byte) *skipListNode {
	return &skipListNode{key: key, value: value}
}

func (node *skipListNode) matchesKey(key []byte) bool {
	return bytes.Equal(node.key, key)
}

func (node *skipListNode) isKeyLessEqualTo(key []byte) bool {
	return bytes.Compare(node.key, key) <= 0
}

func (node *skipListNode) addToRightWith(key, value []byte) *skipListNode {
	newNode := newNode(key, value)
	newNode.updateRight(node.right)
	node.updateRight(newNode)
	return newNode
}

func (node *skipListNode) updateDown(down *skipListNode) *skipListNode {
	node.down = down
	return node
}

func (node *skipListNode) updateRight(right *skipListNode) *skipListNode {
	node.right = right
	return node
}

func (node *skipListNode) updateValue(value []byte) {
	node.value = value
}
