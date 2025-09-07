package main

import (
	"context"
	"fmt"
	"net/http"
	"time"
)

func main() {
	srv := &http.Server{Addr: ":0"}
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintln(w, "ok")
	})
	go srv.ListenAndServe()
	ctx, cancel := context.WithTimeout(context.Background(), 50*time.Millisecond)
	defer cancel()
	_ = srv.Shutdown(ctx)
}