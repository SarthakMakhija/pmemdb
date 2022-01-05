package engine

import "bytes"

type node struct {
	key, value  []byte
	right, down *node
}

func (node *node) updateRight(right *node) *node {
	node.right = right
	return node
}

func (node *node) updateDown(down *node) *node {
	node.down = down
	return node
}

func (node *node) matchesKey(key []byte) bool {
	return bytes.Compare(node.key, key) == 0
}

func (node *node) isKeyLessEqualTo(key []byte) bool {
	return bytes.Compare(node.key, key) <= 0
}

func addNewNode(key, value []byte, left *node) *node {
	node := &node{key: key, value: value}
	node.updateRight(left.right)
	left.updateRight(node)
	return node
}
