package main

import "testing"

func TestWordCount(t *testing.T) {
	c := WordCount("Hello hello world")
	if c["hello"] != 2 || c["world"] != 1 {
		t.Fatalf("unexpected counts: %#v", c)
	}
}