package engine

import (
	"testing"
)

func TestFailsWhileCreatingASkipListWithTowerSizeEqualToZero(t *testing.T) {
	defer func() { recover() }()

	NewSkipList(0)

	t.Errorf("Did not panic")
}

func TestFailsWhileCreatingASkipListWithTowerSizeLessThanZero(t *testing.T) {
	defer func() { recover() }()

	NewSkipList(-1)

	t.Errorf("Did not panic")
}

func TestCreatesASkipListAndGetsAValueByKey(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))

	value, _ := skipList.GetByKey([]byte("HDD"))
	if string(value) != ("Hard disk drive") {
		t.Fatalf("Expected value to be %v, received %v", "Hard disk drive", string(value))
	}
}

func TestCreatesASkipListAndGetsAValueByKeyWithIncreaseInTowerSize1(t *testing.T) {
	skipList := NewSkipList(3)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("NVMe"))
	if string(value) != ("Non Volatile memory express") {
		t.Fatalf("Expected value to be %v, received %v", "Non Volatile memory express", string(value))
	}
}

func TestCreatesASkipListAndGetsAValueByKeyWithIncreaseInTowerSize2(t *testing.T) {
	skipList := NewSkipList(1)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("NVMe"))
	if string(value) != ("Non Volatile memory express") {
		t.Fatalf("Expected value to be %v, received %v", "Non Volatile memory express", string(value))
	}
}

func TestCreatesASkipListAndGetsAValueByKeyGivenKeyDoesNotExist(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("HDD@"))
	if value != nil {
		t.Fatalf("Expected value to be %v, received %v", nil, value)
	}
}
