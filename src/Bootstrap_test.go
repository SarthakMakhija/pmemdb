package src

import (
	"strings"
	"testing"
)

func TestBootstrap(t *testing.T) {
	str, substr := "persistent memory", "persistent"
	index := strings.Index(str, substr)

	if index != 0 {
		t.Fatalf("Expected %v at index 0 in %v but was %v", substr, str, index)
	}
}
