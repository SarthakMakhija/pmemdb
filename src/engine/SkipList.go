package engine

import (
	"github.com/SarthakMakhija/pmemdb/src/assert"
	"math/rand"
)

type SkipList struct {
	tower []*skipListNode
}

func NewSkipList(towerSize int) *SkipList {
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

	parents := &nodes{}
	targetNode := list.tower[len(list.tower)-1]
	for ; targetNode != nil; targetNode = targetNode.down {
		for targetNode.right != nil && targetNode.right.isKeyLessEqualTo(key) {
			targetNode = targetNode.right
		}
		parents.add(targetNode)
	}

	left := parents.pop()
	node := left.addWith(key, value)
	for rand.Intn(2) == 1 {
		if parents.isEmpty() {
			sentinelNode := list.increaseTowerSize()
			parents.add(sentinelNode)
		}
		left = parents.pop()
		newNode := left.addWith(key, value)
		newNode.updateDown(node)
		node = newNode
	}
}

func (list *SkipList) GetByKey(key []byte) ([]byte, bool) {
	targetNode := list.tower[len(list.tower)-1]
	for targetNode != nil {
		for targetNode.right != nil && targetNode.right.isKeyLessEqualTo(key) {
			targetNode = targetNode.right
		}
		if targetNode.matchesKey(key) {
			return targetNode.value, true
		}
		targetNode = targetNode.down
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
