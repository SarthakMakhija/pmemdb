package engine

import (
	"github.com/SarthakMakhija/pmemdb/src/assert"
	"math/rand"
)

type SkipList struct {
	tower []*skipListNode
}

func NewSkipList(towerSize int) *SkipList {
	assert.Assert(towerSize > 0, "initial towerSize must be greater than zero")

	initializeWithSentinelNodesOf := func(list *SkipList) {
		for index := 0; index < towerSize; index++ {
			sentinelNode := &skipListNode{}
			list.tower[index] = sentinelNode
			if index > 0 {
				sentinelNode.down = list.tower[index-1]
			}
		}
	}
	list := &SkipList{tower: make([]*skipListNode, towerSize)}
	initializeWithSentinelNodesOf(list)
	return list
}

func (list *SkipList) Put(key, value []byte) {
	assert.Assert(key != nil, "Key can not be nil")
	assert.Assert(value != nil, "Key can not be nil")

	targetNode, ok := list.getByKey(key)
	if !ok {
		list.put(key, value)
	} else {
		list.update(key, value, targetNode)
	}
}

func (list *SkipList) put(key []byte, value []byte) {
	targetNode := list.tower[len(list.tower)-1]

	collectNodesAtLevels := func() *nodes {
		parents := &nodes{}
		list.traverse(targetNode, key, func(key []byte, node *skipListNode) traversalStatus {
			parents.add(node)
			return traversalStatus{node: node, shouldContinue: true}
		})
		return parents
	}
	multilevelPut := func(parents *nodes) {
		left := parents.pop()
		node := left.addToRightWith(key, value)
		for rand.Intn(2) == 1 {
			if parents.isEmpty() {
				sentinelNode := list.increaseTowerSize()
				parents.add(sentinelNode)
			}
			left = parents.pop()
			newNode := left.addToRightWith(key, value)
			newNode.updateDown(node)
			node = newNode
		}
	}
	multilevelPut(collectNodesAtLevels())
}

func (list *SkipList) update(key []byte, value []byte, startingNode *skipListNode) {
	targetNode := startingNode

	list.traverse(targetNode, key, func(key []byte, node *skipListNode) traversalStatus {
		if node.matchesKey(key) {
			node.updateValue(value)
		}
		return traversalStatus{node: node, shouldContinue: true}
	})
}

func (list *SkipList) GetByKey(key []byte) ([]byte, bool) {
	targetNode, ok := list.getByKey(key)
	if ok {
		return targetNode.value, true
	}
	return nil, false
}

func (list *SkipList) getByKey(key []byte) (*skipListNode, bool) {
	targetNode := list.tower[len(list.tower)-1]
	node := list.traverse(targetNode, key, func(key []byte, node *skipListNode) traversalStatus {
		if node.matchesKey(key) {
			return traversalStatus{node: node, shouldContinue: false}
		} else {
			return traversalStatus{node: nil, shouldContinue: true}
		}
	})
	if node != nil {
		return node, true
	}
	return nil, false
}

func (list *SkipList) increaseTowerSize() *skipListNode {
	sentinelNode := &skipListNode{}
	list.tower = append(list.tower, sentinelNode)
	topIndex := len(list.tower) - 1
	list.tower[topIndex].down = list.tower[topIndex-1].down
	return sentinelNode
}

func (list SkipList) traverse(startingNode *skipListNode, key []byte, execute func(key []byte, node *skipListNode) traversalStatus) *skipListNode {
	targetNode := startingNode
	for ; targetNode != nil; targetNode = targetNode.down {
		for targetNode.right != nil && targetNode.right.isKeyLessEqualTo(key) {
			targetNode = targetNode.right
		}
		status := execute(key, targetNode)
		if !status.shouldContinue {
			return status.node
		}
	}
	return nil
}

type traversalStatus struct {
	node           *skipListNode
	shouldContinue bool
}
