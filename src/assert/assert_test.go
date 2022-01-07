package assert

import (
	"fmt"
	"github.com/stretchr/testify/assert"
	"strings"
	"testing"
)

func TestAssertShouldPanicGivenConditionIsNotMet(t *testing.T) {
	str := "persistent memory"

	assert.Panics(t, func() {
		Assert(strings.Contains(str, "pmem"), fmt.Sprintf("pmem is not contained in %v", str))
	})
}

func TestAssertShouldNotPanicGivenConditionIsMet(t *testing.T) {
	str := "persistent memory"

	assert.NotPanics(t, func() {
		Assert(strings.Contains(str, "persistent"), fmt.Sprintf("persistent is not contained in %v", str))
	})
}
