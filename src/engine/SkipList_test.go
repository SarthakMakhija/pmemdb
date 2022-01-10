package engine

import (
	"github.com/stretchr/testify/assert"
	"testing"
)

func TestFailsWhileCreatingASkipListWithTowerSizeEqualToZero(t *testing.T) {
	assert.Panics(t, func() {
		NewSkipList(0)
	})
}

func TestFailsWhileCreatingASkipListWithTowerSizeLessThanZero(t *testing.T) {
	assert.Panics(t, func() {
		NewSkipList(-1)
	})
}

func TestCreatesASkipListAndGetsAValueByKey(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))

	value, _ := skipList.GetByKey([]byte("HDD"))
	assert.Equalf(t, "Hard disk drive", string(value), "Expected value to be %v, received %v", "Hard disk drive", string(value))
}

func TestCreatesASkipListAndGetsAValueByKeyWithIncreaseInTowerSize1(t *testing.T) {
	skipList := NewSkipList(3)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("NVMe"))
	assert.Equalf(t, "Non Volatile memory express", string(value), "Expected value to be %v, received %v", "Non Volatile memory express", string(value))
}

func TestCreatesASkipListAndGetsAValueByKeyWithIncreaseInTowerSize2(t *testing.T) {
	skipList := NewSkipList(1)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("NVMe"))
	assert.Equalf(t, "Non Volatile memory express", string(value), "Expected value to be %v, received %v", "Non Volatile memory express", string(value))
}

func TestCreatesASkipListAndGetsAValueByKeyGivenKeyDoesNotExist(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))
	skipList.Put([]byte("SDD"), []byte("Solid State Drive"))
	skipList.Put([]byte("NVMe"), []byte("Non Volatile memory express"))

	value, _ := skipList.GetByKey([]byte("HDD@"))
	assert.Nilf(t, value, "Expected value to be %v, received %v", nil, value)
}

func TestUpdatesTheValueOfAnExistingKey(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))

	skipList.Put([]byte("HDD"), []byte("Hard disk"))
	value, _ := skipList.GetByKey([]byte("HDD"))

	assert.Equalf(t, "Hard disk", string(value), "Expected value to be %v, received %v", "Hard disk", string(value))
}

func TestCreatesASkipListAndAttemptsToGetAValueByNonExistentKey(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))

	value, _ := skipList.GetByKey([]byte("SDD"))
	assert.Equalf(t, "", string(value), "Expected value to be %v, received %v", "", string(value))
}

func TestCreatesASkipListAndAttemptsToGetPresenceOfValueByNonExistentKey(t *testing.T) {
	skipList := NewSkipList(5)
	skipList.Put([]byte("HDD"), []byte("Hard disk drive"))
	skipList.Put([]byte("Pmem"), []byte("Persistent memory"))

	_, ok := skipList.GetByKey([]byte("SDD"))
	assert.Falsef(t, ok, "Expected false while getting a value for non-existent key")
}
