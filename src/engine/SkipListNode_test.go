package engine

import "testing"

func TestShouldMatchKeyInSkipListNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	matches := node.matchesKey([]byte("HDD"))

	if matches != true {
		t.Fatalf("Expected key %v to match", "HDD")
	}
}

func TestShouldNotMatchKeyInSkipListNode(t *testing.T) {
	node := newNode([]byte("SDD"), []byte("Hard disk drive"))
	matches := node.matchesKey([]byte("HDD"))

	if matches != false {
		t.Fatalf("Expected key %v to not match", "HDD")
	}
}

func TestShouldReturnTrueGivenKeyIsEqualToNodesKey(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	isLessOrEqualTo := node.isKeyLessEqualTo([]byte("HDD"))

	if isLessOrEqualTo != true {
		t.Fatalf("Expected key %v to be equal to node's key", "HDD")
	}
}

func TestShouldReturnTrueGivenNodeKeyIsLessThanGivenKey(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	isLessOrEqualTo := node.isKeyLessEqualTo([]byte("SDD"))

	if isLessOrEqualTo != true {
		t.Fatalf("Expected node's key to be less than given key %v", "HDD")
	}
}

func TestShouldAddANewNodeToTheRight(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	newNode := node.addToRightWith([]byte("SDD"), []byte("Solid state drive"))

	if node.right != newNode {
		t.Fatalf("Expected node's right to be %v, received %v", newNode, node.right)
	}
}

func TestShouldUpdateDownPointerInTheNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	down := newNode([]byte("SDD"), []byte("Solid state drive"))
	node.updateDown(down)

	if node.down != down {
		t.Fatalf("Expected node's down to be %v, received %v", down, node.down)
	}
}

func TestShouldUpdateRightPointerInTheNode(t *testing.T) {
	node := newNode([]byte("HDD"), []byte("Hard disk drive"))
	right := newNode([]byte("SDD"), []byte("Solid state drive"))
	node.updateRight(right)

	if node.right != right {
		t.Fatalf("Expected node's right to be %v, received %v", right, node.right)
	}
}
