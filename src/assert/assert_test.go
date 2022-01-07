package assert

import (
	"fmt"
	"strings"
	"testing"
)

func TestAssertShouldPanicGivenConditionIsNotMet(t *testing.T) {
	defer func() { recover() }()

	str := "persistent memory"
	Assert(strings.Contains(str, "pmem"), fmt.Sprintf("pmem is not contained in %v", str))

	t.Errorf("Did not panic")
}

func TestAssertShouldNotPanicGivenConditionIsMet(t *testing.T) {
	str := "persistent memory"
	Assert(strings.Contains(str, "persistent"), fmt.Sprintf("persistent is not contained in %v", str))
}
