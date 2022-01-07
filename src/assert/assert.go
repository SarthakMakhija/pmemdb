package assert

func Assert(condition bool, failureMessage string) {
	if !condition {
		panic(failureMessage)
	}
}
