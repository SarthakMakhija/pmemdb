package engine

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestShouldMatchKeyInSkipListNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	matches := node.matchesKey([]byte("HDD"))

	assert.Truef(t, matches, "Expected key %v to match", "HDD")
}

func TestShouldNotMatchKeyInSkipListNode(t *testing.T) {
	node := newNode([]byte("SDD"), []byte("Hard disk drive"))
	matches := node.matchesKey([]byte("HDD"))

	assert.Falsef(t, matches, "Expected key %v to not match", "HDD")
}

func TestShouldReturnTrueGivenKeyIsEqualToNodesKey(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	isLessOrEqualTo := node.isKeyLessEqualTo([]byte("HDD"))

	assert.Truef(t, isLessOrEqualTo, "Expected key %v to be equal to node's key", "HDD")
}

func TestShouldReturnTrueGivenNodeKeyIsLessThanGivenKey(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	isLessOrEqualTo := node.isKeyLessEqualTo([]byte("SDD"))

	assert.Truef(t, isLessOrEqualTo, "Expected node's key to be less than given key %v", "HDD")
}

func TestShouldAddANewNodeToTheRight(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	newNode := node.addToRightWith([]byte("SDD"), []byte("Solid state drive"))

	assert.Equalf(t, newNode, node.right, "Expected node's right to be %v, received %v", newNode, node.right)
}

func TestShouldUpdateDownPointerInTheNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	down := newNode([]byte("SDD"), []byte("Solid state drive"))
	node.updateDown(down)

	assert.Equalf(t, down, node.down, "Expected node's down to be %v, received %v", down, node.down)
}

func TestShouldUpdateRightPointerInTheNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	right := newNode([]byte("SDD"), []byte("Solid state drive"))
	node.updateRight(right)

	assert.Equalf(t, right, node.right, "Expected node's right to be %v, received %v", right, node.right)
}
