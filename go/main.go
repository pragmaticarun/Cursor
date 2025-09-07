package main

import (
	"context"
	"fmt"
	"os"
	"path/filepath"
	"strings"
	"time"
)

type User struct {
	ID   int
	Name string
}

func WordCount(s string) map[string]int {
	m := map[string]int{}
	for _, w := range strings.Fields(strings.ToLower(s)) {
		m[w]++
	}
	return m
}

func main() {
	fmt.Println("=== Go demo ===")

	u := User{ID: 1, Name: "Ada"}
	fmt.Printf("User: %+v\n", u)

	fmt.Println("WordCount:", WordCount("Go go gophers!"))

	here, _ := os.Getwd()
	entries, _ := os.ReadDir(filepath.Dir(here))
	for i, e := range entries {
		if i >= 3 {
			break
		}
		fmt.Println("-", e.Name())
	}

	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Millisecond)
	defer cancel()
	select {
	case <-time.After(1 * time.Millisecond):
		fmt.Println("timer fired")
	case <-ctx.Done():
		fmt.Println("timeout")
	}
}